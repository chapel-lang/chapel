/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Support for parsing and writing TOML files.

Chapel's Library for `Tom's Obvious, Minimal Language (TOML)
<https://github.com/toml-lang/toml>`_.

  .. note::

    The planned features and known limitations of this module can be found in
    `Improve Toml issue <https://github.com/chapel-lang/chapel/issues/7104>`_.


*/
pragma "error mode fatal"
module TOML {


private use List;
private use Map;
public use TomlParser;
private use TomlReader;
private use Sort;
use IO;


/* Receives a TOML file as a parameter and outputs a Toml object.


.. code-block:: chapel

     use TOML;

     const tomlFile = open("example.toml", ioMode.r);
     const toml = parseToml(tomlFile);

To read tables of a TOML file, use the same syntax as accessing associative arrays. For example,
to access to the following TOML file's project name,

.. code-block:: yaml

     [root]
     author = "Sam Partee"
     name = "example"
     version = "1.0.0"

Use the following code in chapel.

.. code-block:: chapel

     use TOML;

     const tomlFile = open("example.toml", ioMode.r);
     const toml = parseToml(tomlFile);
     const projectName = ["root"]["name"] // returns a TOML object
     writeln(projectName.toString());     // to turn TOML object into string representation


.. note::

  As of Chapel 1.26.0, TOML objects will print their values in the following manner:
  If the object contains a `root` table, it will be printed first.
  Keys within the root table will be printed in sorted order.
  All other tables will be printed in a sorted order after `root`, if it exists.
  All table keys will be printed in a sorted order. Prior to this change, the `root`
  table would print first, followed by keys and other tables in what may have been
  a non-deterministic manner.

*/
proc parseToml(input: file) : shared Toml {
  var tomlStr: string;
  var tomlFile = input.reader();
  tomlFile.readAll(tomlStr);
  tomlFile.close();
  return parseToml(tomlStr);
}

/* Receives a channel to a TOML file as a parameter and outputs a Toml object.
*/
proc parseToml(input: fileReader) : shared Toml {
  var tomlStr: string;
  input.readAll(tomlStr);
  return parseToml(tomlStr);
}

 /* Receives a string of TOML format as a parameter and outputs a Toml object */
proc parseToml(input: string) : shared Toml {
  var D: domain(string);
  var table: [D] shared Toml?;
  var rootTable = new shared Toml(table);
  const source = new unmanaged Source(input);
  const parser = new unmanaged Parser(source, rootTable);
  const tomlData = parser.parseLoop();
  delete parser;
  delete source;
  return tomlData;
}


class TomlError : Error {

  var msg:string;
  proc init(msg:string) {
    this.msg = msg;
  }
  override proc message() {
    return msg;
  }
}

/*
Parser module with the Toml class for the Chapel TOML library.
*/
module TomlParser {

  private use Regex;
  use Time;
  use Map, List;
  import IO.fileWriter;
  private use TOML.TomlReader;
  import TOML.TomlError;
  use Sort;

  /* Prints a line by line output of parsing process */
  config const debugTomlParser = false;

  /* Number of spaces in an indentation for JSON output */
  @chpldoc.nodoc
  const tabSpace = 4;

  @chpldoc.nodoc
  class Parser {

    var source: Source;
    var rootTable: shared Toml;
    var curTable: string;

    const doubleQuotes = '"[^,]*?"',
      singleQuotes = "'[^,]*?'",
      bracket = '\\[|\\]',
      digit = "\\d+",
      keys = "^\\w+";
    const Str = new regex(doubleQuotes + '|' + singleQuotes),
      kv = new regex('|'.join(doubleQuotes, singleQuotes, digit, keys)),
      dt = new regex('^\\d{4}-\\d{2}-\\d{2}[ T]\\d{2}:\\d{2}:\\d{2}$'),
      realNum = new regex("\\+\\d*\\.\\d+|\\-\\d*\\.\\d+|\\d*\\.\\d+"),
      ld = new regex('^\\d{4}-\\d{2}-\\d{2}$'),
      ti = new regex('^\\d{2}:\\d{2}:\\d{2}(.\\d{6,})?$'),
      ints = new regex("(\\d+|\\+\\d+|\\-\\d+)"),
      inBrackets = new regex("(\\[.*?\\])"),
      corner = new regex("(\\[.+\\])"),
      brackets = new regex('\\[|\\]'),
      whitespace = new regex("\\s"),
      comment = new regex("(\\#)"),
      comma = new regex("(\\,)");

