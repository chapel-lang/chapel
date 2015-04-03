module Other {
  /* Comment for inline method inside module */
  inline proc inside { }

  inline proc commentless() {

  }

  /* Comment for inline method with a body inside module */
  inline proc body() {
    writeln("I have a body");
  }

  inline proc bodyCommentless () {
    writeln("I have a body!");
  }
  
  /* Comment for method with an argument inside module */
  inline proc hasArg(val: int) {
    
  }

  inline proc hasArgCommentless(val: int) {

  }
  
  /* Comment for method with argument and body inside module */
  inline proc argAndBody(val: int) {
    writeln(val);
  }

  inline proc argAndBodyCommentless(val: int) {
    writeln("arg and a body!");
  }

  /* This method has a return type */
  inline proc hasReturnType(): int {

  }
  
  inline proc hasReturnTypeCommentless(): int {

  }
  
  /* This method has a return type and an argument */
  inline proc hasReturnTypeAndArg(val: int): int {

  }
  
  inline proc hasReturnTypeAndArgCommentless(val: int): int {

  }

  /* This method has both a return type and a body */
  inline proc returnTypeAndBody(): int {
    writeln("Oh man, not again");
  }

  inline proc returnTypeAndBodyCommentless(): int {
    writeln("Wascally Wabbit!");
  }

  /* This method has a return type, an argument, and a function body */
  inline proc allThree(val: int): int {
    writeln("What's this?  A campsite with hot food all ready to eat?");
  }

  inline proc allThreeCommentless(val: int): int {
    writeln("MAKE ME A PIZZA");
  }
}

/* Comment for method1 */
inline proc method1() {

}

inline proc uncommented() {

}

/* Comment for method with a body */
inline proc body() {
  writeln("I have a body");
}

inline proc bodyCommentless() {
  var x = 1 + 3;
}

/* Comment for method with an argument */
inline proc hasArg(val: int) {

}

inline proc hasArgCommentless(val: int) {

}

/* Comment for method with argument and body */
inline proc argAndBody(val: int) {
  writeln(val);
}

inline proc argAndBodyCommentless(val: int) {
  writeln ("I haz cheeseburger");
}

/* This is an rather long comment 
   It goes on for many lines
   But says very little */
inline proc longComment () {

}

/* This method has a return type */
inline proc hasReturnType(): int {

}

inline proc hasReturnTypeCommentless(): int {

}

/* This method has a return type and an argument */
inline proc hasReturnTypeAndArg(val: int): int {

}

inline proc hasReturnTypeAndArgCommentless(val: int): int {

}

/* This method has both a return type and a body */
inline proc returnTypeAndBody(): int {
  writeln("Oh man, not again");
}

inline proc returnTypeAndBodyCommentless(): int {
  writeln("Wascally Wabbit!");
}

/* This method has a return type, an argument, and a function body */
inline proc allThree(val: int): int {
  writeln("What's this?  A campsite with hot food all ready to eat?");
}

inline proc allThreeCommentless(val: int): int {
  writeln("MAKE ME A PIZZA");
}