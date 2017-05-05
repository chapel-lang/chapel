module Other {
  /* Comment for method inside module */
  proc inside { }

  /* Comment for method with a body inside module */
  proc body() {
    writeln("I have a body");
  }
  
  /* Comment for method with an argument inside module */
  proc hasArg(val: int) {
    
  }
  
  /* Comment for method with argument and body inside module */
  proc argAndBody(val: int) {
    writeln(val);
  }
}

/* Comment for method1 */
proc method1() {

}

/* Comment for method with a body */
proc body() {
  writeln("I have a body");
}

/* Comment for method with an argument */
proc hasArg(val: int) {

}

/* Comment for method with argument and body */
proc argAndBody(val: int) {
  writeln(val);
}

