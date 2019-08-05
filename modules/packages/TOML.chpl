/*
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

/*
Chapel's Library for `Tom's Obvious, Minimal Language (TOML)
<https://github.com/toml-lang/toml>`_.
This module provides support for parsing and writing toml files.


  .. note::

    The planned features and known limitations of this module can be found in
    `Improve Toml issue <https://github.com/chapel-lang/chapel/issues/7104>`_.


*/
pragma "error mode fatal"
module TOML {


use TomlParser;
private use TomlReader;


/* Receives a TOML file as a parameter and outputs a Toml object.


.. code-block:: chapel

     use TOML;

     const tomlFile = open("example.toml", iomode.r);
     const toml = parseToml(tomlFile);

To read tables of a TOML file, use the same syntax as accessing associative arrays. For example,
to access to the following TOML file's project name,

.. code-block:: yaml

     [root]
     name = "example"
     version = "1.0.0"
     author = "Sam Partee"

Use the following code in chapel.

.. code-block:: chapel

     use TOML;

     const tomlFile = open("example.toml", iomode.r);
     const toml = parseToml(tomlFile);
     const projectName = ["root"]["name"] // returns a TOML object
     writeln(projectName.toString());     // to turn TOML object into string representation


*/
proc parseToml(input: file) : unmanaged Toml {
  var tomlStr: string;
  var tomlFile = input.reader();
  tomlFile.readstring(tomlStr);
  tomlFile.close();
  return parseToml(tomlStr);
}

/* Receives a channel to a TOML file as a parameter and outputs a Toml object.
*/
proc parseToml(input: channel) : unmanaged Toml {
  var tomlStr: string;
  input.readstring(tomlStr);
  return parseToml(tomlStr);
}

