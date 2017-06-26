
/*
Reader module for use in the Parser Class. 
*/

use Regexp;

// Main method
proc main(args: [] string) {
  var source = new Source(args[1]);
  ready(source);
  writeln(getToken(source));
  writeln(getToken(source));
  delete source;
}



// Returns the next token in the current line without removing it. 
proc top(source) {
  var latest = source.currentLine.A[source.currentLine.D.first];
  return latest;
}

// skips the next token from the reader. 
proc skipToken(source) {
  source.currentLine.skip();
}

// Updates currentline in the source class.
proc readLine(source) {
  source.nextLine();
}

// retrives the next token in currentline
proc getToken(source) {
  return source.nextToke();
}

proc ready(source) { 
  if source.ready == false { 
    source.genTokenlist();
    source.nextLine();
    source.ready = true;
  }
}// add throw error here




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
  var currentLine: Tokens;
  var ready = false;
  
  // generates list of Token objects(array of strings)
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
          equals = "(\\s=)";

    const pattern = compile('|'.join(doubleQuotes,
				     singleQuotes,
				     bracketContents,
				     brackets,
				     comments,
				     commas,
				     equals));
  for token in pattern.split(line) {
      var linetokens: [1..0] string;
      if token.length != 0  {
	linetokens.push_back(token);
      }
       var tokens = new Tokens(linetokens);
       tokenlist.push_back(tokens);
    }
  }


  // Reads next line into currentline
  proc nextLine() {
    var nextTokens = tokenlist[tokenD.first];
    if nextTokens.A.isEmpty() {
      tokenlist.remove(tokenD.first);
      nextLine();  // EOF issue here need try/catch
    }
    else {
      currentLine = tokenlist[tokenD.first];
      tokenlist.remove(tokenD.first);
    }
  }
  
  // retrives next token in currentLine
  proc nextToke() {
    var nexttoken = currentLine.next();
    if nexttoken == '\n' || nexttoken == '#' {
      nextLine();
      nexttoken = currentLine.next();
    }
    return nexttoken;
  }

  proc debug() {
    for line in tokenlist {
      for token in line {
	write(" token: " + token);
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
    var idx =  D.first;
    var toke = A(idx);
    A.remove(idx);
  }

  proc next() { 
    var idx =  D.first;
    var toke = A(idx);
    A.remove(idx);
    return toke;
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
