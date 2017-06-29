/* 
TOML library  written in chapel and used in the chapel package manager
*/

use reader;
use Regexp;

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
  source.debug();
  parser.parseLoop();
  prettyPrint(parser.tables);
  delete parser;
}


/* Stolen from arrays primer b/c associative array writeThis sucks.. */
proc prettyPrint(arr : [?dom], f:channel=IO.stdout) {
  f.write("[ ");
  var first = true;
  for k in dom.sorted() {
    if !first {
      f.write(", ", k, " => ", arr[k]);
    } else {
      f.write(k, " => ", arr[k]);
      first = false;
    }
  }
  f.write(" ]");
}




class Parser {

  var source;
  var D: domain(string);
  var tables: [D] Node;
  var curTable = "";

  proc parseLoop() {
    const inBrackets = compile("(\\[.*?\\])");
    const doubleQuotes = '".*?"',
      singleQuotes = "'.*?'",
      digits = "\\d+",
      keys = "\\w+"; 
    const kv = compile('|'.join(doubleQuotes, singleQuotes, digits, keys));

    while(readLine(source)) {
      var token = top(source).strip();
      
      if token == '#' {
	parseComment();
	token = getToken(source).strip(); // gets rid of the actual comment
      }
      else if inBrackets.match(token) {
	parseTable();
      }
      else if kv.match(token) {
	parseAssign();
      }
      else {
	write(getToken(source));
      } 
    }
  }


  proc parseTable() {
    var brackets = compile('\\[|\\]');
    var toke = getToken(source).strip();
    var tablename = brackets.sub('', toke);
    tables[tablename] = new Node();
    curTable = tablename;
  }


  proc parseAssign() {
    var key = getToken(source).strip();
    var equals = getToken(source).strip(); //add expects here
    var value = getToken(source).strip();
    tables[curTable][key] = new Node(value);
  } 
  
  // do nothing
  proc parseComment() {
  }
  
}



class Node {
  // TODO: Other types:  datetime, Array
  var i: int;
  var boo: bool;
  var re: real;
  var s: string;
  var D: domain(string);
  var A: [D] Node;

  // Empty
  proc init() {}

  // String
  proc init(s:string) {
    this.s = s;
  }

  // Node
  proc init(A: [?D] Node) {
    this.D = D;
    this.A = A;
  }

  // Int
  proc init(i: int) {
    this.i = i;
  }

  // Boolean
  proc init(boo: bool) {
    this.boo = boo;
  }

  // Real
  proc init(re: real) {
    this.re = re;
  }

  proc this(idx) ref {
    return A[idx];
  }

  proc writeThis(f) {
    if D.size > 0 {
      prettyPrint(A, f);
    } else {
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






