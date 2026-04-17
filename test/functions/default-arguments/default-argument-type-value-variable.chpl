// Test that type formals cannot have value variable defaults
// Related to issue #7955 (non-immediate case)

var intValue: int = 42;

proc test(type x = intValue) {
  writeln(x:string);
}

test();