    var debugCounter = 1;

    proc parseLoop() : shared Toml throws {
      try! {

        if !source.isEmpty() {
          while(readLine(source)) {
            var token = top(source);
            if comment.match(token) {
              parseComment();
            }
            else if inBrackets.match(token) {
              parseTable();
            }
            else if brackets.match(token) {
              parseSubTbl();
            }
            else if kv.match(token) {
              parseAssign();
            }
            else if token == "\n" {
              getToken(source);
            }
            else {
              throw new owned TomlError("Line "+ debugCounter:string +": Unexpected Token -> " + getToken(source));
            }
            debugCounter += 1;
            if debugTomlParser {
              debugPrint();
            }
          }
        }
      }
      catch e: TomlError {
        writeln(e.message());
        exit(1);
      }
      return rootTable;
    }

    proc parseTable() {
      var toke = getToken(source);
      var tablename = toke.replace(brackets, '');
      var tblD: domain(string);
      var tbl: [tblD] shared Toml?;
      if !rootTable.pathExists(tablename) {
        rootTable.set(tablename, tbl);
      }
      curTable = tablename;
    }

    proc parseSubTbl() {
      skipNext(source);
      var tblname = getToken(source);
      skipNext(source);
      var tblD: domain(string);
      var tbl: [tblD] shared Toml?;
      var (tblPath, tblLeaf) = splitTblPath(tblname);
      if !rootTable.pathExists(tblPath) then makePath(tblPath);
      rootTable[tblPath]!.set(tblLeaf, tbl);
      curTable = tblname;
    }

    /* Creates a path to a sub-table when no parent has been initialized */
    proc makePath(tblPath: string) {
      var path = tblPath.split('.');
      var firstIn = path.domain.first;
      var first = true;
      var i: int = 0;
      for parent in path {
        if first {
          var tblD: domain(string);
          var tbl: [tblD] shared Toml?;
          rootTable.set(parent, tbl);
          first = false;
        }
        else {
          var tblD: domain(string);
          var tbl: [tblD] shared Toml?;
          var grandParent = '.'.join(path[..firstIn+i]);
          rootTable[grandParent]!.set(parent, tbl);
          i+=1;
        }
      }
    }

    proc parseInlineTbl(key: string) {
      var tblname: string;
      var tblD: domain(string);
      var tbl: [tblD] shared Toml?;
      if curTable.isEmpty() {
        tblname = key;
        rootTable.set(key, tbl);
      }
      else {
        tblname = '.'.join(curTable, key);
        var (tblPath, tblLeaf) = splitTblPath(tblname);
        rootTable[tblPath]!.set(tblLeaf, tbl);
      }
      var temp = curTable;
      curTable = tblname;
      while top(source) != '}' {
        parseAssign();
        if top(source) == ',' {
          skipNext(source);
        }
      }
      skipNext(source);
      curTable = temp; // resets curTable after assignments to inline
    }

    proc parseAssign() {
      try! {
        var key = getToken(source);
        var equals = getToken(source);
        if top(source) == '{' {
          skipNext(source);
          parseInlineTbl(key);
        }
        else {
          var value = parseValue();
          if curTable.isEmpty() then rootTable[key] = value;
          else rootTable[curTable]![key] = value;
        }
      }
      catch e: TomlError {
        writeln(e.message());
        exit(1);
      }
    }

    proc parseComment() {
      skipLine(source);
    }

    /* Returns leaf of embedded table */
    proc splitTblPath(s: string) {
      var A = s.split('.');
      var fIdx = A.domain.first;
      var leaf = A[A.domain.last];
      var path = '.'.join(A[..A.domain.last-1]);
      if A.size == 1 then path = A[fIdx];
      return (path, leaf);
    }

