// Variadic Arguments

/*
   This primer demonstrates procedures with variable length arguments lists.
*/

//
// Procedures can be defined with variable length argument lists. The
// following procedure accepts integer arguments and defines the
// parameter ``n`` as the number of arguments passed to the current
// call. The args argument is an ``n``-tuple of ``int`` values.
//
proc intWriteln(args: int ...?n) {
  for i in args.indices {
    if i != n-1 then
      write(args(i), " ");
    else
      writeln(args(i));
  }
}

intWriteln(1, 2, 3, 4);

//
// By eliding the type of the ``args`` argument, the variable arguments can be
// made generic. The following procedure takes ``n`` arguments of any type and
// writes them on a single line. Here, ``args`` is a heterogeneous ``n``-tuple,
// so a parameter for loop is used to unroll the loop body so that the
// index ``i`` is a parameter and each ``write()`` call can be instantiated
// independently.
//
proc anyTypeWriteln(args...?n) {
  for param i in 0..<n {
    if i != n-1 then
      write(args(i), " ");
    else
      writeln(args(i));
  }
}

anyTypeWriteln(1, 2.0, 3.14 + 2.72i);

//
// Variable arguments can also be types instead of values. This procedure
// accepts a variable number of types and returns a tuple containing the
// default values for each type.
//
proc defaultValues(type args...?n) {
  var val: args;
  return val;
}

//
// Write the default values of each of these types, using the previously
// defined function. Because the ``defaultValues`` procedure returns a tuple,
// it is destructured before being passed to the writer procedure by using
//the tuple destructuring expression: ``(...Tuple)``.
//
anyTypeWriteln((...defaultValues(int, complex, bool, 2*real)));

//
// This procedure uses `Euclid's algorithm`_ to compute the greatest common
// divisor of two integers. It is the base case for a version below that
// finds the gcd of any number of integers.
//
proc gcd(in a:int, in b:int) {
  while b != 0 {
    var temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

//
// Find the greatest common divisor of ``n+2`` integers. Use the base case
// defined above for the first two arguments, and recursively compare that
// against the gcd of the rest of the arguments.
//
proc gcd(a:int, b:int, c:int ...?n) {
  return gcd(gcd(a, b), (...c));
}

writeln(gcd(100, 25, 50, 200));

//.. _Euclid's algorithm: https://en.wikipedia.org/wiki/Euclidean_algorithm