 /* Receives a string of TOML format as a parameter and outputs a Toml object */
proc parseToml(input: string) : unmanaged Toml {
  var D: domain(string);
  var table: [D] unmanaged Toml;
  var rootTable = new unmanaged Toml(table);
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

  private use Regexp;
  use DateTime;


  /* Prints a line by line output of parsing process */
  config const debugTomlParser = false;

  /* Number of spaces in an indentation for JSON output */
  pragma "no doc"
  const tabSpace = 4;

  pragma "no doc"
  class Parser {

    var source: Source;
    var rootTable: unmanaged Toml;
    var curTable: string;

    const doubleQuotes = '".*?"',
      singleQuotes = "'.*?'",
      bracket = '\\[|\\]',
      digit = "\\d+",
      keys = "^\\w+";
    const Str = compile(doubleQuotes + '|' + singleQuotes),
      kv = compile('|'.join(doubleQuotes, singleQuotes, digit, keys)),
      dt = compile('^\\d{4}-\\d{2}-\\d{2}[ T]\\d{2}:\\d{2}:\\d{2}$'),
      realNum = compile("\\+\\d*\\.\\d+|\\-\\d*\\.\\d+|\\d*\\.\\d+"),
      ld = compile('^\\d{4}-\\d{2}-\\d{2}$'),
      ti = compile('^\\d{2}:\\d{2}:\\d{2}(.\\d{6,})?$'),
      ints = compile("(\\d+|\\+\\d+|\\-\\d+)"),
      inBrackets = compile("(\\[.*?\\])"),
      corner = compile("(\\[.+\\])"),
      brackets = compile('\\[|\\]'),
      whitespace = compile("\\s"),
      comment = compile("(\\#)"),
      comma = compile("(\\,)");

    var debugCounter = 1;

    proc parseLoop() : unmanaged Toml throws {
      try! {

        if !source.isEmpty() {
          while(readLine(source)) {
            var token = top(source);

            if token == '#' {
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
              throw new owned TomlError("Line "+ debugCounter +": Unexpected Token -> " + getToken(source));
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
      var tablename = brackets.sub('', toke);
      var tblD: domain(string);
      var tbl: [tblD] unmanaged Toml;
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
      var tbl: [tblD] unmanaged Toml;
      var (tblPath, tblLeaf) = splitTblPath(tblname);
      if !rootTable.pathExists(tblPath) then makePath(tblPath);
      rootTable[tblPath].set(tblLeaf, tbl);
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
          var tbl: [tblD] unmanaged Toml;
          rootTable.set(parent, tbl);
          first = false;
        }
        else {
          var tblD: domain(string);
          var tbl: [tblD] unmanaged Toml;
          var grandParent = '.'.join(path[..firstIn+i]);
          rootTable[grandParent].set(parent, tbl);
          i+=1;
        }
      }
    }

    proc parseInlineTbl(key: string) {
      var tblname: string;
      var tblD: domain(string);
      var tbl: [tblD] unmanaged Toml;
      if curTable.isEmpty() {
        tblname = key;
        rootTable.set(key, tbl);
      }
      else {
        tblname = '.'.join(curTable, key);
        var (tblPath, tblLeaf) = splitTblPath(tblname);
        rootTable[tblPath].set(tblLeaf, tbl);
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
          else rootTable[curTable][key] = value;
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
    proc parseValue(): unmanaged Toml throws {
      var val: string;
      try! {
        val = top(source);
        // Array
        if val == '['  {
          skipNext(source);
          var Dom: domain(1);
          var array: [Dom] unmanaged Toml;
          while top(source) != ']' {
            if comma.match(top(source)) {
              skipNext(source);
            }
            else if comment.match(top(source)) {
              skipLine(source);
            }
            else {
              var toParse = parseValue();
              array.push_back(toParse);
            }
          }
          skipNext(source);
          return new unmanaged Toml(array);
        }
        // Strings (includes multi-line)
        else if Str.match(val) {
          var toStr: string;
          if val.startsWith('"""') {
            toStr += getToken(source).strip('"""', true, false);
            while toStr.endsWith('"""') == false {
              toStr += " " + getToken(source);
            }
            return new unmanaged Toml(toStr.strip('"""'));
          }
          else if val.startsWith("'''") {
            toStr += getToken(source).strip("'''", true, false);
            while toStr.endsWith("'''") == false {
              toStr += " " + getToken(source);
            }
            return new unmanaged Toml(toStr.strip("'''"));
          }
          else {
            toStr = getToken(source).strip('"').strip("'");
            return new unmanaged Toml(toStr);
          }
        }
        // DateTime
        else if dt.match(val) {
          var date = datetime.strptime(getToken(source), "%Y-%m-%dT%H:%M:%SZ");
          return new unmanaged Toml(date);
        }
        // Date
        else if ld.match(val) {
          var raw = getToken(source).split("-");
          var d = new date(raw[1]: int,
                           raw[2]: int,
                           raw[3]: int);
          return new unmanaged Toml(d);
        }
        // Time
        else if ti.match(val) {
          var raw = getToken(source).split(":");
          var sec = '%.6dr'.format(raw[3]: real).split('.');
          var t: time;

          t = new time(raw[1]: int,
                       raw[2]: int,
                       sec[1]: int,
                       sec[2]: int);

          return new unmanaged Toml(t);
        }
        // Real
        else if realNum.match(val) {
         var token = getToken(source);
         var toReal = token: real;
         return new unmanaged Toml(toReal);
        }
        // Int
        else if ints.match(val) {
          var token = getToken(source);
          var toInt = token: int;
          return new unmanaged Toml(toInt);
        }
        // Boolean
        else if val == "true" || val ==  "false" {
          var token = getToken(source);
          var toBool = token: bool;
          return new unmanaged Toml(toBool);
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
          throw new owned TomlError("Line "+ debugCounter +": Unexpected Token -> " + getToken(source));
          return new unmanaged Toml(val);
        }
      }
      catch e: IllegalArgumentError {
        writeln("Line "+ debugCounter +": Illegal Value -> " + val);
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


pragma "no doc"
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

 pragma "no doc"
 proc =(ref t: unmanaged Toml, s: string) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new unmanaged Toml(s);
   } else {
     t.tag = fieldString;
     t.s = s;
   }
 }

 pragma "no doc"
 proc =(ref t: unmanaged Toml, i: int) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new unmanaged Toml(i);
   } else {
     t.tag = fieldInt;
     t.i = i;
   }
 }

 pragma "no doc"
 proc =(ref t: unmanaged Toml, b: bool) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new unmanaged Toml(b);
   } else {
     t.tag = fieldBool;
     t.boo = b;
   }
 }

 pragma "no doc"
 proc =(ref t: unmanaged Toml, r: real) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new unmanaged Toml(r);
   } else {
     t.tag = fieldReal;
     t.re = r;
   }
 }