    /* Creates and returns a Toml parsed from tokens into respective type */
    proc parseValue(): shared Toml throws {
      var val: string;
      try! {
        val = top(source);
        // Array
        if val == '['  {
          skipNext(source);
          var array: list(shared Toml);
          while top(source) != ']' {
            if comma.match(top(source)) {
              skipNext(source);
            }
            else if comment.match(top(source)) {
              skipNext(source);
            }
            else {
              var toParse = parseValue();
              array.pushBack(toParse);
            }
          }
          skipNext(source);
          return new shared Toml(array);
        }
        // Strings (includes multi-line)
        else if Str.match(val) {
          var toStr: string;
          if val.startsWith('"""') {
            toStr += getToken(source).strip('"""', true, false);
            while toStr.endsWith('"""') == false {
              toStr += " " + getToken(source);
            }
            return new shared Toml(toStr.strip('"""'));
          }
          else if val.startsWith("'''") {
            toStr += getToken(source).strip("'''", true, false);
            while toStr.endsWith("'''") == false {
              toStr += " " + getToken(source);
            }
            return new shared Toml(toStr.strip("'''"));
          }
          else {
            toStr = getToken(source).strip('"').strip("'");
            return new shared Toml(toStr);
          }
        }
        // DateTime
        else if dt.match(val) {
          var date = dateTime.strptime(getToken(source), "%Y-%m-%dT%H:%M:%SZ");
          return new shared Toml(date);
        }
        // Date
        else if ld.match(val) {
          var raw = getToken(source).split("-");
          var d = new date(raw[0]: int,
                           raw[1]: int,
                           raw[2]: int);
          return new shared Toml(d);
        }
        // Time
        else if ti.match(val) {
          use IO;
          var raw = getToken(source).split(":");
          var sec = '%.6dr'.format(raw[2]: real).split('.');
          var t: time;

          t = new time(raw[0]: int,
                       raw[1]: int,
                       sec[0]: int,
                       sec[1]: int);

          return new shared Toml(t);
        }
        // Real
        else if realNum.match(val) {
         var token = getToken(source);
         var toReal = token: real;
         return new shared Toml(toReal);
        }
        // Int
        else if ints.match(val) {
          var token = getToken(source);
          var toInt = token: int;
          return new shared Toml(toInt);
        }
        // Boolean
        else if val == "true" || val ==  "false" {
          var token = getToken(source);
          var toBool = token: bool;
          return new shared Toml(toBool);
        }
        // Comments within arrays
        else if val == '#' {
          skipLine(source);
          return parseValue();
        }
        else if corner.match(val) {
          var token = getToken(source);
          var value =  token.strip(bracket);
          var toAdd = [']', value, '['];
          addToken(source, toAdd);
          return parseValue();
        }
        // Error
        else {
          throw new owned TomlError("Line "+ debugCounter:string +": Unexpected Token -> " + getToken(source));
          return new shared Toml(val);
        }
      }
      catch e: IllegalArgumentError {
        writeln("Line "+ debugCounter:string +": Illegal Value -> " + val);
        exit(1);
      }
      catch e: TomlError {
        writeln(e.message());
        exit(1);
      }
    }

    proc debugPrint() {
      writeln(debugCounter, ':');
      writeln(rootTable);
      // Pointer to the line we are currently on
      write('->');
      source.debug();
      writeln();
    }

  }


@chpldoc.nodoc
// Enum for Toml class field: tag
 enum fieldtag {
   fieldBool,
   fieldInt,
   fieldArr,
   fieldToml,
   fieldReal,
   fieldString,
   fieldEmpty,
   fieldDate,
   fieldTime,
   fieldDateTime };
 private use fieldtag;

