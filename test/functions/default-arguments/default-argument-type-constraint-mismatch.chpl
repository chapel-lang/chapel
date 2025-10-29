// Test that type formals with constraints properly reject mismatched types
// Related to issue #7955 (constraint validation)

proc test(type x: integral = real) {
  writeln(x:string);
}

test();
