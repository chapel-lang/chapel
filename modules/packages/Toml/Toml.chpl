/* 
Chapel's Library for `Tom's Obvious, Minimal Language (TOML)
              <https://github.com/toml-lang/toml>`_. 
This module provides support for parsing and writing toml files.
*/

/*
Receives a channel to a TOML file as a parameter and outputs an associative
array Node.
*/
proc parseToml(input: channel) : Node {}

/*
Receives a string of TOML format as a parameter and outputs an associative
array Node. 
*/
proc parseToml(input: string) : Node {}



use reader;
use Regexp;
use DateTime;


// Main method
proc main(args: [] string) {
  const source = new Source(args[1]);
  const parser = new Parser(source);
  ready(source);
  parser.parseLoop();
  writeln(parser.tables);
  delete parser;
  delete source;
}


config const DEBUG: bool = false;

class Parser {

  var source;
  var D: domain(string);
  var table: [D] Node;
  var tables = new Node(table);
  var curTable: string;
  var root: Node;
    
  const doubleQuotes = '".*?"',
    singleQuotes = "'.*?'",
    digit = "\\d+",
    keys = "^\\w+";
  const Str = compile(doubleQuotes + '|' + singleQuotes),
    kv = compile('|'.join(doubleQuotes, singleQuotes, digit, keys)),
    dt = compile('^\\d{4}-\\d{2}-\\d{2}[ T]\\d{2}:\\d{2}:\\d{2}$'),
    realNum = compile("\\+\\d*\\.\\d+|\\-\\d*\\.\\d+|\\d*\\.\\d+"),
    ints = compile("(\\d+|\\+\\d+|\\-\\d+)"),
    inBrackets = compile("^(\\[.*?\\])"),
    brackets = compile('\\[|\\]'),
    whitespace = compile("\\s"),
    comma = compile("(\\,)");

    

  proc parseLoop() {

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
      if DEBUG {
	writeln();
	writeln("========================= Debug Info  ==========================");
	source.debug();
	writeln();
	writeln(tables);
	writeln();
        writeln("================================================================");
      }
    }
  }

  proc parseTable() {
    var toke = getToken(source);
    var tablename = brackets.sub('', toke);
    var tblD: domain(string);
    var tbl: [tblD] Node;
    root = new Node(tbl);
    tables[tablename] = root;
    curTable = tablename;
  }

  proc parseSubTbl() {
    skipNext(source);
    var tblname = getToken(source); //throw error if empty
    skipNext(source);
    var tblD: domain(string);
    var tbl: [tblD] Node;
    var (tblPath, tblLeaf) = splitTblPath(tblname);
    tables.getIdx(tblPath)[tblLeaf] = new Node(tbl);
    curTable = tblname;
  }
  
  proc parseInlineTbl(key: string) {
    var tblname = '.'.join(curTable, key);
    var tblD: domain(string);
    var tbl: [tblD] Node;
    var (tblPath, tblLeaf) = splitTblPath(tblname);
    tables.getIdx(tblPath)[tblLeaf] = new Node(tbl);
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
      tables.getIdx(curTable)[key] = value;
    }
  }
  
  // Skip the line with the comment 
  proc parseComment() {
    skipLine(source);
  }  

  // returns leaf of embedded table
  proc splitTblPath(s: string) {
    var A = s.split('.');
    var fIdx = A.domain.first;
    var leaf = A[A.domain.last];
    var path = '.'.join(A[..A.domain.last-1]);
    if A.size == 1 then path = A[fIdx];
    return (path, leaf);
  }

  // [servers.alpha.echo] => [servers, alpha, echo]
  proc splitName(s: string) {
    var A = s.split('.');
    return A;
  }
  
  proc parseValue(): Node {
    var val = top(source);
    // Array
    if val == '['  {
      skipNext(source);
      var nodeDom: domain(1);
      var array: [nodeDom] Node;
      while top(source) != ']' {
	if comma.match(top(source)) {
	  skipNext(source);
	}
	else {
	  var toParse = parseValue();
	  array.push_back(toParse);
	}
      }
      skipNext(source);
      var nodeArray = new Node(array);
      return nodeArray;
    }
    // Strings (includes multi-line) 
    else if Str.match(val) {
      if val.startsWith('"""') || val.startsWith("'''") {
	var toStr: string;
	while toStr.endsWith('"""') == false && toStr.endsWith("'''") == false {
	  toStr += " " + getToken(source);
	}
        var mlStringNode = new Node(toStr);
	return mlStringNode;
      }
      else {
	var stringNode = new Node(getToken(source));
	return stringNode;
      }
    }
    // DateTime
    else if dt.match(val) {
      var date = datetime.strptime(getToken(source), "%Y-%m-%dT%H:%M:%SZ");
      var Datetime = new Node(date);
      return Datetime;
    }
    // Real
    else if realNum.match(val) {
      var token = getToken(source);
      var toReal = token: real;
      var realNode = new Node(toReal);
      return realNode;
    }
    // Int
    else if ints.match(val) {
      var token = getToken(source);
      var toInt = token: int;
      var intNode = new Node(toInt);
      return intNode;
    } 
    // Boolean
    else if val == "true" || val ==  "false" {
      var token = getToken(source);
      var toBool = token: bool;
      var boolNode = new Node(toBool);
      return boolNode;
    }
    // This will eventually be an error
    else {
      var token = getToken(source);
      var unhandeled = new Node(token);
      writeln("Unhandeled Type: ", token);
      return unhandeled; 
    }
  }
}


