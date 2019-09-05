// List Operations

//
// This primer is about list operations
//

//
// [What are lists? What are they good for?]
//

/*
  Declare a list of ``int(64)`` and initialize it with the values ``1..8``.
  After that, let's make sure the initializer did what it was supposed to
  do!
*/
const r = 1..8;
var lst1: list(int) = r;

for (x, y) in zip(r, lst) do
  assert(x == y);

assert(!lst1.isEmpty());

writeln("List 1 was initialized to: ", lst1);

/*
  The most common operation performed on a list is an ``append``. The
  following code appends some integers to the end of our list.
*/
for i in 1..8 do
  lst1.append(i);

writeln("List 1 contains: ", lst1);

/*
  We've added the same values to our list twice. It would be convenient if we
  could empty our list before continuing the primer. We can do that with the
  ``clear`` method.
*/
lst1.clear();

assert(lst1.isEmpty());
writeln("List 1 contains: ", lst1);

/*
  If we set the ``parSafe`` value of our list to `true`, then we can safely
  append elements to it in parallel.
*/
lst1.parSafe = true;

/*
  Let's simulate adding a lot of elements to a list in parallel.
*/
coforall tid in 1..4 do
  forall i in 1..8 with (ref lst1) {
    const elem = tid * i;
    lst1.append(i);
  }

assert(!lst1.isEmpty() && lst1.size == 32);

writeln("List 1 contains: " lst1);

/*
  Odds are very good that our new list is out of order. Let's fix that with
  a sort.
*/
lst1.sort();

/*
  Lists can be zipped together in ``forall`` loops. Before we can do that,
  though, we need to make a second list.

  .. note::
  
    Lists have a ``parSafe`` value of `false` by default. To be consistent
    with our other list, we explicitly initialize `lst2` with a ``parSafe``
    value of `true`.
*/
var lst2: list(int, true);

/*
  Now let's pop the first half of `lst1` into `lst2`. All lists use one-based
  indexing, so to pop the first element we'd say `lst1.pop(1)`.

  .. note::
  
    The ``list.pop`` operation is O(n) worst case. If you pop an element from
    the front of a list, all the other elements after it have to be shifted
    one to the left.
*/
var halfSize: int = list.size / 2;

while lst1.size > halfSize {
  const elem = lst1.pop(1);
  lst2.append(elem);
}

assert(lst1.size == lst2.size);

writeln("List 1 contains: ", lst1);
writeln("List 2 contains: ", lst2);

/*
  Great. Now we can zipper our two lists together.

  .. note::

    List iterators are not thread safe. They implicitly assume that their list
    is not being modified while iteration is happening, and it is the user's
    responsibility to abide by this assumption.
*/
forall (x, y) in zip(lst1, lst2) {


}


//
// Look at the first and last array elements without modifying the array
//
writeln("The first and last elements in A are: ", (A.head(), A.tail()));

//
// Remove the two elements added earlier with ``push_front`` and ``push_back``.
// The domain will become ``{1..5}`` again.
//
A.pop_front();
assert(A.domain == {1..6});
A.pop_back();
assert(A.domain == {1..5});

writeln("After popping the two elements pushed previously A is: ", A);

//
// Insert values ``10``, ``11`` and ``12`` at indices ``4``, ``3``, and ``2``.
// The domain will grow by 3 to become ``{1..8}`` and the array elements above
// the inserted positions will be shifted up.
//
A.insert(4, 10);
A.insert(3, 11);
A.insert(2, 12);
assert(A.domain == {1..8});
writeln("After inserting some new values, A is: ", A);


//
// The method ``find`` searches the array for the argument.  It returns a tuple
// containing a bool and an index. If the returned bool is ``true``, the
// argument was found at the returned index.  If the bool is ``false``, the
// value was not found and the index is unspecified.
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
assert(A.domain == {0..9});

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
assert(A.domain == {0..8});
writeln("After first remove A is: ", A);

//
// A range of indices can also be removed
//
A.remove(4..6);
assert(A.domain == {0..5});
writeln("After second remove A is: ", A);

//
// A starting index and a count also work
//
A.remove(2, 2);
assert(A.domain == {0..3});
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
// The ``clear`` method will empty the array completely.  If the domain's low
// bound was ``low`` this sets the domain to ``{low..low-1}``
//
A.clear();
assert(A.domain == {0..-1});
writeln("After clearing, A is: ", A, " - with ", A.size, " elements");
