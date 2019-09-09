// List Operations

/*
  .. default-domain:: chpl

  The Chapel :mod:`List` module provides the implementation of the ``list``
  type. Lists are useful for building up and iterating over a collection
  of elements in a structured manner.
*/

private use List;

/*
  We'll start by declaring a list of ``int(64)`` and initializing it with the
  values contained in the range `1..8`.

  After initialization, we can zip `lst1`  against the range to make sure
  that the initializer did what it promised to do.
*/

const r = 1..8;
var lst1: list(int) = r;

assert(!lst1.isEmpty());

for (x, y) in zip(r, lst1) do
  assert(x == y);

writeln("List 1 after init: ", lst1);

/*
  The most common operation performed on a list is ``list.append``. The
  following code appends some integers to the end of our list.
*/

for i in r do
  lst1.append(i);

writeln("List 1 after appends: ", lst1);

/*
  If the ``list.parSafe`` value of a list is set to `true`, then elements may
  be safely appended to it in parallel. The ``parSafe`` value of a list can
  only be set during initialization, so we have to make a new list to test
  it out.
*/

var lst2: list(int, parSafe=true);

/*
  Now we can simulate adding elements to our new list in parallel.
*/

coforall tid in 0..3 with (ref lst2) do
  for i in r {
    const elem = tid * 8 + i;
    lst2.append(elem);
  }

assert(!lst2.isEmpty() && lst2.size == 32);

/*
  Tasks spawned in a forall loop aren't guaranteed to execute in a fixed
  order. The contents of `lst2` might be out of order even though our loop
  size is small (only 4 tasks).

  We can call ``list.sort`` on our list to be on the safe side.
*/

lst2.sort();

writeln("List 2 sorted: ", lst2);

/*
  We can create another new list with elements that are copied from `lst2`.
  The contents of the two lists should be identical.
*/

var lst3 = lst2;

assert(lst2.size == lst3.size);

for (x, y) in zip(lst2, lst3) do
  assert(x == y);

/*
  Before zippering `lst2` and `lst3` together, it would be good to vary their
  contents a little bit. Let's ``list.pop`` the first 16 elements from `lst2`
  and ``list.append`` them to `lst3`.

  .. note::
  
    The ``list.pop`` operation is O(n) worst case. If you pop an element from
    the front of a list, all the other elements after it have to be shifted
    one to the left.
*/

var count = 0;

while count < 16 {
  const elem = lst2.pop(1);
  lst3.append(elem);
  count += 1;
}

writeln("List 2 after pops: ", lst2);
writeln("List 3 after appends: ", lst3);

/*
  Let's ensure `lst2` and `lst3` have unique values. The ``list.remove``
  method takes a secondary argument specifying how many instances of a given
  element to remove. The value `0` will remove every instance of an element
  from a list. The default value removes a single instance.
*/

var removed = 0;

for elem in lst2 do
  removed += lst3.remove(elem, 0);

assert(removed == 16);

writeln("List 3 after removes: ", lst3);

/*
  It seems like `lst3` still has some duplicates in it. Let's remove those
  duplicates with a combination of ``list.remove`` and ``list.count``.
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

for elem in lst3 {
  const count = lst3.count(elem);
  assert(count == 1);
}

assert(lst2.size == lst3.size);

writeln("List 3 after removing duplicates: ", lst3);

/*
  Come to think of it, `lst1` is just wasting memory at this point. Let's
  go ahead and clear it using ``list.clear``.
*/

lst1.clear();

writeln("List 1 after clear: ", lst1);

/*
  Great. Now we can zipper our two lists together. Let's double check our work
  and make sure that our two lists really share no elements in common.

  .. note::

    List iterators are not thread safe. They implicitly assume that their list
    is not being modified while iteration is occurring, and it is the user's
    responsibility to abide by this assumption.
*/

forall (x, y) in zip(lst2, lst3) {
  assert(!lst3.contains(x));
  assert(!lst2.contains(y));
}

/*
  We can use the ``list.extend`` method to merge the contents of our lists
  together. Since `lst1` is now empty, we can reuse it to save space.
*/

assert(lst1.isEmpty());

lst1.extend(lst2);
lst1.extend(lst3);

writeln("List 1 after extends: ", lst1);

/*
   Whoops. It looks like the contents of `lst1` are backwards. We could call
   ``list.sort`` to fix this problem...or we fix the contents of the list
   ourselves!

   .. note::

    The ``list.this`` subscripting operator will cause the currently running
    program to halt if the index requested is out of bounds. Be careful!
*/

for i in 1..(lst1.size / 2) {
  ref a = lst1[i];
  ref b = lst1[i + lst1.size / 2];
  a <=> b;
}

writeln("List 1 after correction: ", lst1);

/*
  If you need to get the specific index of an element contained in a list,
  you can use the ``list.indexOf`` operator.

  .. note::

    The ``list.indexOf`` operator will halt if the given search range falls
    outside the bounds of the list.
*/
for x in lst2 {
  const idx = lst1.indexOf(x);
  assert(x == idx);
}

for x in lst3 {
  const idx = lst1.indexOf(x);
  assert(x == idx);
}

