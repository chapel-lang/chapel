module Other {
  /* Comment for a function inside module */
  proc inside { }

  /* Comment for a function with a body inside module */
  proc body() {
    writeln("I have a body");
  }
  
  /* Comment for a function with an argument inside module */
  proc hasArg(val: int) {
    
  }
  
  /* Comment for a function with argument and body inside module */
  proc argAndBody(val: int) {
    writeln(val);
  }
}

/* Comment for method1 */
proc method1() {

}

/* Comment for a function with a body */
proc body() {
  writeln("I have a body");
}

/* Comment for a function with an argument */
proc hasArg(val: int) {

}

/* Comment for a function with argument and body */
proc argAndBody(val: int) {
  writeln(val);
}
