record R {
}

proc R.init=() { }  // should give an error
proc R.init=(a1: R) { }
proc R.init=(a1: R, a2: R) { }  // should give an error
