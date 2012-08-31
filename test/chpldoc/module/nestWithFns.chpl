/* This module has a comment, but uncommented first method */
module OuterMod {

  proc outerFn() {

  }

  /* The second method is commented */
  proc outerCommented() {

  }

  module Inner {
    /* This module has no comment, but it's inner function does! */
    proc innerFn() {

    }
  }
}