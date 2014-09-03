/*

  Associative Primer

  This is a tutorial on Chapel's associative domains and arrays.

  It would be beneficial to read the 'arrays.chpl' and 'domains.chpl'
  primers before proceeding.

*/

config const n = 20;

//
// Associative domains represent an arbitrary index set of any type.
//
// Names is an associative domain whose indices are strings.  Associative domains
// start out empty (no indices).
//
var Names: domain(string);
writeln("An empty associative domain: ", Names);
writeln();

//
// The '+=' operator can be used to add indices to an associative domain.
//
Names += "Alice";
Names += "Bob";
Names += "Clark";
Names += "Dana";

//
// The 'add' method is equivalent to '+='
//
Names.add("Ellen");
Names.add("Frank");
writeln("A populated associative domain of strings: ", Names);
writeln();


//
// The '-=' operator is used to remove indices from an associative domain.
// The 'remove' method performs the same function.
//
Names -= "Bob";
Names += "Robert";
writeln("Removed 'Bob', added 'Robert': ", Names);
writeln();

//
// If we want to check that "Bob" was indeed removed we can use the
// 'member' function, which returns a boolean.
//
assert(!Names.member("Bob"));

//
// Like all other domains, we can use associative domains to define an array.
// Scores is the Chapel-equivalent of a 'map', 'dictionary', or 'hash' found in
// other languages.
//
// Scores: an associative array of strings to integers.
//
var Scores : [Names] int;

//
// All array elements are initialized to the default value for that type.
//
writeln("Our first associative array: ", Scores);

//
// Printing an array only prints its values. Let's write a function that 
// prints our associative array in a more human-readable format.
//

//
// given an array, print in the following format:
// { idx => val, ... }
//
proc prettyPrint(arr : []) {
  write("{ ");
  var first = true;
  for k in arr.domain.sorted() {
    if !first then write(", ", k, " => ", arr[k]);
    else {
      write(k, " => ", arr[k]);
      first = false;
    }
  }
  writeln(" }");
}

write("Our first array, pretty-printed: ");
prettyPrint(Scores);
writeln();

//
// We can also implicitly add indices to an associative array if the 
// underlying domain defines only one array.
//
// Here, the string "Gary" is added as an index, and the corresponding 
// array element is initialized to 10.
//
Scores["Gary"] = 10;
write("Our Scores array after adding 'Gary': ");
prettyPrint(Scores);
writeln();

//
// We can also verify that the underlying 'Names' domain was modified:
//
assert(Names.member("Gary"));

//
// If we were to define another array with 'Names' as the backing domain,
// then we could no longer use this convenient syntax.
//
// Here, Ages and Scores are backed by the same associative domain, Names.
// We can no longer write something like
//   Ages["Mark"] = 18;
// if "Mark" is not a valid index.
//
var Ages : [Names] int;

//
// This limitation exists because it is not obvious that other arrays
// are being modified. After you define multiple arrays with one domain, 
// you should use that domain to add more indices.
//
Names.add("Mark");
Ages["Mark"] = 18;

//
// The program will still halt if one tries to access an index
// not in the domain.
//
// HALTS: writeln(Scores["Sally"]);
//

//
// Various set operations are available on associative domains and arrays.
// Union (| or +)
// Intersection (&)
// Difference (-)
// Symmetric Difference (^)
//

//
// We'll use domains of integers for convenience, but 
// these set operations work for any index type.
//

var odds, evens, ltHalf : domain(int);
odds += 1..n by 2;
evens += 1..n by 2 align 2;
ltHalf += 1..(n/2);

//
// Note: The sorted() method returns an array, not a sorted domain
//
writeln("Integers from 1..n composed of odds | evens: ", (odds | evens).sorted());
writeln("Odd integers below n/2: odds & ltHalf = ", (odds & ltHalf).sorted());
writeln("Another way to get the same result: ltHalf - evens = ", (ltHalf - evens).sorted());
writeln("Even below n/2, odd above: ltHalf ^ odds = ", (ltHalf ^ odds).sorted());
writeln();

//
// Let's use the array-literal syntax to create a couple of arrays to use 
// as examples for set operations.
//
var A = ["a" => true, "b" => false, "c" => true];
var B = ["b" => true, "c" => false, "d" => true];
writeln("Some simple associative arrays:");
write("A = ");
prettyPrint(A);
write("B = ");
prettyPrint(B);

//
// When performing a union between two arrays and indices conflict, the values
// of the second array take precedence.
//
var C = A | B;
write("C is the union of A and B; C = A | B : ");
prettyPrint(C);

//
// We can also use the |= operator on associative arrays. However, this
// is only permitted when the array's domain doesn't define any other arrays.
// Again, this is limitation exists so that other arrays aren't modified in
// a surprising way.
//
A |= B;
write("A becomes the union of A and B; A |= B: ");
prettyPrint(A);
