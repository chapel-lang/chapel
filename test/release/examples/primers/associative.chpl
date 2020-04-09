// Associative Domains and Arrays

/*

  This is a tutorial on Chapel's associative domains and arrays.

  It may be beneficial to read the :ref:`arrays.chpl <primers-arrays>` and
  :ref:`domains.chpl <primers-domains>` primers before proceeding if you're not
  already familiar with Chapel's domains and arrays.

*/

//
// Associative Domains
// -------------------
//
// Associative domains are commonly used to represent arbitrary sets, or to
// represent the set of keys for a dictionary/hash table.
//
// The current implementation supports most Chapel types, and will eventually
// support any language-defined type.
//
// The following block contains some examples of associative domains with
// various index types.
//
{
  var A : domain(int);     // a domain (set) whose indices are integers
  var B : domain(string);  // a domain (set) whose indices are strings
  var C : domain(real);    // a domain (set) whose indices are reals

  class X {
    var x : int;
  }
  
  //
  // A domain whose indices are classes of type 'X'. The indices are hashed by
  // identity.
  //
  var D : domain(borrowed X);
  
  record Y {
    var y : real;
  }

  //
  // A domain whose indices are records of type 'Y'. The indices are hashed by
  // value.
  //
  var E : domain(Y);
}

//
// Below, ``Names`` is an associative domain of strings.  Associative domains
// start out empty (no indices).
//
var Names: domain(string);
writeln("An empty associative domain: ", Names);
writeln();

//
// We can use the ``size`` method to confirm the emptiness of the ``Names`` domain.
//
writeln("The initial size of the 'Names' domain is ", Names.size);
writeln();

/*
We can also use the domain-literal syntax to create an associative domain.

Below, ``Days`` is an associative domain of strings.
*/
var Days = {"Sunday", "Wednesday", "Saturday"};

/*
  This declaration of ``Days`` relies on Chapel's type inference, and is
  equivalent to

  .. code-block:: chapel

       var Days : domain(string) = {"Sunday", "Wednesday", "Saturday"};

*/

//
// The ``+=`` operator can be used to add indices to an associative domain.
//
Names += "Alice";
Names += "Bob";
Names += "Clark";
Names += "Dana";

//
// The ``add`` method is equivalent to ``+=``
//
// The order in which indices are printed is not guaranteed for associative
// domains.
//
Names.add("Ellen");
Names.add("Frank");
writeln("A populated associative domain of strings: ", Names);
writeln();

//
// Associative domains cannot contain duplicate indices.
//
const PreviousNamesSize = Names.size;
Names.add("Alice");
if Names.size != PreviousNamesSize then
  halt("Error: Added duplicate index, but size of domain changed");

//
// Below we can add an array of strings with the ``+=`` or ``add`` operators
// because of Chapel's standard function promotion semantics. For the
// ``ColorSet`` domain, those functions take string arguments. Because
// ``PrimaryColors`` is a collection of strings Chapel promotes the routine,
// calling it for each string represented in the array.
//
var PrimaryColors : [1..3] string = ["Red", "Green", "Blue"];
var ColorSet : domain(string);
ColorSet += PrimaryColors;

//
// The ``-=`` operator is used to remove indices from an associative domain.
// The ``remove`` method performs the same function.
//
Names -= "Bob";
Names += "Robert";
Names.remove("Frank");
writeln("Removed 'Bob' and 'Frank', added 'Robert': ", Names);
writeln();

//
// If we want to check that "Bob" and "Frank" were indeed removed we can use the
// ``contains`` function, which returns a boolean.
//
if Names.contains("Bob") then
  halt("Error: Bob should have been removed!");
if Names.contains("Frank") then
  halt("Error: Frank should have been removed!");

//
// Associative Arrays
// ------------------
//
// Like all other domains, we can use associative domains to define an array.
// ``Scores`` is the Chapel-equivalent of a "map", "dictionary", or "hash
// table" found in other languages. Where ``Names`` provides the keys,
// ``Scores`` provides the values.
//
// Scores: an associative array mapping strings to integers.
//
var Scores : [Names] int;

//
// Like other arrays, we can initialize associative array elements to have
// an initial value.
//
var HoursInDay : [Days] int = 24;
for hours in HoursInDay do
  if hours != 24 then
    halt("HoursInDay's values should have all been initialized to '24'");

//
// We could also use the array-literal syntax to create an associative
// array.
//
// Below, ``DaysInMonth`` is an associative array mapping strings to integers.
//
var DaysInMonth = ["June" => 30, "January" => 31, "September" => 30];

//
// All array elements are initialized to the default value for that type.
//
writeln("Our 'Scores' associative array: ", Scores);

//
// Printing an array only prints its values. Let's write a function that 
// prints our associative array in a more human-readable format.
//