 @chpldoc.nodoc
 operator Toml.=(ref t: shared Toml, s: string) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new shared Toml(s);
   } else {
     t.tag = fieldString;
     t.s = s;
   }
 }

 @chpldoc.nodoc
 operator Toml.=(ref t: shared Toml, i: int) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new shared Toml(i);
   } else {
     t.tag = fieldInt;
     t.i = i;
   }
 }

 @chpldoc.nodoc
 operator Toml.=(ref t: shared Toml, b: bool) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new shared Toml(b);
   } else {
     t.tag = fieldBool;
     t.boo = b;
   }
 }

 @chpldoc.nodoc
 operator Toml.=(ref t: shared Toml, r: real) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new shared Toml(r);
   } else {
     t.tag = fieldReal;
     t.re = r;
   }
 }

 @chpldoc.nodoc
 operator Toml.=(ref t: shared Toml, ld: date) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new shared Toml(ld);
   } else {
     t.tag = fieldDate;
     t.ld = ld;
   }
 }

 @chpldoc.nodoc
 operator Toml.=(ref t: shared Toml, ti: time) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new shared Toml(ti);
   } else {
     t.tag = fieldTime;
     t.ti = ti;
   }
 }

 @chpldoc.nodoc
 operator Toml.=(ref t: shared Toml, dt: dateTime) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new shared Toml(dt);
   } else {
     t.tag = fieldDateTime;
     t.dt = dt;
   }
 }

 @chpldoc.nodoc
 operator Toml.=(ref t: shared Toml,
                 A: [?D] shared Toml) where D.isAssociative() {
   compilerWarning("= overloads for Toml are deprecated");
   setupToml(t, A);
 }
 @chpldoc.nodoc
 proc setupToml(ref t: shared Toml, A: [?D] shared Toml) where D.isAssociative() {
   if t == nil {
     t = new shared Toml(A);
   } else {
     t.tag = fieldToml;
     t.D = D;
     t.A = A;
   }
 }

 @chpldoc.nodoc
 proc setupToml(ref t: shared Toml, arr: [?dom] shared Toml) where !dom.isAssociative(){
   if t == nil {
     t = new shared Toml(arr);
   } else {
     t.tag = fieldArr;
     t.dom = dom;
     t.arr = arr;
   }
 }


 @chpldoc.nodoc
 operator Toml.=(ref t: shared Toml, arr: [?dom] shared Toml) where !dom.isAssociative(){
   compilerWarning("= overloads for Toml are deprecated");
   setupToml(t, arr);
 }


