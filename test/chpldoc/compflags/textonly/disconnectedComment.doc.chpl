/* This comment should be attached to unattached but not reallyCommentless, 
   even when before the defined module */

module unattached {
  proc reallyCommentless() {

  }

  /* This comment should be attached to the method */

  proc commentLess() {

  }

  /* This comment also should be attached 
     even though it is a longer comment */

  proc commentLess2() {

  }

  /* This comment also should be attached, even though
     it is further away from the method */







  proc commentLess3() {

  }

  /* This comment should be attached even with an argument */

  proc hasArg (val: int) {

  }

  /* This comment should be attached, even with a body */

  proc hasBody () {
    writeln("I have a body!");
  }

  /* Attach me, even with a body and an argument */

  proc hasBodyAndArg (val: int) {
    writeln(val);
  }

  /* This comment should not be attached, even after all methods are defined */

}

/* This comment must not be attached, even when outside of the defined module */