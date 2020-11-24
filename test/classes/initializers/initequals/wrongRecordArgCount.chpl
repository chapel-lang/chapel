record R {
  proc init=() { }  // should give an error
  proc init=(a1: R) { }
  proc init=(a1: R, a2: R) { }  // should give an error
}
