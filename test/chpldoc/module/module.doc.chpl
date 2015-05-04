/* This module has a comment */
module M {

  proc commentless() {

  }
}

/* So does this module */
module N {


  proc hasArgCommentless (val: int) {

  }
}

/* And this one */
module O {

  proc hasBodyCommentless () {
    writeln("I have a body but no comment");
  }
}

/* What?  Even this one! */
module P {

  proc hasBodyAndArgCommentless(val: int){
    writeln(val);
  }

  proc useless() {

  }
}

module Q {
  /* Sadly, this method's module does not */
  proc main() {
    writeln("Oh no! Something printed!");
  }
}

module R {
  /* Nor this method's module */
  proc furthermore() {

  }
}

/* This comment should not be picked up by anything */