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
  //  source.debug();
  while(readLine(source)) {
    var token = getToken(source).strip();
    if token == '#' {
      parser.parseComment();
    }
    else if token == '[' { //change this to regmatch i think
      write("Array: " + token);
    }
    else if token == '=' {
      write('Assignment: ' + token);
    } 
    else if token == '\n' { 
      writeln();
    }
    else {
      writeln(token);
    } 
  }
} 





class Parser {

  var source;
  var rDom = {1..0},
    resultlist: [rDom] Results;

  proc loadKeyValue() { }
  
  proc parseType() { 
    var reArray = compile("(\\[.*?\\])");
    var reString = compile('".*?\\[^\\]?"'|"'.*?\\[^\\]?'");
  } 

  proc parseLoop() {}

  proc parseComment() {
  }

}





/* Array wrapper */
class Results {
  var R: domain(1);
  var rArr: [R] string;
  
  proc init(rArr: [?R] string) {
    this.R = R;
    this.rArr = rArr;
  }
    
  proc this(idx) ref {
    return this.rArr[idx];
  }
  
  iter these() ref {
    for r in this.rArr {
      yield r;
    }
  }
  
  /* This makes the writeln(tokens) == writeln(tokens.A) */
  proc readWriteThis(f) {
    f <~> this.rArr;
  }
}

