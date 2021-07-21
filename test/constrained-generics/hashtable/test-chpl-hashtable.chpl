/*
2021-06: this started as rev. 309ebb8173 of:
  test/types/chplhashtable/test-chpl-hashtable.chpl

WIP: conversion from record 'chpl__hashtable' to interface 'chpl_Hashtable'.
*/

use MyHashtable;

config var printInitDeinit = true;
config const debug = false;


proc printTable(h: chpl_Hashtable) {
  writeln("printing table tableSize=", h.tableSize,
          " tableNumFullSlots=", h.tableNumFullSlots);
  for slot in h.ifcAllSlots() {
    ref entry = h.table[slot];
    if entry.status == chpl__hash_status.full {
      writeln("slot ", slot, " full. key = ", toString(entry.key), " val = ", toString(entry.val));
    } else {
      writeln("slot ", slot, " ", entry.status:string, ".");
    }
  }
}

proc test1(ref h: chpl_Hashtable, key1: h.keyType, val1: h.valType) {
  writeln("test1");

  var foundFullSlot: bool;
  var slotNum: int;

  (foundFullSlot, slotNum) = h.findAvailableSlot(key1);
  assert(!foundFullSlot);
  assert(slotNum > 0);
  h.fillSlot(slotNum, key1, val1);

  for slot in h.ifcAllSlots() {
    if h.isSlotFull(slot) {
      ref entry = h.table[slot];
      writeln("key = ", toString(entry.key), " val = ", toString(entry.val));
    }
  }

  (foundFullSlot, slotNum) = h.findFullSlot(key1);
  assert(foundFullSlot);
  assert(slotNum > 0);

  var gotKey: h.keyType;
  var gotVal: h.valType;
  h.clearSlot(slotNum, gotKey, gotVal);
  assert(gotKey == key1);
  assert(gotVal == val1);
  h.maybeShrinkAfterRemove();
}

var h1 = new chpl__hashtable(int, int);
test1(h1, 1, 10); // instantiates 'chpl__hashtable implements chpl_Hashtable;'

class C {
  var xx: int = 0;
}

record R {
  var x: int = 0;
  var ptr: shared C = new shared C(0);
  proc init() {
    this.x = 0;
    this.ptr = new shared C(0);
    if printInitDeinit then writeln("init (default)");
  }
  proc init(arg:int) {
    this.x = arg;
    this.ptr = new shared C(arg);
    if printInitDeinit then writeln("init ", arg, " ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    this.ptr = new shared C(other.ptr.xx);
    if printInitDeinit then writeln("init= ", other.x, " ", other.ptr.xx);
  }
  proc deinit() {
    if printInitDeinit then writeln("deinit ", x, " ", ptr.xx);
  }
  proc toString() {
    return "(" + this.x:string + " " + this.ptr.xx:string + ")";
  }
}
operator R.=(ref lhs:R, rhs:R) {
  if printInitDeinit then writeln("lhs", lhs.toString(), " = rhs", rhs.toString());
  lhs.x = rhs.x;
  lhs.ptr = new shared C(rhs.ptr.xx);
}
operator R.==(const ref lhs: R, const ref rhs: R) {
  return lhs.x == rhs.x && lhs.ptr.xx == rhs.ptr.xx;
}
proc chpl__defaultHash(o: R) {
  return chpl__defaultHashCombine(chpl__defaultHash(o.x),
                                  chpl__defaultHash(o.ptr.xx),
                                  1);
}

R implements Hashable;
R implements StdOps;

printInitDeinit = false;
var globalRone = new R(1);
var globalRten = new R(10);
var globalRoneCopy = globalRone;
assert(globalRoneCopy == globalRone);
printInitDeinit = true;

/*
proc test2() {
  writeln("test2");
  var h = new chpl__hashtable(R, R);

  var foundFullSlot: bool;
  var slotNum: int;

  writeln("adding ", globalRone);
  {
    var key = globalRone;
    var val = globalRten;
    (foundFullSlot, slotNum) = h.findAvailableSlot(key);
    assert(!foundFullSlot);
    assert(slotNum > 0);
    h.fillSlot(slotNum, key, val);
  }

  if debug then
    printTable(h);

  writeln("iterating");
  for slot in h.allSlots() {
    if h.isSlotFull(slot) {
      ref entry = h.table[slot];
      writeln("key = ", entry.key, " val = ", entry.val);
    }
  }

  if debug then
    printTable(h);

  writeln("finding");
  (foundFullSlot, slotNum) = h.findFullSlot(globalRone);
  if debug then
    writeln("found slot ", slotNum);
  assert(foundFullSlot);
  assert(slotNum > 0);

  writeln("requestCapacity");
  h.requestCapacity(100);
  if debug then
    printTable(h);

  writeln("finding");
  (foundFullSlot, slotNum) = h.findFullSlot(globalRone);
  if debug then
    writeln("found slot ", slotNum);
  assert(foundFullSlot);
  assert(slotNum > 0);

  writeln("clearing");
  var gotKey: R;
  var gotVal: R;
  h.clearSlot(slotNum, gotKey, gotVal);
  assert(gotKey.x == 1);
  assert(gotVal.x == 10);
  h.maybeShrinkAfterRemove();
}
test2();
*/

var h2 = new chpl__hashtable(R, R);
test1(h2, globalRone, globalRten); // instantiates 'chpl__h implements chpl_H'

// todo: remove this test -- it is not interesting
proc test3() {
  writeln("test3");
  var h = new chpl__hashtable(R, R);
}
test3();

printInitDeinit = false;
