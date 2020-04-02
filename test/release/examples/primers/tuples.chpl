// Tuples

//
// This primer demonstrates tuples.
//


// Tuples are a way of grouping several values together. The number of
// components in each tuple must be known at compile-time. However,
// unlike an array, a tuple can store components of different types.
// 
// Tuples are a language-supported type. Tuples automatically get
// assignment and comparison operators.
//
// Tuple indices always start at 1.

// Getting Started with Tuples
// ***************************
// 
// The easiest way to create a tuple is with a tuple literal.
// Tuple literals syntax looks like this:

var myTuple = (1, "two");

// Now ``myTuple`` stores a tuple with two elements:
//   ``myTuple(1) == 1`` and
//   ``myTuple(2) == 2``
assert(myTuple(1) == 1);
assert(myTuple(2) == "two");

// As we saw above, to access an individual tuple element,
// use ``someTuple(i)`` or ``someTuple[i]``. The first tuple
// component is at index ``1``.

// We can assign to a component in our tuple variable
myTuple(2) = "four";

// We can print out the tuple. The output format mirrors
// the Chapel syntax.
writeln(myTuple); // output: (1, four)

// Sometimes it's useful in generic code to query the size of a tuple.
// The ``.size`` method returns the size of a tuple.
writeln(myTuple.size); // output: 2


// Tuple Types
// ***********
// 
// The syntax for tuple types mirrors the tuple literal syntax.

// The following line declares a tuple variable storing an int and a string
var otherTuple: (int, string);

// We can assign to that tuple from a variable with the same type:
otherTuple = myTuple;

// The following line declares a tuple variable holding 3 real values:
var threeReals: 3*real;
writeln(threeReals); // output: (0.0, 0.0, 0.0)

// Tuples that have only one component type support
// some arithmetic on the components:
threeReals += 1.0;
writeln(threeReals); // output: (1.0, 1.0, 1.0)

// Tuples with only one component type are also called homogeneous
// tuples whereas tuples with mixed types are called heterogeneous
// tuples.

// Tuple Iteration
// ***************

// Tuples can be iterated over using for loops:

var sum = 0.0;
for r in threeReals {
  sum += r;
}
writeln(sum);

// In addition, homogeneous tuples can be iterated over using forall
// and coforall loops.

// Heterogeneous tuples can also be traversed using for and coforall
// loops.  Such loops are always unrolled by the compiler so that each
// iteration has its own index variable of the appropriate type.  For
// example, in the following loop, the index ``t`` of the first
// iteration would be of type ``int`` while the index ``t`` of the
// second iteration would be of type ``string`` since ``myTuple`` has
// type ``(int, string)``:

for t in myTuple {
  writeln(t);
}

// Tuple Unpacking
// ***************
// 
// Tuple values can be unpacked into variables during
// variable initialization:

var (myInt, myString) = otherTuple;
writeln(myInt, " ", myString); // output: 1 four

// Or assignment:
var x, y, z: real;
(x, y, z) = threeReals;

// It's also possible in `for` and `forall` loops:
var ArrayOfTuples: [1..3] (int, real);
for (i,r) in ArrayOfTuples {
  i = 1;
  r = 0.0;
}
writeln(ArrayOfTuples);

// Or in function calls:
proc magnitude( (x,y,z):3*real ) {
  return sqrt(x*x + y*y + z*z);
}
threeReals = (1.0, 2.0, 2.0);
var m = magnitude(threeReals);
writeln(m);

// Additionally, tuples can be unpacked in the process
// of making a function call. For example, suppose we
// have a function accepting individual arguments:
proc add(x, y, z) {
  return x + y + z;
}

// If we want to pass threeReals to it, we could write
add(threeReals(1), threeReals(2), threeReals(3));
// but the tuple can be unpacked at the call site like this:
var total = add( (...threeReals) );
writeln(total);

// One-Element Tuples
// ******************
// 
// Sometimes one needs to create a tuple storing just one element.
// To do that, just use the following syntax:

var oneTuple = (1.0,);
writeln(oneTuple);
