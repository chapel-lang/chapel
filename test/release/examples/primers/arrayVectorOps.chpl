/*
 * Vector Operations on 1D Arrays Primer
 */

//
// 1D rectangular arrays support several list-like or vector-like capabilities.
// These are supported on arrays which have a 1:1 correspondence with their
// domains. This restriction is because many of these operations modify the
// array's domain, which would unexpectedly modify other arrays if they shared
// domains.
//
// As currently implemented, every operation that results in a domain size
// change results in an array reallocation and so is O(n).  This is expected
// to improve in a future release.
//

//
// Declare a 1D array and initialize it to the values 1..5. The anonymous
// domain used in this declaration has a 1:1 relationship with the array.
//
var A: [1..5] int = [i in 1..5] i;

writeln("A was initialized to: ", A);

//
// Add an element to the front of the array and one to the back of the array.
// This will modify the array's domain to become {0..6} and add elements
// 0 and 6 to the array.
//
A.push_front(0);
A.push_back(6);

writeln("After adding to the front and back A is: ", A);

//
// Look at the first and last array elements without modifying the array
//
writeln("The first and last elements in A are: ", (A.head(), A.tail()));

//
// Remove the two elements added earlier with push_front and push_back.
// The domain will become {1..5} again.
//
A.pop_front();
A.pop_back();

writeln("After popping the two elements pushed previously A is: ", A);

//
// Insert values 10, 11 and 12 at indices 4, 3, and 2.  The domain will
// grow by 3 to become {1..8} and the array elements above the inserted
// positions will be shifted up.
//
A.insert(4, 10);
A.insert(3, 11);
A.insert(2, 12);
writeln("After inserting some new values, A is: ", A);


//
// The method 'find' searches the array for the argument.  It returns a tuple
// containing a bool and an index. If the returned bool is true, the argument
// was found at the returned index.  If the bool is false, the value was not
// found and the index is unspecified.
//
var (found, idx) = A.find(10);
if found then
  writeln("Found 10 at index: ", idx);
else
  writeln("Didn't find 10");

(found, idx) = A.find(7);
if found then
  writeln("Found 7 at index: ", idx);
else
  writeln("Didn't find 7");

//
// A few other useful methods are available.
// To demo them, add a few more 5s to the array
//
A.push_front(5);
A.push_back(5);

//
// Count how many times an element is in the array
//
writeln("The value 5 is in A ", A.count(5), " times.");

//
// Reverse the elements in the array
//
writeln("Before calling reverse A is: ", A);
A.reverse();
writeln("After calling reverse A is: ", A);

//
// Array elements can be removed one at a time by specifying an
// index to remove.  The elements above the removed one will be
// shifted down.
//
A.remove(3);
writeln("After first remove A is: ", A);

//
// A range of indices can also be removed
//
A.remove(4..6);
writeln("After second remove A is: ", A);

//
// A starting index and a count also work
//
A.remove(2, 2);
writeln("After third remove A is: ", A);

//
// The array is still a normal 1D Chapel array and supports regular array
// operations such as parallel iteration:
//
forall i in A.domain {
  A[i] += 1;
}
writeln("After adding 1 to all elements A is: ", A);

//
// Or reductions:
//
writeln("The sum of elements in A is ", + reduce A);

//
// The clear method will empty the array completely.  If the domain's low
// bound was 'low' this sets the domain to {low..low-1}
//
A.clear();
writeln("After clearing, A is: ", A, " - with ", A.size, " elements");
