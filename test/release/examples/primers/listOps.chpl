// List Operations

/*
  .. default-domain:: chpl

  The Chapel :mod:`List` module provides the implementation of the ``list``
  type. Lists are useful for building up and iterating over a collection
  of values in a structured manner.
*/

private use List;

config const quiet: bool = false;

/*
  We'll start by declaring a list of ``int(64)`` and initializing it with the
  values contained in the range ``1..8``.
*/

var lst1: list(int) = 1..8;

writeln("List 1 after init: ", lst1);

/*
  The most common operation performed on a list is ``list.append()``. The
  following code appends some integers to the end of our list.

  .. note::

    The ``list`` type guarantees that appending a value to the end of a list
    will not invalidate references to existing values.

    This is particularly useful in a parallel context. One task may append
    values to the end of a list while others index over existing values
    and use them in useful work.
*/

for i in 1..8 do
  lst1.append(i);

writeln("List 1 after appends: ", lst1);

/*
  If a list is intended to be used in parallel, then the ``parSafe`` field
  must be set to ``true`` at initialization. 

  Let's create a new list and ``list.append()`` values to it in parallel.
*/

var lst2: list(int, parSafe=true);

coforall tid in 0..3 with (ref lst2) {
  for i in 1..8 {
    const elem = tid * 8 + i;
    lst2.append(elem);
  }
}

/*
  Tasks spawned in a coforall loop aren't guaranteed to execute in a fixed
  order. The contents of ``lst2`` might be out of order even though our loop
  size is small (only 4 tasks).

  We can call ``list.sort()`` on our list to be on the safe side.
*/

if !quiet then
  writeln("List 2 before sort: ", lst2);

lst2.sort();

writeln("List 2 sorted: ", lst2);

/*
  We can create another new list with values that are copied from ``lst2``.
*/

var lst3 = lst2;

/*
  Before zippering ``lst2`` and ``lst3`` together, it would be good to vary
  their contents a little bit. Let's ``list.pop()`` the values from the first
  half of ``lst2`` and ``list.append()`` them to ``lst3``.

  .. note::
  
    The ``list.pop()`` operation is O(n) worst case. If you pop a value
    from the front of a list, all the other values after it have to be
    shifted one to the left.
*/

var count = 0;

while count < 16 {
  const elem = lst2.pop(0);
  lst3.append(elem);
  count += 1;
}

writeln("List 2 after pops: ", lst2);
writeln("List 3 after appends: ", lst3);

/*
  Let's ensure ``lst2`` and ``lst3`` have unique values. The ``list.remove()``
  method takes a secondary argument called `count` which specifies how many
  instances of a value to remove. Passing ``0`` to `count` will remove every
  value matching the input from a list.
*/

for elem in lst2 do
  lst3.remove(elem, 0);

writeln("List 3 after removes: ", lst3);

/*
  Even though ``lst2`` and ``lst3`` have no values in common, ``lst3`` still
  has some duplicate values that could be removed. We can do that with a
  combination of ``list.remove()`` and ``list.count()``.

  .. warning::

    You should be careful not to modify the structure of a list while it is
    being iterated over, as this can cause the iterator to exhibit
    undefined behavior.

    In this example, you'll notice that we break out of our loop and start
    over if we happen to remove duplicate values during the current
    iteration.
*/

var uniqued = false;

while !uniqued do
  for elem in lst3 {
    const count = lst3.count(elem);
    if count > 1 {
      lst3.remove(elem, count - 1);
      break;
    }
    uniqued = true;
  }

writeln("List 3 after removing duplicates: ", lst3);

/*
  Great. Now we can zipper our two lists together. Let's double check our work
  and make sure that our two lists really share no values in common.

  .. warning::

    List iterators are not thread safe. They implicitly assume that their list
    is not being modified while iteration is occurring, and it is the user's
    responsibility to abide by this assumption.

    It is safe to modify the values of a list (i.e, references returned via
    indexing into a list) in a forall loop, but not the list itself.
*/

forall (x, y) in zip(lst2, lst3) {
  assert(!lst3.contains(x));
  assert(!lst2.contains(y));
}

/*
  It seems like ``lst1`` is just wasting memory at this point. Let's go ahead
  and clear it using ``list.clear()``. This will remove every value from
  the list and set its size to ``0``.
*/

lst1.clear();

writeln("List 1 after clear: ", lst1);

/*
  We can use the ``list.extend()`` method to merge the contents of our lists
  together. Since ``lst1`` is now empty, we can reuse it to save space.
*/

lst1.extend(lst2);
lst1.extend(lst3);

writeln("List 1 after extends: ", lst1);

/*
   You'll notice that the contents of ``lst1`` are backwards. We could call
   ``list.sort()`` to fix this problem...or we can fix the contents of the
   list ourselves!

   .. warning::

    Indexing a list with a value that is out of bounds will cause the
    currently running program to halt. Be careful!
*/

for i in 0..#(lst1.size / 2) {
  ref a = lst1[i];
  ref b = lst1[i + lst1.size / 2];
  const tmp = a;
  a = b;
  b = tmp;
}

writeln("List 1 after correction: ", lst1);

/*
  If you need to get the specific index of a value contained in a list, you
  can use the ``list.indexOf()`` operator.

  .. warning::

    The ``list.indexOf()`` operator will halt if the search range specified
    by the arguments `start` and `end` falls outside the bounds of the list.
*/

for x in lst2 {
  const idx = lst1.indexOf(x);
  assert(x == idx+1);
}

for x in lst3 {
  const idx = lst1.indexOf(x);
  assert(x == idx+1);
}

/*
  And finally, you can use the ``list.insert()`` method to insert a value
  at any position in a list.

  As a trivial example, let's insert the value ``-100`` at the front of
  ``lst1``.

  .. note::

    Similar to ``list.pop()``, the ``list.insert()`` operation is O(n) in
    the worst case.

  .. warning::

    The ``list.insert()`` method will halt if the index specified by the
    argument `idx` falls outside the bounds of the list.
*/

lst1.insert(0, -100);

writeln("List 1 after inserting -100: ", lst1);
