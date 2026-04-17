// Test that value formals with constraints properly reject mismatched literals
// Related to issue #7955 (value constraint validation)

proc test(x: integral = 3.14) {
  writeln(x);
}

test();