 pragma "no doc"
 proc =(ref t: unmanaged Toml, ld: date) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new unmanaged Toml(ld);
   } else {
     t.tag = fieldDate;
     t.ld = ld;
   }
 }

 pragma "no doc"
 proc =(ref t: unmanaged Toml, ti: time) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new unmanaged Toml(ti);
   } else {
     t.tag = fieldTime;
     t.ti = ti;
   }
 }

 pragma "no doc"
 proc =(ref t: unmanaged Toml, dt: datetime) {
   compilerWarning("= overloads for Toml are deprecated");
   if t == nil {
     t = new unmanaged Toml(dt);
   } else {
     t.tag = fieldDateTime;
     t.dt = dt;
   }
 }

 pragma "no doc"
 proc =(ref t: unmanaged Toml, A: [?D] unmanaged Toml) where isAssociativeDom(D) {
   compilerWarning("= overloads for Toml are deprecated");
   setupToml(t, A);
 }
 pragma "no doc"
 proc setupToml(ref t: unmanaged Toml, A: [?D] unmanaged Toml) where isAssociativeDom(D) {
   if t == nil {
     t = new unmanaged Toml(A);
   } else {
     t.tag = fieldToml;
     t.D = D;
     t.A = A;
   }
 }

 pragma "no doc"
 proc setupToml(ref t: unmanaged Toml, arr: [?dom] unmanaged Toml) where !isAssociativeDom(dom){
   if t == nil {
     t = new unmanaged Toml(arr);
   } else {
     t.tag = fieldArr;
     t.dom = dom;
     t.arr = arr;
   }
 }


 pragma "no doc"
 proc =(ref t: unmanaged Toml, arr: [?dom] unmanaged Toml) where !isAssociativeDom(dom){
   compilerWarning("= overloads for Toml are deprecated");
   setupToml(t, arr);
 }


