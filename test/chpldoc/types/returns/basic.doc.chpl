module Other {
  /* Comment for method inside module */
  proc inside ():int  { }

  proc insideCommentless (): int { }

  /* Comment for method with a body inside module */
  proc body(): real {
    writeln("I have a body");
  }
  
  proc bodyCommentless(): real {
    writeln("I have a body");
  }

  /* Comment for method with an argument inside module */
  proc hasArg(val: int): real {
    
  }
  
  proc hasArgCommentless(val: int): real {
    
  }

  /* Comment for method with argument and body inside module */
  proc argAndBody(val: int): bool {
    writeln(val);
  }

  proc argAndBodyCommentless(val: int): bool {
    writeln(val);
  }

  /* This proc has an implicit return type */
  proc implicit() {
    
  }
  
  proc implicitCommentless() {
    
  }

  /* This proc has an implicit return type and a return statement */
  proc returnStatement() {
    return 3;
  }

  proc returnCommentless() {
    return 3;
  }
}

/* Comment for method1 */
proc method1(): string {

}

proc method1Commentless(): string {

}

/* Comment for method with a body */
proc body(): complex {
  writeln("I have a body");
}

proc bodyCommentless(): complex {
  writeln("I have a body");
}

/* Comment for method with an argument */
proc hasArg(val: int): uint {

}

proc hasArgCommentless(val: int): uint {

}

/* Comment for method with argument and body */
proc argAndBody(val: int): imag {
  writeln(val);
}

proc argAndBodyCommentless(val: int): imag {
  writeln(val);
}

/* This comment is rather long
   And spans for
   Multiple lines */
proc longComment(): int {

}

/* This proc has an implicit return type */
proc implicit() {

}

proc implicitCommentless() {

}

/* This proc has an implicit return type and a return statement */
proc returnStatement() {
  return 3;
}

proc returnCommentless() {
  return 3;
}