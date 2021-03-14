/*
 * This is a simplified version of vararg-type-tuple.chpl that
 * helped to localize the source of the problem that was affecting
 * that test.
 *
 */


record Foo {
  type retType;

  var val : retType;
}

var A : Foo(int);
var B : Foo(real);
var C : Foo(string);

proc main() {
  var R = foo();

  writeln("R = ", R);
}

proc foo() {
  var   arg1s    = (A, B, C);
  type  retTypes = bar((...arg1s));
  var   retval   : retTypes;

  for param i in 0 .. 2 {
    retval(i) = arg1s(i).val;
  }

  return retval;
}


proc bar(a, b, c) type {
  return (a.retType, b.retType, c.retType);
}
