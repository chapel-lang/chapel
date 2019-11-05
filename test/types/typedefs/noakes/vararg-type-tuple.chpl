/*
 * The following program was submitted by a user in April 2017 to
 * demonstrate an internal error that occured during resolution.
 *
 * The error has been fixed and this test locks that fix in.
 *
 *
 *
 * This program includes varargs, type functions, and tuples in a
 * relatively complex fashion.  In the end it turned out the problem
 * was the initialization of the type alias retTypes in makeTuple().
 *
 * See non-vararg.chpl for a slightly simpler variation that helped
 * to localize the problem.
 *
 */

record Foo {
  type      retType;

  var val : retType;
}

var A : Foo(int);
var B : Foo(real);
var C : Foo(string);



proc main() {
  var R = makeTuple(A, B, C);

  writeln("R = ", R);
}


proc makeTuple(arg0s ... ?N) {
  type retTypes              = tupleType((...arg0s));
  var  retval   : retTypes;

  for param i in 0 .. N-1 {
    retval(i) = arg0s(i).val;
  }

  return retval;
}



proc tupleType(arg) type {
  return (arg.retType,);
}

proc tupleType(arg, arg1s...) type {
  return (arg.retType, (...tupleType((...arg1s))));
}





