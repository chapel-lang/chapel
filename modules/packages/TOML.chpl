/*
 * Copyright 2004-2018 Cray Inc.
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
*/
pragma "error mode fatal"
module TOML {


use TomlParser;
use TomlReader;


/* Receives a TOML file as a parameter and outputs an associative
array */
proc parseToml(input: file) : Toml {
  var tomlStr: string;
  var tomlFile = input.reader();
  tomlFile.readstring(tomlStr);
  tomlFile.close();
  return parseToml(tomlStr);
}

/* Receives a channel to a TOML file as a parameter and outputs an associative
array. */
proc parseToml(input: channel) : Toml {
  var tomlStr: string;
  input.readstring(tomlStr);
  return parseToml(tomlStr);
}

 /* Receives a string of TOML format as a parameter and outputs an associative
array. */
proc parseToml(input: string) : Toml {
  var D: domain(string);
  var table: [D] Toml;
  var rootTable = new Toml(table);
  const source = new Source(input);
  const parser = new Parser(source, rootTable);
  const tomlData = parser.parseLoop();
  delete parser;
  delete source;
  return tomlData;
}


/*
Parser module with the Toml class for the Chapel TOML library.
*/
module TomlParser {

  use Regexp;
  use DateTime;


  /* Prints a line by line output of parsing process */
  config const debugTomlParser = false;

  /* Number of spaces in an indentation for JSON output */
  pragma "no doc"
  const tabSpace = 4;

  pragma "no doc"
  class Parser {

    var source: Source;
    var rootTable: Toml;
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
      ints = compile("(\\d+|\\+\\d+|\\-\\d+)"),
      inBrackets = compile("(\\[.*?\\])"),
      corner = compile("(\\[.+\\])"),
      brackets = compile('\\[|\\]'),
      whitespace = compile("\\s"),
      comment = compile("(\\#)"),
      comma = compile("(\\,)");

    var debugCounter = 1;

    proc parseLoop() : Toml {

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
          else {
            halt("Unexpected token ->", getToken(source));
          }
          if debugTomlParser {
            debugCounter += 1;
            debugPrint();
          }
        }
      }
      return rootTable;
    }

    proc parseTable() {
      var toke = getToken(source);
      var tablename = brackets.sub('', toke);
      var tblD: domain(string);
      var tbl: [tblD] Toml;
      if !rootTable.pathExists(tablename) {
        rootTable[tablename] = tbl;
      }
      curTable = tablename;
    }

