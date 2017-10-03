/* This module has a comment, but uncommented first function */
module OuterMod {

  proc outerFn() {

  }

  /* The second function is commented */
  proc outerCommented() {

  }

  module Inner {
    /* This module has no comment, but it's inner function does! */
    proc innerFn() {

    }
  }
}