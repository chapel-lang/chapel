import ChapelDynamicLoading as mod;

config const hi = 2**17;

proc test0() {
  const b1: mod.chpl_localBuffer(int);
  assert(b1.size == 0);

  var b2 = new mod.chpl_localBuffer(int, hi);
  assert(b2.size == hi);

  var sum1 = 0;
  for i in 0..<hi {
    ref slot = b2[i];
    assert(slot == 0);

    slot = i;
    assert(slot == i && b2[i] == slot);

    // Size should never change.
    assert(b2.size == hi);

    // Accumulate the sum.
    sum1 += slot;
  }

  var sum2 = 0;
  for slot in b2 do sum2 += slot;

  // Confirm the sums match.
  assert(sum1 == sum2);
}

proc testLoopRangeSum(start: int, hi: int) {
  var sum = 0;
  for idx in start..<hi do sum += 1;
  for idx in 0..<start do sum += 1;
  assert(sum == hi);
}

// Ensure logic used to walk in ring-buffer style over the hashtable works.
proc test1() {
  testLoopRangeSum(0, 1024);
  testLoopRangeSum(765, 1024);
  testLoopRangeSum(1023, 2048);
}

proc test2() {
  var m: mod.chpl_localMap(int, int);

  // Size should not have changed yet.
  assert(m.size == 0);

  // Any key with a bitwise value of zero should not be added.
  const addedZeroKey = m.set(0, 128);
  assert(!addedZeroKey);

  // Size should not have changed yet.
  assert(m.size == 0);

  for i in 1..<hi {
    // Any key with a bitwise value of zero should not be added.
    const addedZeroKey = m.set(0, i);
    assert(!addedZeroKey);

    // Size should not have changed yet.
    assert(m.size == (i - 1));

    const addedIdx1 = m.set(i, 0);
    assert(addedIdx1);

    // Size should have increased by 1.
    assert(m.size == i);

    // The entry should have the value 0.
    var foundVal1;
    const foundIdx1 = m.get(i, foundVal1);
    assert(foundIdx1);
    assert(foundVal1 == 0);

    // Size should be the same.
    assert(m.size == i);

    // We should not have added an entry.
    const addedIdx2 = m.set(i, (i*2));
    assert(!addedIdx2);

    // Size should be the same.
    assert(m.size == i);

    // The entry should have the value 'i*2'.
    var foundVal2;
    const foundIdx2 = m.get(i, foundVal2);
    assert(foundIdx2);
    assert(foundVal2 == (i*2));

    // Size should be the same.
    assert(m.size == i);
  }
}

proc main() {
  test0();
  test1();
  test2();
}
