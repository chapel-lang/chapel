// Atomics

/*

  This primer illustrates Chapel's atomic variables.  For more information on
  Chapel's atomics, see the :ref:`Atomic_Variables` section of the language
  specification.

*/

// The ``atomic`` keyword is a type qualifiers that can be applied to
// the following Chapel primitive types to declare atomic variables:
//
// - ``bool``
// - ``int`` (all supported sizes)
// - ``uint`` (all supported sizes)
// - ``real`` (all supported sizes)
//
// Atomic variables support a limited number of operations that are
// currently implemented as methods.
//

// Atomic variables can only be used or assigned via the ``read()`` and
// ``write()`` methods, respectively.
//

config const n = 31;
const R = 1..n;

var x: atomic int;

x.write(n);
if x.read() != n then
  halt("Error: x (", x.read(), ") != n (", n, ")");

// All atomic types support atomic ``exchange()``, and ``compareAndSwap()``
//
// The ``exchange()`` method atomically swaps the old value with the given
// argument and returns the old value.  The ``compareAndSwap()`` method only
// perform the swap if the current value is equal to the first argument,
// returning ``true`` if the exchange was performed.
//
// In the following example, ``n`` parallel tasks are created via a
// ``coforall`` statement.  Each task tries to set the current value of ``x``
// to ``id-1``, but only one will succeed.
//
var numFound: atomic int;
coforall id in R {
  numFound.add(x.compareAndSwap(n, id-1));
}
if numFound.read() != 1 then
  halt("Error: numFound != 1 (", numFound.read(), ")");
if x.read() == n then
  halt("Error: x == n");

// As a convenience, ``atomic bools`` also support ``testAndSet()`` and
// ``clear()``. The ``testAndSet()`` method atomically reads the value of
// the variable, sets it to ``true``, then returns the original value.  The
// ``clear()`` method atomically sets the value to ``false``.
//
// In the following example, we create ``n`` tasks and each task calls
// ``testAndSet()`` on the atomic variable named ``flag`` and saves the
// result in a unique element of the ``result`` array.
//
var flag: atomic bool;
var result: [R] bool;
coforall r in result do
  r = flag.testAndSet();

// When the ``coforall`` is complete, only one of the above ``testAndSet()``
// calls should have returned ``false``.
//
var found = false;
for r in result {
  if !r then
    if found then
      halt("Error: found multiple times!");
    else
      found = true;
}
if !found then
  halt("Error: not found!");
flag.clear();


// The integral and ``real`` atomic types also support the following atomic
// fetch and non-fetching operations:
//
// - ``fetchAdd()`` and ``add()``
// - ``fetchSub()`` and ``sub()``
// - ``fetchOr()`` and ``or()`` (bit-wise) (integral types only)
// - ``fetchAnd()`` and ``and()`` (bit-wise) (integral types only)
// - ``fetchXor()`` and ``xor()`` (bit-wise) (integral types only)
//
// Each of the above atomically reads the variable, stores the result
// of the operation (``+``, ``-``, ``|``, ``&``, ``^``) using the value and the
// method argument, then, for the fetchOps functions, returns the original
// value.
//
// In the following example, we create ``n`` tasks to atomically increment
// the atomic variable ``a`` with the square of the task's given ``id``.
//
var a: atomic int;
coforall id in R do a.add(id*id);

// The sum of this finite series should be ``n(n+1)*(2n+1)/6``
//
var expected = n*(n+1)*(2*n+1)/6; 
if a.read() != expected then
  halt("Error: a=", a.read(), " (should be ", expected, ")");

// In the following example, we create ``n`` tasks to atomically increment
// the atomic variable ``a`` with the square of the task's given ``id``.  The
// sum of this finite series is ``n(n+1)*(2n+1)/6``. If the returned value
// is the expected value less ``id*id`` (last task to arrive), check the
// results.
//
a.write(0);
const sumOfSq = n*(n+1)*(2*n+1)/6;
coforall id in R {
  const mySq = id*id;
  const last = a.fetchAdd(mySq);
  if sumOfSq-mySq == last {
    const t = a.read();
    if t != n*(n+1)*(2*n+1)/6 then
      halt("Error: a=", t, " (should be ", sumOfSq, ") id=", id);
  }
}