class Node {
  var i: int;
  var boo: bool;
  var re: real;
  var s: string;
  var dt: datetime;
  var dom: domain(1);
  var arr: [dom] Node;
  var D: domain(string);
  var A: [D] Node;
    
  const fieldBool = 1,
    fieldInt = 2,
    fieldArr = 3,
    fieldNode = 4,
    fieldReal = 5,
    fieldString = 6,
    fieldEmpty = 7,
    fieldDate = 8;
  var tag: int = fieldEmpty;  


  // Empty
  proc init() {
    tag = fieldNode;
  }

  // String
  proc init(s:string) {
    this.s = s;
    tag = fieldString;
  }

  // Node
  proc init(A: [?D] Node) where isAssociativeDom(D) {
    this.D = D;
    this.A = A;
    tag = fieldNode;
  }
  // Datetime
  proc init(dt: datetime) {
    this.dt = dt;
    tag = fieldDate;
  }

  // Int
  proc init(i: int) {
    this.i = i;
    tag = fieldInt;
  }

  // Boolean
  proc init(boo: bool) {
    this.boo = boo;
    tag = fieldBool;
  }

  // Real
  proc init(re: real) {
    this.re = re;
    tag = fieldReal;
  }

  // Array
  proc init(arr: [?dom] Node) where isAssociativeDom(dom) == false  {
    this.dom = dom;
    this.arr = arr;
    tag = fieldArr;
  }

  proc this(idx: string) ref {
    return A[idx];
  }

  proc getIdx(tbl: string) ref : Node {
    var indx = tbl.split('.');
    var top = indx.domain.first;
    if indx.size < 2 {
     if this.A.domain.member(tbl) == false {
       halt("Error in getIdx 1");
     }
      else {
        return this.A[tbl];
      }
    } 
    else {
      var next = '.'.join(indx[top+1..]);
      if this.A.domain.member(indx[top]) {
        return this.A[indx[top]].getIdx(next);
      }
      else {
        halt("Error in getIdx 2");
      }
    }
  }
  
  
  proc writeThis(f) {
    var flatDom: domain(string);
    var flat: [flatDom] Node;
    this.flatten(flat);
    printHelp(flat, f);
  }
  

  proc flatten(flat: [?d] Node, rootKey = '') : flat.type { 
    for (k, v) in zip(this.D, this.A) {
      if v.tag == 4 { 
        var fullKey = k;
        if rootKey != '' then fullKey = '.'.join(rootKey, k);
        flat[fullKey] = v;
        v.flatten(flat, fullKey);
      }
    }
    return flat;
  }

  proc printHelp(flat: [?d] Node, f:channel) {
    for (k, v) in zip(d, flat) {
      f.writeln('[', k, ']');
      printValues(f, v);
    }
  }
  
  proc printValues(f: channel, v) {
    for (key, value) in zip(v.D, v.A) {
      select value.tag {
        when 4 do write(""); // Table
        when 1 {
          f.writeln(key, ' = ', toString(value));
        }
        when 2 {
          f.writeln(key, ' = ', toString(value));
        }
        when 3 {
          var first = true;
          f.write(key, ' = ');
          f.write('[');
          for k in value.arr {
            if first {
              f.write(toString(k), ', ');
              first = false;
            }
            else if k == value.arr[value.arr.domain.last] {
              f.writeln(toString(k), ']');
            }
            else {
              f.write(toString(k), ', ');
            }
          }
        }
        when 5 {
          f.writeln(key, ' = ', toString(value));
        }
        when 6 {
          f.writeln(key, ' = ', toString(value));
        }
        when 7 {
          halt("Keys have to have a value");
        }
        when 8 {
          f.writeln(key, ' = ', toString(value));
        }
        otherwise { 
          f.write("not yet supported");
        }
        } 
    }
    f.writeln();
  }
  
  proc toString(val: Node) : string { 
    select val.tag {
      when 1 do return val.boo;
      when 2 do return val.i;
      when 3 {
        var first = true;
        var final: string;
        for k in val.arr {
          if first {
            final += '[' + toString(k) + ', ';
            first = false;
          }
          else if k == val.arr[val.arr.domain.last] {
            final += toString(k) + ']';
          }
          else {
            final += toString(k) + ', ';
          }
        }
        return final;
      }
      when 5 do return val.re;
      when 6 do return val.s;
      when 7 do return ""; // empty
      when 8 do return val.dt.isoformat();
      otherwise {
        return val;
        writeln("something weird happened with", val);
      }
      }
  }
  
    
  /* Don't forget to free your memory! */
  proc deinit() {
    for a in A {
      delete a;
    }
  }
}
