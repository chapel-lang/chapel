// During the transition period this code forces the compiler
// to emit "XXX is deprecated, replace it with YYY" messages.
// After the transition, by definition, the deprecated pieces will be errors.

def c11() { }  // def->proc

def c12() { return 1; } // def->proc

def c13() { yield "12"; } // def->iter

def c13() { // def->iter
  yield "13a";
  return "13"; // return expr -> { yield expr; return; }
}

_extern def c14(); // def->proc
