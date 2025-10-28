// Test that type formals cannot have value literal defaults
// This is the primary bug from issue #7955

proc test(type x = 42) {
  writeln(x:string);
}

test();
