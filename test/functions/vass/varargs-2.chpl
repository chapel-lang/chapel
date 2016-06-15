// Note there is a TODO in this test.
// When it is done, update the .good.


var A111: [1..2] int, i222 = 2222;

proc showit(tag, var11, var22) {
  writeln(tag, " :  ", var11, "  ", var22);
}


// these invoke showit()

proc show_default(xxx...) {
  xxx(1) = 4444;  // OK for an array
  xxx(2) = 5555;  // error: can't assign to 'const'
  showit(11, xxx(2), xxx(1));
}

proc show_const(const xxx...) {
  xxx(1) = 4444;  // TODO: error: can't assign to 'const'
  xxx(2) = 5555;  // error: can't assign to 'const'
  showit(12, xxx(2), xxx(1));
}

proc show_const_in(const in xxx...) {
  xxx(1) = 4444;  // error: can't assign to 'const'
  xxx(2) = 5555;  // error: can't assign to 'const'
  showit(13, xxx(2), xxx(1));
}

proc show_const_ref(const ref xxx...) {
  xxx(1) = 4444;  // error: can't assign to 'const'
  xxx(2) = 5555;  // error: can't assign to 'const'
  showit(14, xxx(2), xxx(1));
}


// these do not call showit()

proc test_default(xxx...) {
  xxx(1) = 4444;  // OK for an array
  xxx(2) = 5555;  // error: can't assign to 'const'
}

proc test_const(const xxx...) {
  xxx(1) = 4444;  // TODO: error: can't assign to 'const'
  xxx(2) = 5555;  // error: can't assign to 'const'
}

proc test_const_in(const in xxx...) {
  xxx(1) = 4444;  // error: can't assign to 'const'
  xxx(2) = 5555;  // error: can't assign to 'const'
}

proc test_const_ref(const ref xxx...) {
  xxx(1) = 4444;  // error: can't assign to 'const'
  xxx(2) = 5555;  // error: can't assign to 'const'
}



proc main {
  show_default(A111, i222);
  show_const(A111, i222);
  show_const_in(A111, i222);
  show_const_ref(A111, i222);

  test_default(A111, i222);
  test_const(A111, i222);
  test_const_in(A111, i222);
  test_const_ref(A111, i222);
}