/*
Class to hold various types parsed from input
used to recursively hold tables and respective values
*/
  class Toml {

    pragma "no doc"
    var i: int,
      boo: bool,
      re: real,
      s: string,
      ld: date,
      ti: time,
      dt: datetime,
      dom: domain(1),
      arr: [dom] unmanaged Toml,
      D: domain(string),
      A: [D] unmanaged Toml,
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
    proc init(A: [?D] unmanaged Toml) where isAssociativeDom(D) {
      this.D = D;
      this.A = A;
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
    proc init(dt: datetime) {
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
    proc init(arr: [?dom] unmanaged Toml) where isAssociativeDom(dom) == false  {
      this.dom = dom;
      this.arr = arr;
      this.tag = fieldArr;
    }

    // Clone
    proc init(root: unmanaged Toml) {
      // INIT TODO: Can this be written in phase one?
      this.complete();
      this.boo = root.boo;
      this.i = root.i;
      this.re = root.re;
      this.dom = root.dom;
      for idx in root.dom do this.arr[idx] = new unmanaged Toml(root.arr[idx]);
      this.ld = root.ld;
      this.ti = root.ti;
      this.dt = root.dt;
      this.s = root.s;
      this.D = root.D;
      for idx in root.D do this.A[idx] = new unmanaged Toml(root.A[idx]);
      this.tag = root.tag;
    }


    /* Returns the index of the table path given as a parameter */
    proc this(tbl: string) ref : unmanaged Toml throws {
      const indx = tbl.split('.');
      var top = indx.domain.first;
      if indx.size < 2 {
        return this.A[tbl];
      }
      else {
        var next = '.'.join(indx[top+1..]);
        if !this.A.domain.contains(indx[top]) {
          throw new owned TomlError("No index found for " + tbl);
        }
        return this.A[indx[top]][next];
      }
    }

    pragma "no doc"
    /* Returns true if table path exists in rootTable */
    proc pathExists(tblpath: string) : bool {
      try! {
        var path = tblpath.split('.');
        var top = path.domain.first;
        if path.size < 2 {
          if this.A.domain.contains(tblpath) == false {
            return false;
          }
          else {
            return true;
          }
        }
        else {
          var next = '.'.join(path[top+1..]);
          if this.A.domain.contains(path[top]) {
            return this.A[path[top]].pathExists(next);
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

    proc set(tbl: string, toml: unmanaged Toml) {
      ref t = this(tbl);
      if t == nil {
        t = toml;
      } else {
        delete t;
        t = toml;
      }
    }
    proc set(tbl: string, s: string) {
      ref t = this(tbl);
      if t == nil {
        t = new unmanaged Toml(s);
      } else {
        t.tag = fieldString;
        t.s = s;
      }
    }
    proc set(tbl: string, i: int) {
      ref t = this(tbl);
      if t == nil {
        t = new unmanaged Toml(i);
      } else {
        t.tag = fieldInt;
        t.i = i;
      }
    }
    proc set(tbl: string, b: bool) {
      ref t = this(tbl);
      if t == nil {
        t = new unmanaged Toml(b);
      } else {
        t.tag = fieldBool;
        t.boo = b;
      }
    }
    proc set(tbl: string, r: real) {
      ref t = this(tbl);
      if t == nil {
        t = new unmanaged Toml(r);
      } else {
        t.tag = fieldReal;
        t.re = r;
      }
    }
    proc set(tbl: string, ld: date) {
      ref t = this(tbl);
      if t == nil {
        t = new unmanaged Toml(ld);
      } else {
        t.tag = fieldDate;
        t.ld = ld;
      }
    }
    proc set(tbl: string, ti: time) {
      ref t = this(tbl);
      if t == nil {
        t = new unmanaged Toml(ti);
      } else {
        t.tag = fieldTime;
        t.ti = ti;
      }
    }
    proc set(tbl: string, dt: datetime) {
      ref t = this(tbl);
      if t == nil {
        t = new unmanaged Toml(dt);
      } else {
        t.tag = fieldDateTime;
        t.dt = dt;
      }
    }
    proc set(tbl: string, A: [?D] unmanaged Toml) where isAssociativeDom(D) {
      ref t = this(tbl);
      if t == nil {
        t = new unmanaged Toml(A);
      } else {
        t.tag = fieldToml;
        t.D = D;
        t.A = A;
      }
    }
    proc set(tbl: string, arr: [?dom] unmanaged Toml) where !isAssociativeDom(dom) {
      ref t = this(tbl);
      if t == nil {
        t = new unmanaged Toml(arr);
      } else {
        t.tag = fieldArr;
        t.dom = dom;
        t.arr = arr;
      }
    }


    /* Write a Table to channel f in TOML format */
    override proc writeThis(f) {
      writeTOML(f);
    }

    /* Write a Table to channel f in TOML format */
    proc writeTOML(f) {
      try! {
        var flatDom: domain(string);
        var flat: [flatDom] unmanaged Toml;
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
        var flatDom: domain(string);
        var flat: [flatDom] unmanaged Toml;
        this.flatten(flat);           // Flattens containing Toml

        var indent=0;

        f.writeln('{');
        indent += tabSpace;

        // Prints key values in containing Toml
        printValuesJSON(f, this, indent=indent);

        if flatDom.contains('root') {
          printValuesJSON(f, flat['root'], indent=indent);
          flatDom.remove('root');
        }
        for k in flatDom.sorted() {
          f.writef('%s"%s": {\n', ' '*indent, k);
          indent += tabSpace;
          printValuesJSON(f, flat[k], indent=indent);
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



    pragma "no doc"
    /* Flatten tables into flat associative array for writing */
    proc flatten(flat: [?d] unmanaged Toml, rootKey = '') : flat.type {
      for (k, v) in zip(this.D, this.A) {
        if v.tag == fieldToml {
          var fullKey = k;
          if rootKey != '' then fullKey = '.'.join(rootKey, k);
          flat[fullKey] = v;
          v.flatten(flat, fullKey);
        }
      }
      return flat;
    }

    pragma "no doc"
    proc printTables(flat: [?d] unmanaged Toml, f:channel) {
      if d.contains('root') {
        f.writeln('[root]');
        printValues(f, flat['root']);
        d.remove('root');
      }
      for k in d.sorted() {
        f.writeln('[', k, ']');
        printValues(f, flat[k]);
      }
    }

    pragma "no doc"
    /* Send values from table to toString for writing  */
    proc printValues(f: channel, v: borrowed Toml) throws {
      for (key, value) in zip(v.D, v.A) {
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
                final += toString(k);
              }
              else {
                final += toString(k) + ', ';
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

    pragma "no doc"
    /* Send values from table to toString for writing  */
    proc printValuesJSON(f: channel, v: borrowed Toml, in indent=0) throws {
      for (key, value, i) in zip(v.D, v.A, 1..v.D.size) {
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
              f.writef('%s{"type": "%s", "value": "%s"}', ' '*indent, k.tomlType, toString(k));
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
        if i != v.D.size {
          f.writef(',');
        }
        f.writef('\n');
      }
    }


    pragma "no doc"
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
              final += toString(k);
            }
            else {
              final += toString(k) + ', ';
            }
          }
          final += ']';
          return final;
        }
        when fieldReal do return val.re:string;
        when fieldString do return ('"' + val.s + '"');
        when fieldEmpty do return ""; // empty
        when fieldDate do return val.ld.isoformat();
        when fieldTime do return val.ti.isoformat();
        when fieldDateTime do return val.dt.isoformat();
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


    pragma "no doc"
    proc deinit() {
      for a in A do delete a;
      for a in arr do delete a;
    }
  }
}


pragma "no doc"
 /*
 Reader module for use in the Parser Class.
 */
module TomlReader {

 private use Regexp;

 config const debugTomlReader = false;

  /* Returns the next token in the current line without removing it */
  proc top(source) throws {
    if !source.nextLine() {
      throw new owned TomlError("Reached end of file unexpectedly");
    }
    return source.currentLine![source.currentLine!.D.first];
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
    var emptyArray: [1..0] string;
    var emptyCurrent = new unmanaged Tokens(emptyArray);
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
    var tokenD = {1..0},
      tokenlist: [tokenD] unmanaged Tokens;
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
        currentLine = tokenlist[tokenD.first];
      }
    }

    proc isEmpty() {
      return tokenlist.size == 0;
    }

    proc splitLine(line) {
      var linetokens: [1..0] string;
      var nonEmptyChar: bool = false;

      const doubleQuotes = '(".*?")',           // ""
            singleQuotes = "('.*?')",           // ''
            bracketContents = "(\\[\\w+\\])",   // [_]
            brackets = "(\\[)|(\\])",           // []
            comments = "(\\#)",                 // #
            commas = "(\\,)",                   // ,
            equals = "(\\=)",                   // =
            curly = "(\\{)|(\\})";              // {}

      const pattern = compile('|'.join(doubleQuotes,
                                       singleQuotes,
                                       bracketContents,
                                       brackets,
                                       comments,
                                       commas,
                                       curly,
                                       equals));

      for token in pattern.split(line) {
        var strippedToken = token.strip(" \t");
        if strippedToken.length != 0 {
          if debugTomlReader {
            writeln('Tokenized: ', '(', strippedToken, ')');
          }

          nonEmptyChar = true;
          linetokens.push_back(strippedToken);
        }
      }

      // If no non-empty-chars => token is a blank line
      if(nonEmptyChar == false){
        linetokens.push_back("\n");
      }

      if !linetokens.isEmpty() {
        var tokens = new unmanaged Tokens(linetokens);
        tokenlist.push_back(tokens);
      }
    }


    proc nextLine() {
      if currentLine!.isEmpty() {
        if tokenD.size == 1 {
          return false;
        }
        else {
          var ptrhold = currentLine;
          tokenlist.remove(tokenD.first);
          currentLine = tokenlist[tokenD.first];
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
            if token.length != 0 {
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
    var D: domain(1);
    var A: [D] string;

    proc init(A: [?D] string) {
      this.D = D;
      this.A = A;
    }

    proc skip() {
      var idx = D.first;
      var toke = A(idx);
      A.remove(idx);
    }

    proc next() {
      var idx =  D.first;
      var toke = A(idx);
      A.remove(idx);
      return toke;
    }

    proc addToke(toke: string) {
      A.push_front(toke);
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

    proc readWriteThis(f) {
      f <~> this.A;
    }
  }
}

}
