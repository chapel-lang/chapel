
/* 
TOML library  written in chapel and used in the chapel package manager
*/

use reader;
use Regexp;
use DateTime;

/*
parseToml takes in a channel to a .toml file as an argument and outputs an 
associative array based on the contents of the .toml file. This array
can then be passed to a writeToml method to write to a .toml file.

calls
  - source = Source(input)
  - parser = Parser(source)
  - return Parser.Dict
*/ 
proc parseToml(input: channel) {}


/*
parseTomlStr takes in a string in toml format to be parse into an associative
array. The array can be printed using the writeToml procedure. 
*/
proc parseTomlStr(input: string) {}


/*
Takes in a channel to an output file provided as an argument. The format
for writing into the file is TOML.  
*/
proc writeToml(output: channel) {}



// Main method
proc main(args: [] string) {
  var source = new Source(args[1]);
  var parser = new Parser(source);
  ready(source);
  parser.parseLoop();
  delete parser;
}


/* global tables print method for debugging */
proc prettyPrint(arr : [?dom], f:channel=IO.stdout) {
  var first = true;
  f.writeln();
  for k in dom {
    if !first {
      f.writeln();
      f.writeln('===New Table===');
      f.writeln();
      f.writeln("[",k,"]", " fields:  ");
      f.write(arr[k]); 
    } else {
      f.writeln("[",k,"]", " fields:  ");
      f.write(arr[k]); 
      first = false;
    }
  }
}




config const DEBUG: bool = false;

class Parser {

  var source;
  var D: domain(string);
  var tables: [D] Node;
  var curTable = "";
  

  
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
      else if kv.match(token) {
	parseAssign();
      }
      else {
	write("Exception: ", getToken(source));
      }
      if DEBUG {
	writeln();
	writeln("========================= Debug Info  ==========================");
	source.debug();
	writeln();
	prettyPrint(tables);
	writeln();
	writeln("================================================================");
      }
    }
  }


  proc parseTable() {
    var toke = getToken(source);
    var tablename = brackets.sub('', toke);
    tables[tablename] = new Node();
    curTable = tablename;
  }



  proc parseValue(): Node {
    var val = top(source);

    // Array
    if val == '['  {
      source.currentLine.skip();     // skip '['
      var nodeDom: domain(1);
      var array: [nodeDom] Node;
      while top(source) != ']' {
	if comma.match(top(source)) {
	  source.currentLine.skip();
	}
	else {
	  var toParse = parseValue();
	  array.push_back(toParse);
	}
      }
      source.currentLine.skip();
      var nodeArray = new Node(array);
      return nodeArray;
    }
    // String
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
      var toReal: real;
      var token = getToken(source);
      toReal = token: real;
      var realNode = new Node(toReal);
      return realNode;
    }
    // Int
    else if ints.match(val) {
      var toInt: int;
      var token = getToken(source);
      toInt = token: int;
      var intNode = new Node(toInt);
      return intNode;
    } 
    // Boolean
    else if val == "true" || val ==  "false" {
      var toBool: bool;
      var token = getToken(source);
      toBool = token: bool;
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
	
      
  proc parseAssign() {
    var key = getToken(source);
    var equals = getToken(source); // Add expects?
    var value = parseValue();
    tables[curTable][key] = value;
  } 
  
  // Skip the line with the comment 
  proc parseComment() {
    skipLine(source);
  }
}




class Node {
  // TODO: Other types:  datetime, fix array
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
  proc init(arr: [?dom] Node)  {
    this.dom = dom;
    this.arr = arr;
    tag = fieldArr;
  }

  proc this(idx) ref {
    return A[idx];
  }


  proc writeThis(f) {

    if tag == fieldArr {
      var first = true;
      f.write('[');
      for k in dom {
	if first { 
	  f.write(arr[k], ', ');
	  first = false;
	}
	else if arr[k] == arr[dom.last] {
	  f.write(arr[k], ']');
	}
	else {
	  f.write(arr[k], ', ');
	}
      }
    } 
    else if tag == fieldNode {
      for k in D {
	f.writeln(k ,' => ', A[k]);}
    }
    else if tag == fieldInt {
      f.write(i);
    }
    else if tag == fieldBool {
      f.write(boo);
    }
    else if tag == fieldString {
      f.write(s);
    }
    else if tag == fieldDate {
      f.write(dt.isoformat());
    }
    else if tag == fieldReal {
      f.write(re);
    }
    else if tag == fieldEmpty {
      f.write("empty");
    }
    else {
      f.write(s);
    }
  }



  /* Don't forget to free your memory! */
  proc deinit() {
    for a in A {
      delete a;
    }
  }
}