//
// Given an array, print in the following format:
// [ idx => val, ... ]
//
// We'll use the ``sorted`` iterator to print in a consistent order. Otherwise,
// the order in which indices are yielded is nondeterministic.
//
proc prettyPrint(arr : [?dom]) {
  write("[ ");
  var first = true;
  for k in dom.sorted() {
    if !first {
      write(", ", k, " => ", arr[k]);
    } else {
      write(k, " => ", arr[k]);
      first = false;
    }
  }
  writeln(" ]");
}


//
// Using our new routine, print our array.
//
write("Our 'Scores' array, pretty-printed: ");
prettyPrint(Scores);
writeln();

//
// Let's initialize our ``Scores`` array.
//
// Traditional array operations like whole-array assignment and indexing
// are supported for associative arrays.
//
Scores = 100; // every value in 'Scores' is now '100'.

Scores["Robert"] = 33;
Scores["Alice"] = 42;
Scores["Dana"] = 91;
write("Our 'Scores' array initialized with some values: ");
prettyPrint(Scores);
writeln();

//
// Like other Chapel arrays, we can use reductions.
//
writeln("The average score is ", (+ reduce Scores) / Scores.size);
writeln();

//
// Because Chapel arrays are updated when their domain is changed, we can
// add indices after the array is declared. When we add an index to the 
// defining domain, the array is extended to support an element for that index.
// The new element is initialized to the default value of its type.
//
Names += "John";
Names += "Gary";

if Scores["John"] != 0 then
  halt("Scores[\"John\"] should have been initialized to 0.");

//
// Once added to the ``Names`` domain, we can use it like any other index in
// the array.
//
Scores["John"] = 76;
Scores["Gary"] = 10;

write("Our updated 'Scores' array after adding some more names: ");
prettyPrint(Scores);
writeln();

/*
  The first line of code in the previous block is equivalent to:

  .. code-block:: chapel

       Names += "Gary";
       Scores["Gary"] = 10;
*/

//
// We can also verify that the underlying ``Names`` domain was modified:
//
if !Names.contains("Gary") then
  halt("Error: 'Gary' should be present in the 'Names' domain");

//
// If we were to define another array with ``Names`` as the backing domain,
// then we could no longer use this convenient feature.
//
var Ages : [Names] int;

/*
  Here, since ``Ages`` and ``Scores`` are backed by the same associative
  domain, we can no longer write something like

  .. code-block:: chapel

       Ages["Mark"] = 33;

  or

  .. code-block:: chapel

       Score["Sam"] = 42;

  since "Mark" or "Sam" are not valid indices in the ``Names`` domain.
*/


//
// This restriction exists because Chapel binds arrays to their domains for
// their lifetime to support semantic reasoning by the compiler and users.
// Extending the array via indexing will modify its domain, which would in
// turn modify the keys and values of any other arrays sharing that domain.
//
// We consider this behavior to be too surprising to support ("I modified
// ``Ages``, why did ``Scores`` change as well?"). Thus, to extend an array which
// shares its domain, the user is required to modify the domain directly.
// By modifying such a domain, it should be less surprising that arrays
// bound to that domain are affected.
//
Names.add("Mark");
Ages["Mark"] = 33;
Scores["Mark"] = 81;


/*
  The program will still halt with an out-of-bounds error if one tries to
  access an index not in the domain:

  .. code-block:: chapel

     writeln(Scores["Sally"]);
*/


/*
  Set Operations
  --------------

  Set operations are available on associative domains and arrays.

  The supported set operations are:
    - Union (``|`` or ``+``)
    - Intersection (``&``)
    - Difference (``-``)
    - Symmetric Difference (``^``)

  For both associative domains and arrays, the ``op=`` variants are supported:
   ``|=, +=, &=, -=, and ^=``
*/


var primeDom = {2, 3, 5, 7, 11, 13, 17};  // some prime numbers
var fibDom   = {0, 1, 1, 2, 3, 5, 8, 13}; // part of the Fibonacci sequence

var primeAndFib = primeDom & fibDom;
writeln("Some primes in the Fibonacci sequence: ", primeAndFib);
writeln("Some primes not in the Fibonacci sequence: ", primeDom - primeAndFib);
writeln();

var Women = {"Alice", "Dana", "Ellen"};
var Men = Names - Women;

writeln("Women = ", Women);
writeln("Men = ", Men);
writeln();

if (Men | Women) != Names then
  halt("The union of the 'Men' and 'Women' sets should be equivalent to 'Names'");

//
// Special rules exist for associative array set operations.
//
// If performing a union on two associative arrays with overlapping indices,
// the value of the second array takes precedence.
//
// For the ``op=`` variants on associative arrays, the array must not share its
// domain with another array.
//

/*
Future Directions
-----------------

Today, associative domains cannot be distributed across multiple
locales.  A prototype domain map for this exists, and the effort to make
it a polished feature could be accelerated with sufficient user
interest.
*/