/*
Class to hold various types parsed from input
used to recursively hold tables and respective values
*/
  class Toml {

    @chpldoc.nodoc
    var i: int,
      boo: bool,
      re: real,
      s: string,
      ld: date,
      ti: time,
      dt: dateTime,
      dom: domain(1),
      arr: [dom] shared Toml?,
      A: map(string, shared Toml?, false),
      tag: fieldtag;

    // Empty
    proc init() {
      this.tag = fieldEmpty;
    }

    // String
    proc init(s:string) {
      this.s = s;
      this.tag = fieldString;
    }

    // Toml
    proc init(A: [?D] shared Toml) where D.isAssociative() {
      this.complete();
      for i in D do this.A[i] = A[i];
      this.tag = fieldToml;
    }

    @chpldoc.nodoc
    proc init(A: [?D] shared Toml?) where D.isAssociative() {
      this.complete();
      for i in D do this.A[i] = A[i];
      this.tag = fieldToml;
    }

    // Date
    proc init(ld: date) {
      this.ld = ld;
      this.tag = fieldDate;
    }

    // Time
    proc init(ti: time) {
       this.ti = ti;
       this.tag = fieldTime;
    }

    // Datetime
    proc init(dt: dateTime) {
      this.dt = dt;
      this.tag = fieldDateTime;
    }

    // Int
    proc init(i: int) {
      this.i = i;
      this.tag = fieldInt;
    }

    // Boolean
    proc init(boo: bool) {
      this.boo = boo;
      this.tag = fieldBool;
    }

    // Real
    proc init(re: real) {
      this.re = re;
      this.tag = fieldReal;
    }

    // Array
    proc init(arr: [?dom] shared Toml) where dom.isAssociative() == false  {
      this.dom = dom;
      this.arr = arr;
      this.tag = fieldArr;
    }

    @chpldoc.nodoc
    proc init(arr: [?dom] shared Toml?) where dom.isAssociative() == false  {
      this.dom = dom;
      this.arr = arr;
      this.tag = fieldArr;
    }

    // List
    proc init(lst: list(shared Toml)) {
      // Cheat by translating directly into an array for now.
      this.init(lst.toArray());
    }

    @chpldoc.nodoc
    proc init(lst: list(shared Toml?)) {
      // Cheat by translating directly into an array for now.
      this.init(lst.toArray());
    }


    // Clone
    proc init(root: Toml) {
      // INIT TODO: Can this be written in phase one?
      this.complete();
      this.boo = root.boo;
      this.i = root.i;
      this.re = root.re;
      this.dom = root.dom;
      for idx in root.dom do this.arr[idx] = new shared Toml(root.arr[idx]!)?;
      this.ld = root.ld;
      this.ti = root.ti;
      this.dt = root.dt;
      this.s = root.s;
      for idx in root.A.keys() do this.A[idx] = new shared Toml(root.A[idx]!)?;
      this.tag = root.tag;
    }


    /* Returns the index of the table path given as a parameter */
    proc this(tbl: string) ref : shared Toml? throws {
      const indx = tbl.split('.');
      var top = indx.domain.first;

      //
      // TODO: This is a bug when the return type of this routine is a
      // non-nilable class, see #14367/#14861. So for now, we have to make
      // `Toml.this` return `Toml?`, which makes it a lot less pleasant
      // to use.
      //
      if indx.size < 2 {
        return this.A[tbl];
      } else {
        var next = '.'.join(indx[top+1..]);
        if !this.A.contains(indx[top]) {
          throw new owned TomlError("No index found for " + tbl);
        }
        return this.A[indx[top]]![next];
      }
    }

    @chpldoc.nodoc
    /* Returns true if table path exists in rootTable */
    proc pathExists(tblpath: string) : bool {
      try! {
        var path = tblpath.split('.');
        var top = path.domain.first;
        if path.size < 2 {
          if this.A.contains(tblpath) == false {
            return false;
          }
          else {
            return true;
          }
        }
        else {
          var next = '.'.join(path[top+1..]);
          if this.A.contains(path[top]) {
            return this.A[path[top]]!.pathExists(next);
          }
          else {
            return false;
          }
        }
      }
      catch e: TomlError {
        writeln(e.message());
      }
      return false;
    }

    proc set(tbl: string, toml: Toml) {
      ref t = this(tbl);
      t = new shared Toml(toml);
    }

    proc set(tbl: string, s: string) {
      ref t = this(tbl);
      if t == nil {
        t = new shared Toml(s);
      } else {
        t!.tag = fieldString;
        t!.s = s;
      }
    }
    proc set(tbl: string, i: int) {
      ref t = this(tbl);
      if t == nil {
        t = new shared Toml(i);
      } else {
        t!.tag = fieldInt;
        t!.i = i;
      }
    }
    proc set(tbl: string, b: bool) {
      ref t = this(tbl);
      if t == nil {
        t = new shared Toml(b);
      } else {
        t!.tag = fieldBool;
        t!.boo = b;
      }
    }
    proc set(tbl: string, r: real) {
      ref t = this(tbl);
      if t == nil {
        t = new shared Toml(r);
      } else {
        t!.tag = fieldReal;
        t!.re = r;
      }
    }
    proc set(tbl: string, ld: date) {
      ref t = this(tbl);
      if t == nil {
        t = new shared Toml(ld);
      } else {
        t!.tag = fieldDate;
        t!.ld = ld;
      }
    }
    proc set(tbl: string, ti: time) {
      ref t = this(tbl);
      if t == nil {
        t = new shared Toml(ti);
      } else {
        t!.tag = fieldTime;
        t!.ti = ti;
      }
    }
    proc set(tbl: string, dt: dateTime) {
      ref t = this(tbl);
      if t == nil {
        t = new shared Toml(dt);
      } else {
        t!.tag = fieldDateTime;
        t!.dt = dt;
      }
    }
    proc set(tbl: string, A: [?D] shared Toml?) where D.isAssociative() {
      ref t = this(tbl);
      if t == nil {
        t = new shared Toml(A);
      } else {
        t!.tag = fieldToml;
        for i in D do t!.A[i] = A[i];
      }
    }
    proc set(tbl: string, arr: [?dom] shared Toml?) where !dom.isAssociative() {
      ref t = this(tbl);
      if t == nil {
        t = new shared Toml(arr);
      } else {
        t!.tag = fieldArr;
        t!.dom = dom;
        t!.arr = arr;
      }
    }


    /* Write a Table to channel f in TOML format */
    override proc writeThis(f) throws {
      writeTOML(f);
    }

    /* Write a Table to channel f in TOML format */
    proc writeTOML(f) {
      try! {
        var flat = new map(string, shared Toml?);
        this.flatten(flat);       // Flattens containing Toml
        printValues(f, this);     // Prints key values in containing Toml
        printTables(flat, f);       // Prints tables in containing Toml
      }
      catch e: TomlError {
        writeln(e.message());
      }
    }

    /* Write a Table to channel f in JSON format */
    proc writeJSON(f) {
      try! {
        var flat = new map(string, shared Toml?);
        this.flatten(flat);           // Flattens containing Toml

        var indent=0;

        f.writeln('{');
        indent += tabSpace;

        // Prints key values in containing Toml
        printValuesJSON(f, this, indent=indent);

        if flat.contains('root') {
          printValuesJSON(f, flat['root']!, indent=indent);
          flat.remove('root');
        }
        for k in sorted(flat.keysToArray()) {
          f.writef('%s"%s": {\n', ' '*indent, k);
          indent += tabSpace;
          printValuesJSON(f, flat[k]!, indent=indent);
          indent -= tabSpace;
          f.writef('%s}\n', ' '*indent);
        }
        indent -= tabSpace;
        f.writeln('}');
      }
      catch e: TomlError {
        writeln(e.message());
      }
    }



    @chpldoc.nodoc
    /* Flatten tables into flat associative array for writing */
    proc flatten(ref flat: map(string, shared Toml?, false), rootKey = '') : flat.type {
      for (k, v) in zip(this.A.keys(), this.A.values()) {
        if v!.tag == fieldToml {
          var fullKey = k;
          if rootKey != '' then fullKey = '.'.join(rootKey, k);
          flat[fullKey] = v;
          v!.flatten(flat, fullKey);
        }
      }
      return flat;
    }

    @chpldoc.nodoc
    proc printTables(ref flat: map(string, shared Toml?, false), f:fileWriter) {
      if flat.contains('root') {
        f.writeln('[root]');
        printValues(f, flat['root']!);
        flat.remove('root');
      }
      for k in sorted(flat.keysToArray()) {
        f.writeln('[', k, ']');
        printValues(f, flat[k]!);
      }
    }

    @chpldoc.nodoc
    /* Send values from table to toString for writing  */
    proc printValues(f: fileWriter, v: borrowed Toml) throws {
      var keys = v.A.keysToArray();
      sort(keys);
      for key in keys {
        var value = v.A[key]!;
        select value.tag {
          when fieldToml do continue; // Table
          when fieldBool {
            f.write(key, ' = ', toString(value));
          }
          when fieldInt {
            f.write(key, ' = ', toString(value));
          }
          when fieldArr {
            var final: string;
            f.write(key, ' = ');
            final += '[';
            for k in value.arr {
              if value.arr.domain.size == 1 || k == value.arr[value.arr.domain.last] {
                final += toString(k!);
              }
              else {
                final += toString(k!) + ', ';
              }
            }
            final += ']';
            f.write(final);
          }
          when fieldReal {
            f.write(key, ' = ', toString(value));
          }
          when fieldString {
            f.write(key, ' = ', toString(value));
          }
          when fieldEmpty {
            throw new owned TomlError("Keys must have a value");
          }
          when fieldDate {
            f.write(key, ' = ', toString(value));
          }
          when fieldTime {
            f.write(key, ' = ', toString(value));
          }
          when fieldDateTime {
            f.write(key, ' = ', toString(value));
          }
          otherwise {
            throw new owned TomlError("Not yet supported");
          }
        }
        f.writeln();
      }
      f.writeln();
    }

    @chpldoc.nodoc
    /* Send values from table to toString for writing  */
    proc printValuesJSON(f: fileWriter, v: borrowed Toml, in indent=0) throws {
      var keys = v.A.keysToArray();
      sort(keys);
      for (key, i) in zip(keys, 1..v.A.size) {
        var value = v.A[key]!;
        select value.tag {
          when fieldToml do continue; // Table
          when fieldBool {
            f.writef('%s"%s": {"type": "%s", "value": "%s"}', ' '*indent, key, value.tomlType, toString(value));
          }
          when fieldInt {
            f.writef('%s"%s": {"type": "%s", "value": "%s"}', ' '*indent, key, value.tomlType, toString(value));
          }
          when fieldArr {
            f.writef('%s"%s": {\n', ' '*indent, key);
            indent += tabSpace;
            f.writef('%s"%s": "type",\n', ' '*indent, this.tomlType);
            f.writef('%s"value": [\n', ' '*indent);
            indent += tabSpace;
            var arrayElements: string;
            for i in value.arr.domain {
              ref k = value.arr[i];
              f.writef('%s{"type": "%s", "value": "%s"}', ' '*indent, k!.tomlType, toString(k!));
              if i != value.arr.domain.last {
                f.writef(',');
              }
              f.writef('\n');
            }
            indent -= tabSpace;
            f.writef('%s]\n', ' '*indent);
            indent -= tabSpace;
            f.writef('%s}\n', ' '*indent);
          }
          when fieldReal {
            f.writef('%s"%s": {"type": "%s", "value": "%s"}', ' '*indent, key, value.tomlType, toString(value));
          }
          when fieldString {
            f.writef('%s"%s": {"type": "%s", "value": "%s"}', ' '*indent, key, value.tomlType, toString(value));
          }
          when fieldEmpty {
            throw new owned TomlError("Keys must have a value");
          }
          when fieldDate {
            f.writef('%s"%s": {"type": "%s", "value": "%s"}', ' '*indent, key, value.tomlType, toString(value));
          }
          when fieldTime {
            f.writef('%s"%s": {"type": "%s", "value": "%s"}', ' '*indent, key, value.tomlType, toString(value));
          }
          when fieldDateTime {
            f.writef('%s"%s": {"type": "%s", "value": "%s"}', ' '*indent, key, value.tomlType, toString(value));
          }
          otherwise {
            throw new owned TomlError("Not yet supported");
          }
        }
        if i != v.A.size {
          f.writef(',');
        }
        f.writef('\n');
      }
    }


    @chpldoc.nodoc
    /* Return String representation of a value in a node */
    proc toString(val: borrowed Toml) : string throws {
      select val.tag {
        when fieldBool do return val.boo:string;
        when fieldInt do return val.i:string;
        when fieldArr {
          var final: string;
          final += '[';
          for k in val.arr {
            if val.arr.domain.size == 1 || k == val.arr[val.arr.domain.last] {
              final += toString(k!);
            }
            else {
              final += toString(k!) + ', ';
            }
          }
          final += ']';
          return final;
        }
        when fieldReal do return val.re:string;
        when fieldString do return ('"' + val.s + '"');
        when fieldEmpty do return ""; // empty
        when fieldDate do return val.ld.isoFormat();
        when fieldTime do return val.ti.isoFormat();
        when fieldDateTime do return val.dt.isoFormat();
        otherwise {
          throw new owned TomlError("Error in printing " + val.s);
          return val.s;
        }
      }
    }

    /*
     For the user to write values of a node as follows:
     Toml[key].toString()
    */
     proc toString() : string {
       return toString(this);
    }

    /* Return Toml type as a string.

       Valid types include:

       - *empty*
       - *string*
       - *integer*
       - *float*
       - *boolean*
       - *datetime*
       - *array*
       - *toml* (inline table)

     */
    proc tomlType: string throws {
      select this.tag {
        when fieldBool do return 'bool';
        when fieldInt do return 'integer';
        when fieldArr do return 'array';
        when fieldReal do return 'float';
        when fieldString do return 'string';
        when fieldEmpty do return 'empty';
        when fieldDate do return 'date';
        when fieldTime do return 'time';
        when fieldDateTime do return 'datetime';
        when fieldToml do return 'toml';
        otherwise {
          throw new owned TomlError("Unknown type");
          return "nil";
        }
      }
    }
  }
}


