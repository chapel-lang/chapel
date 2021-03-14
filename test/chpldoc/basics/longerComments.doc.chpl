module Other {
  /* This is a comment for a function inside of
     a module */
  proc inside { }

  /* This is a comment for a function with a body 
     inside of a module */
  proc body() {
    writeln("I have a body");
  }
  
  /* This is a comment for a function with an 
     argument inside of a module */
  proc hasArg(val: int) {
    
  }
  
  /* This is a comment for a function with an 
     argument and body inside of a module */
  proc argAndBody(val: int) {
    writeln(val);
  }
}

/* This is a comment for method1, which lies
   outside of a defined module */
proc method1() {

}

/* This is a comment for a function with a body 
   which lies outside of a defined module */
proc body() {
  writeln("I have a body");
}

/* This is a comment for a function with an 
   argument which lies outside of a defined
   module */
proc hasArg(val: int) {

}

/* This is a comment for a function with an
   argument and body which lies outside of a
   defined module */
proc argAndBody(val: int) {
  writeln(val);
}
