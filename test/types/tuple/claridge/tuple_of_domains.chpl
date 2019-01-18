/*
  Noakes 2017/03/08

  This program attempts to allocate a variable that is a tuple of
  domains that relies on the compiler to instantiate a valid default
  initializer.

  A number of interactions within the compiler cause the elements of
  the tuple to be un-initialized and so this program has undefined
  behavior.

  The skipif for this future restricts it to valgrind testing
*/

var domain_tuple: 2 * domain(1);

writeln(domain_tuple);