@chpldoc.nodoc
 /*
 Reader module for use in the Parser Class.
 */
module TomlReader {
 use List;
 import TOML.TomlError;

 private use Regex;
 private use IO;

 config const debugTomlReader = false;

  /* Returns the next token in the current line without removing it */
  proc top(source) throws {
    if !source.nextLine() {
      throw new owned TomlError("Reached end of file unexpectedly");
    }
    return source.currentLine![0];
  }

  /* Returns a boolean or whether or not another line can be read
  /  Also updates the currentLine if empty */
  proc readLine(source) {
    return source.nextLine();
  }

  proc skipNext(source) {
    source.currentLine!.skip();
  }

  proc addToken(source, tokensToAdd: [?dom] string) {
    for toke in tokensToAdd {
      source.currentLine!.addToke(toke);
    }
  }

  proc skipLine(source) {
    var emptyList: list(string);
    var emptyCurrent = new unmanaged Tokens(emptyList);
    var ptrhold = source.currentLine;
    source.currentLine = emptyCurrent;
    var readNextLine = readLine(source);
    delete ptrhold;
  }

  /* retrieves the next token in currentLine */
  proc getToken(source) {
    try! {
      return source.nextToke();
    }
    catch e: TomlError {
      writeln(e.message());
      exit(1);
    }
  }