    proc parseSubTbl() {
      skipNext(source);
      var tblname = getToken(source);
      skipNext(source);
      var tblD: domain(string);
      var tbl: [tblD] Toml;
      var (tblPath, tblLeaf) = splitTblPath(tblname);
      if !rootTable.pathExists(tblPath) then makePath(tblPath);
      rootTable[tblPath][tblLeaf] = tbl;
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
          var tbl: [tblD] Toml;
          rootTable[parent] = tbl;
          first = false;
        }
        else {
          var tblD: domain(string);
          var tbl: [tblD] Toml;
          var grandParent = '.'.join(path[..firstIn+i]);
          rootTable[grandParent][parent] = tbl;
          i+=1;
        }
      }
    }

    proc parseInlineTbl(key: string) {
      var tblname: string;
      var tblD: domain(string);
      var tbl: [tblD] Toml;
      if curTable.isEmptyString() {
        tblname = key;
        rootTable[key] = tbl;
      }
      else {
        tblname = '.'.join(curTable, key);
        var (tblPath, tblLeaf) = splitTblPath(tblname);
        rootTable[tblPath][tblLeaf] = tbl;
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
      var key = getToken(source);
      var equals = getToken(source);
      if top(source) == '{' {
        skipNext(source);
        parseInlineTbl(key);
      }
      else {
        var value = parseValue();
        if curTable.isEmptyString() then rootTable[key] = value;
        else rootTable[curTable][key] = value;
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
    proc parseValue(): Toml {
      var val = top(source);
      // Array
      if val == '['  {
        skipNext(source);
        var Dom: domain(1);
        var array: [Dom] Toml;
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
        var tomlArray: Toml = array;
        return tomlArray;
      }
      // Strings (includes multi-line)
      else if Str.match(val) {
        var toStr: string;
        if val.startsWith('"""') {
          toStr += getToken(source).strip('"""', true, false);
          while toStr.endsWith('"""') == false {
            toStr += " " + getToken(source);
          }
          var mlString: Toml = toStr.strip('"""');
          return mlString;
        }
        else if val.startsWith("'''") {
          toStr += getToken(source).strip("'''", true, false);
          while toStr.endsWith("'''") == false {
            toStr += " " + getToken(source);
          }
          var mlString: Toml = toStr.strip("'''");
          return mlString;
        }
        else {
          toStr = getToken(source).strip('"').strip("'");
          var tomlStr: Toml = toStr;
          return tomlStr;
        }
      }
      // DateTime
      else if dt.match(val) {
        var date = datetime.strptime(getToken(source), "%Y-%m-%dT%H:%M:%SZ");
        var Datetime: Toml = date;
        return Datetime;
      }
      // Real
      else if realNum.match(val) {
        var token = getToken(source);
        var toReal = token: real;
        var realToml: Toml = toReal;
        return realToml;
      }
      // Int
      else if ints.match(val) {
        var token = getToken(source);
        var toInt = token: int;
        var intToml: Toml = toInt;
        return intToml;
      }
      // Boolean
      else if val == "true" || val ==  "false" {
        var token = getToken(source);
        var toBool = token: bool;
        var boolToml: Toml = toBool;
        return boolToml;
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
        halt("Unexpected Token: ", "'", val, "'");
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
   fieldDate };
 use fieldtag;

 pragma "no doc"
 proc =(ref t: Toml, s: string) {
   if t == nil {
     t = new Toml(s);
   } else {
     t.tag = fieldString;
     t.s = s;
   }
 }

 pragma "no doc"
 proc =(ref t: Toml, i: int) {
   if t == nil {
     t = new Toml(i);
   } else {
     t.tag = fieldInt;
     t.i = i;
   }
 }

 pragma "no doc"
 proc =(ref t: Toml, b: bool) {
   if t == nil {
     t = new Toml(b);
   } else {
     t.tag = fieldBool;
     t.boo = b;
   }
 }

 pragma "no doc"
 proc =(ref t: Toml, r: real) {
   if t == nil {
     t = new Toml(r);
   } else {
     t.tag = fieldReal;
     t.re = r;
   }
 }

 pragma "no doc"
 proc =(ref t: Toml, dt: datetime) {
   if t == nil {
     t = new Toml(dt);
   } else {
     t.tag = fieldDate;
     t.dt = dt;
   }
 }

 pragma "no doc"
 proc =(ref t: Toml, A: [?D] Toml) where isAssociativeDom(D) {
   if t == nil {
     t = new Toml(A);
   } else {
     t.tag = fieldToml;
     t.D = D;
     t.A = A;
   }
 }

 pragma "no doc"
 proc =(ref t: Toml, arr: [?dom] Toml) where !isAssociativeDom(dom){
   if t == nil {
     t = new Toml(arr);
   } else {
     t.tag = fieldArr;
     t.dom = dom;
     t.arr = arr;
   }
 }


/*
Class to hold various types parsed from input
Used to recursively hold tables and respective values
*/
  class Toml {

    pragma "no doc"
    var i: int,
      boo: bool,
      re: real,
      s: string,
      dt: datetime,
      dom: domain(1),
      arr: [dom] Toml,
      D: domain(string),
      A: [D] Toml,
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
    proc init(A: [?D] Toml) where isAssociativeDom(D) {
      this.D = D;
      this.A = A;
      this.tag = fieldToml;
    }

    // Datetime
    proc init(dt: datetime) {
      this.dt = dt;
      this.tag = fieldDate;
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
    proc init(arr: [?dom] Toml) where isAssociativeDom(dom) == false  {
      this.dom = dom;
      this.arr = arr;
      this.tag = fieldArr;
    }

    // Clone
    proc init(root: Toml) {
      this.boo = root.boo;
      this.i = root.i;
      this.re = root.re;
      this.dom = root.dom;
      for idx in root.dom do this.arr[idx] = new Toml(root.arr[idx]);
      this.dt = root.dt;
      this.s = root.s;
      this.D = root.D;
      for idx in root.D do this.A[idx] = new Toml(root.A[idx]);
      this.tag = root.tag;
    }


    /* Returns the index of the table path given as a parameter */
    proc this(tbl: string) ref : Toml {
      const indx = tbl.split('.');
      var top = indx.domain.first;
      if indx.size < 2 {
        return this.A[tbl];
      }
      else {
        var next = '.'.join(indx[top+1..]);
        if this.A.domain.member(indx[top]) {
          return this.A[indx[top]][next];
        }
        else {
          halt("Error - no index found for ", tbl);
        }
      }
    }

    pragma "no doc"
    /* Returns true if table path exists in rootTable */
    proc pathExists(tblpath: string) : bool {
      var path = tblpath.split('.');
      var top = path.domain.first;
      if path.size < 2 {
        if this.A.domain.member(tblpath) == false {
          return false;
        }
        else {
          return true;
        }
      }
      else {
        var next = '.'.join(path[top+1..]);
        if this.A.domain.member(path[top]) {
          return this.A[path[top]].pathExists(next);
        }
        else {
          return false;
        }
      }
    }

    /* Write a Table to channel f in TOML format */
    proc writeThis(f) {
      writeTOML(f);
    }

    /* Write a Table to channel f in TOML format */
    proc writeTOML(f) {
      var flatDom: domain(string);
      var flat: [flatDom] Toml;
      this.flatten(flat);       // Flattens containing Toml
      printValues(f, this);     // Prints key values in containing Toml
      printTables(flat, f);       // Prints tables in containing Toml
    }

    /* Write a Table to channel f in JSON format */
    proc writeJSON(f) {
      var flatDom: domain(string);
      var flat: [flatDom] Toml;
      this.flatten(flat);           // Flattens containing Toml

      var indent=0;

      f.writeln('{');
      indent += tabSpace;

      // Prints key values in containing Toml
      printValuesJSON(f, this, indent=indent);

      if flatDom.member('root') {
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


    pragma "no doc"
    /* Flatten tables into flat associative array for writing */
    proc flatten(flat: [?d] Toml, rootKey = '') : flat.type {
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
    proc printTables(flat: [?d] Toml, f:channel) {
      if d.member('root') {
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
    proc printValues(f: channel, v: Toml) {
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
            halt("Keys have to have a value");
          }
          when fieldDate {
            f.write(key, ' = ', toString(value));
          }
          otherwise {
            f.write("not yet supported");
          }
        }
        f.writeln();
      }
      f.writeln();
    }

    pragma "no doc"
    /* Send values from table to toString for writing  */
    proc printValuesJSON(f: channel, v: Toml, in indent=0) {
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
            halt("Keys have to have a value");
          }
          when fieldDate {
            f.writef('%s"%s": {"type": "%s", "value": "%s"}', ' '*indent, key, value.tomlType, toString(value));
          }
          otherwise {
            f.write("not yet supported");
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
    proc toString(val: Toml) : string {
      select val.tag {
        when fieldBool do return val.boo;
        when fieldInt do return val.i;
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
        when fieldReal do return val.re;
        when fieldString do return ('"' + val.s + '"');
        when fieldEmpty do return ""; // empty
        when fieldDate do return val.dt.isoformat();
        otherwise {
          halt("Error in printing '", val, "'", " with tag '", val.tag, "'");
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
    proc tomlType: string {
      select this.tag {
        when fieldBool do return 'bool';
        when fieldInt do return 'integer';
        when fieldArr do return 'array';
        when fieldReal do return 'float';
        when fieldString do return 'string';
        when fieldEmpty do return 'empty';
        when fieldDate do return 'datetime';
        when fieldToml do return 'toml';
        otherwise {
          halt("Unknown type: '", this.tag, "'");
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

 use Regexp;

 config const debugTomlReader = false;

  /* Returns the next token in the current line without removing it */
  proc top(source) {
    if source.nextLine() {
      return source.currentLine[source.currentLine.D.first];
    }
    else {
      halt("Reached end of file");
    }
  }

  /* Returns a boolean or whether or not another line can be read
  /  Also updates the currentLine if empty */
  proc readLine(source) {
    return source.nextLine();
  }

  proc skipNext(source) {
    source.currentLine.skip();
  }

  proc addToken(source, tokensToAdd: [?dom] string) {
    for toke in tokensToAdd {
      source.currentLine.addToke(toke);
    }
  }

  proc skipLine(source) {
    var emptyArray: [1..0] string;
    var emptyCurrent = new Tokens(emptyArray);
    var ptrhold = source.currentLine;
    source.currentLine = emptyCurrent;
    var readNextLine = readLine(source);
    delete ptrhold;
  }

  /* retrieves the next token in currentLine */
  proc getToken(source) {
    return source.nextToke();
  }


  class Source {

    var tomlStr: string;
    var tokenD = {1..0},
      tokenlist: [tokenD] Tokens;
    var currentLine: Tokens;


    proc init(tomlStr: string) {
     this.tomlStr = tomlStr;
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
        var strippedToken = token.strip();
        if strippedToken.length != 0  {
          if debugTomlReader {
            writeln('Tokenized: ', '(', strippedToken, ')');
          }
          linetokens.push_back(strippedToken);}
      }
      if !linetokens.isEmpty() {
        var tokens = new Tokens(linetokens);
        tokenlist.push_back(tokens);
      }
    }


    proc nextLine() {
      if currentLine.isEmpty() {
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
    proc nextToke() {
      if nextLine() {
        return currentLine.next();
      }
      else {
        halt("Reached end of file");
      }
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
