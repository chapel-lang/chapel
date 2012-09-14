module Other {
  /* Comment for method inside module */
  export proc inside () { }

  export proc insideCommentless () { }

  /* Comment for method with a body inside module */
  export proc body() {
    writeln("I have a body");
  }
  
  export proc bodyCommentless() {
    writeln("I have a body");
  }

  /* Comment for method with an argument inside module */
  export proc hasArg(val: int) {
    
  }
  
  export proc hasArgCommentless(val: int) {
    
  }

  /* Comment for method with argument and body inside module */
  export proc argAndBody(val: int) {
    writeln(val);
  }

  export proc argAndBodyCommentless(val: int) {
    writeln(val);
  }
  
  /* This method has a return type */
  export proc hasReturnType(): int {

  }
  
  export proc hasReturnTypeCommentless(): int {

  }
  
  /* This method has a return type and an argument */
  export proc hasReturnTypeAndArg(val: int): int {

  }
  
  export proc hasReturnTypeAndArgCommentless(val: int): int {

  }

  /* This method has both a return type and a body */
  export proc returnTypeAndBody(): int {
    writeln("Oh man, not again");
  }

  export proc returnTypeAndBodyCommentless(): int {
    writeln("Wascally Wabbit!");
  }

  /* This method has a return type, an argument, and a function body */
  export proc allThree(val: int): int {
    writeln("What's this?  A campsite with hot food all ready to eat?");
  }

  export proc allThreeCommentless(val: int): int {
    writeln("MAKE ME A PIZZA");
  }
}

/* Comment for method1 */
export proc method1() {

}

export proc method1Commentless() {

}

/* Comment for method with a body */
export proc body2() {
  writeln("I have a body");
}

export proc bodyCommentless2() {
  writeln("I have a body");
}

/* Comment for method with an argument */
export proc hasArg2(val: int) {

}

export proc hasArgCommentless2(val: int) {

}

/* Comment for method with argument and body */
export proc argAndBody2(val: int) {
  writeln(val);
}

export proc argAndBodyCommentless2(val: int) {
  writeln(val);
}

/* This is a rather long comment
   It spans for multiple lines
   Trying to test most output */
export proc longComment() {

}

/* This method has a return type */
export proc hasReturnType2(): int {

}

export proc hasReturnTypeCommentless2(): int {

}

/* This method has a return type and an argument */
export proc hasReturnTypeAndArg2(val: int): int {

}

export proc hasReturnTypeAndArgCommentless2(val: int): int {

}

/* This method has both a return type and a body */
export proc returnTypeAndBody2(): int {
  writeln("Oh man, not again");
}

export proc returnTypeAndBodyCommentless2(): int {
  writeln("Wascally Wabbit!");
}

/* This method has a return type, an argument, and a function body */
export proc allThree2(val: int): int {
  writeln("What's this?  A campsite with hot food all ready to eat?");
}

export proc allThreeCommentless2(val: int): int {
  writeln("MAKE ME A PIZZA");
}