  class Source {

    var tomlStr: string;
    var tokenlist: list(unmanaged Tokens);
    var currentLine: unmanaged Tokens?;


    proc init(tomlStr: string) {
     this.tomlStr = tomlStr;
     this.complete();
     genTokenlist(tomlStr);
    }

    proc genTokenlist(input: string) {
      for line in input.split('\n') {
        splitLine(line);
      }
      if !this.isEmpty() {
        currentLine = tokenlist[0];
      }
    }

    proc isEmpty() {
      return tokenlist.size == 0;
    }

    proc splitLine(line) {
      var idx = 0;
      var linetokens: list(string);
      var nonEmptyChar: bool = false;

      const doubleQuotes = '("[^,]*?")',           // ""
            singleQuotes = "('[^,]*?')",           // ''
            bracketContents = "(\\[\\w+\\])",   // [_]
            brackets = "(\\[)|(\\])",           // []
            // TODO: fix table headers
            //tblName = '(\\w+."[^"]+")',         // [somename."0.1.0"]
            comments = "(\\#)",                 // #
            commas = "(\\,)",                   // ,
            equals = "(\\=)",                   // =
            curly = "(\\{)|(\\})",              // {}
            dt = "^\\d{4}-\\d{2}-\\d{2}[ T]\\d{2}:\\d{2}:\\d{2}",
            ld = "^\\d{4}-\\d{2}-\\d{2}",
            ti = "^\\d{2}:\\d{2}:\\d{2}(.\\d{6,})?";

      const pattern = new regex('|'.join(doubleQuotes,
                                         singleQuotes,
                                         bracketContents,
                                         brackets,
                                         commas,
                                         curly,
                                         equals,
                                         dt,
                                         ti,
                                         ld));

      for token in pattern.split(line) {
        idx += 1;
        var strippedToken = token.strip(" \t");
        if strippedToken.size != 0 {
          if debugTomlReader {
            writeln('Tokenized: ', '(', strippedToken, ')');
          }
          nonEmptyChar = true;
          // check for date/time in a line and avoid comment
          const toke = strippedToken;
          const isWhiteSpace = new regex("\\s");
          var dateTimeToken = isWhiteSpace.split(toke);
          if (new regex('|'.join(dt,ti,ld))).match(strippedToken).matched then
            strippedToken = dateTimeToken[0];
          var isComment = (new regex(comments)).match(strippedToken);
          if isComment.matched && idx <= 1 {
            linetokens.pushBack(strippedToken);
          } else if !isComment.matched {
            linetokens.pushBack(strippedToken);
          }
        }
      }

      // If no non-empty-chars => token is a blank line
      if(nonEmptyChar == false){
        linetokens.pushBack("\n");
      }

      if !linetokens.isEmpty() {
        var tokens = new unmanaged Tokens(linetokens);
        tokenlist.pushBack(tokens);
      }
    }


