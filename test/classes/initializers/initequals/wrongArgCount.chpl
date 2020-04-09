class C {
  proc init=() { }  // should give an error
  proc init=(a1: C) { }
  proc init=(a1: C, a1: C) { }  // should give an error
}
