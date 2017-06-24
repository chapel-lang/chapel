
/*
Reader module for use in the Parser Class. 
*/

use Regexp;

// Main method
proc main(args: [] string) {
  var source = new Source(args[1]);
  source.genTokenlist();
  source.debug();
  delete source;
}

// Asserts not valid token is found until EOL. accepts comments
proc assertEOL() {}

// Returns the most recent token and removes it 
proc pop(source) {
  //  source.tokenlist
} 

// Returns the next token on the stack ignoring comments 
// if EOL, next line is loaded.
proc top() {}

// skips the next token from the reader. 
proc skip() {}

// Updates line on reader and returns False is EOF is found
proc readLine() {} 

// If nothing left on the stack, read new line.
// Used for multiline arrays
proc readNL() {}



/*
The source class reads a file given as an instance variable upon init.
This class helps the Parser read and consume the toml file, and is provided
as an instance variable to the Parser class.
*/
class Source {

  var file;
  var tokenD = {1..0},
    tokenlist: [tokenD] Tokens;
  var openfile = open(file, iomode.r);


  proc genTokenlist() {
    for line in openfile.lines() {
      splitLine(line);
    }
  }


  proc splitLine(line) {
    const doubleQuotes = '".*?\\[^\\]?"',
          singleQuotes = "'.*?\\[^\\]?'",
          bracketContents = "(\\[.*?\\])",
          brackets = "(\\]|\\[)",
          comments = "(\\#)",
          commas = "(\\,)",
          spaces = "(\\s=)";

    const pattern = compile('|'.join(doubleQuotes,
				     singleQuotes,
				     bracketContents,
				     brackets,
				     comments,
				     commas,
				     spaces));
  for token in pattern.split(line) {
      var linetokens: [1..0] string;
      if token.length != 0  {
	linetokens.push_back(token);
      }
       var tokens = new Tokens(linetokens);
       tokenlist.push_back(tokens);
    }
  }

  proc nextTokens() {
    write(tokenlist[1]);
    tokenlist.remove(1);
} 

  proc debug() {
    for token in tokenlist {
      for toke in token {
	write(" token: " + toke);
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

  proc this(idx) ref {
    return this.A[idx];
  }

  iter these() ref {
    for a in this.A {
      yield a;
    }
  }

  /* This makes the writeln(tokens) == writeln(tokens.A) */
  proc readWriteThis(f) {
    f <~> this.A;
  }
}
