

pragma "no copy return"
proc slicer(A: []) {
  ref rankChange = A[1, ..];
  ref slice = rankChange[1..5];
  return slice;

  // Oops, 'rankChange' will be autoDestroyed as it goes out of scope!
  // Using the returned slice will now result in either segfaults or
  // undefined behavior as the rankChange metadata will be lost.
  //
  // At the time of this comment, there's still an ongoing discussion about
  // returning an alias of an array. The core issue here is not about returning,
  // but about the current implementation of ArrayViews. By 'stacking' views,
  // if a view lower in the stack (here, rankChange) is destroyed then any
  // view above will be useless. With that in mind, focus on the use-after-free
  // errors in valgrind instead of memory leaks (if any).
}

proc main() {
  var A : [1..10, 1..10] int;
  for i in 1..10 do A[i, ..] = i;
  ref S = slicer(A);
  writeln(S);
}