    proc nextLine() {
      if currentLine!.isEmpty() {
        if tokenlist.size == 1 {
          return false;
        }
        else {
          var ptrhold = currentLine;
          tokenlist.pop(0);
          currentLine = tokenlist[0];
          delete ptrhold;
          return true;
        }
      }
      return true;
    }


    /* retrieves next token in currentLine */
    proc nextToke() throws {
      if !nextLine() {
        throw new owned TomlError("Reached end of file unexpectedly");
      }
      return currentLine!.next();
    }


    proc debug() {
      for line in tokenlist {
        if line.A.size != 0 {
          for token in line {
            if token.size != 0 {
              write("(", token, ")");
            }
          }
          writeln();
        }
      }
    }


    proc deinit() {
      for token in tokenlist {
        delete token;
      }
     }
  }


  /* Array wrapper */
  class Tokens {
    var A: list(string);

    proc init(A: list(string)) {
      this.A = A;
    }

    proc skip() {
      A.pop(0);
    }

    proc next() {
      var toke = A.pop(0);
      return toke;
    }

    proc addToke(toke: string) {
      A.insert(0, toke);
    }

    proc isEmpty(): bool {
      return this.A.isEmpty();
    }

    proc this(idx) ref {
      return this.A[idx];
    }

    iter these() ref {
      for a in this.A {
        yield a;
      }
    }

    @chpldoc.nodoc
    proc readThis(f) throws {
      compilerError("Reading a Tokens type is not supported");
    }

    proc init(r: fileReader) {
      this.complete();
      compilerError("Reading a Tokens type is not supported");
    }

    proc writeThis(f) throws {
      f.write(this.A.toArray());
    }
  }
}

}
