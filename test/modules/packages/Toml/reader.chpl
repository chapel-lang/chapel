
/*
Reader module for use in the Parser Class. 
*/

use Regexp;


proc main(args: [] string) {
  var source = new Source(args[1]);
  source.debug();
}

// Returns the next token in the current line without removing it. 
proc top(source) {
  if source.currentLine.D.size < 1 {
    source.newLine();
  }
  return source.currentLine[source.currentLine.D.first];
}

// Returns a boolean or wether or not another line can be read
// Also updates the currentLine if empty
proc readLine(source) {
  return source.nextLine();
}

proc skipNext(source) { 
  source.currentLine.skip();
}

proc skipLine(source) {
  source.skipROL();
}

// retrives the next token in currentline
proc getToken(source) {
  return source.nextToke();
}


/*
The source class reads a file given as an instance variable upon init.
This class helps the Parser read and consume the toml file, and is provided
as an instance variable to the Parser class.
*/
class Source {
  
  var tomlFile: file;
  var tomlStr: string;
  var tokenD = {1..0},
    tokenlist: [tokenD] Tokens;
  var currentLine: Tokens;
  
  
  proc init(tomlFile: file) {
    this.tomlFile = tomlFile;
    this.genTokenlist(tomlFile);
  }
  
  proc init(tomlStr: string) {
    this.tomlStr = tomlStr;
    this.genTokenlist(tomlStr);
  }


  // generates list of Token objects
  proc genTokenlist(input: file) {
    var openFile = open(file, iomode.r);
    for line in openFile.lines() {
      splitLine(line);
    }
    currentLine = tokenlist[tokenD.first];
  }

  proc genTokenlist(input: string) {
    for line in input.split('\n') {
      splitLine(line);
    }
    currentLine = tokenlist[tokenD.first];
  }

  
  proc splitLine(line) {
    var linetokens: [1..0] string;
    const doubleQuotes = '".*?\\[^\\]?"',
          singleQuotes = "'.*?\\[^\\]?'",
          bracketContents = "(\\[\\w+\\])",
          brackets = "(\\[)|(\\])",
          comments = "(\\#)",
          commas = "(\\,)",
          equals = "(\\=)",
          curly = "(\\{)|(\\})";

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
	linetokens.push_back(strippedToken);
      }
    }
      if linetokens.size != 0 {
	var tokens = new Tokens(linetokens);
        tokenlist.push_back(tokens);
      }
  }


  proc newLine() {
    if nextLine() { 
      if currentLine.D.size < 1 {
	tokenlist.remove(tokenD.first);
	currentLine = tokenlist[tokenD.first];
      }
    }
    else {
      writeln("Error: end of file"); //this will be an throw execption
    }
  }
  
  // Reads next line into currentline
  proc nextLine() {
    if currentLine.A.isEmpty() {
      if tokenD.size == 1 {
	return false;
      }
      else {
	tokenlist.remove(tokenD.first);
	currentLine = tokenlist[tokenD.first];
	return true;
      }
    }
    return true; 
  }

  
  // retrives next token in currentLine
  proc nextToke() {
    newLine();
    return currentLine.next();
  }


  proc skipROL() { 
    for token in currentLine {
      currentLine.skip();
    } 
  } 


  proc debug() {
    var lineCounter: int = 1;
    for line in tokenlist {
      write("line: ",lineCounter);
      for token in line {
	write(" token: " + token);
      }
      lineCounter += 1;
      writeln();
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
