config const testPopBackEmpty = false,
             testPopFrontEmpty = false,
             testInsertBadPos = false,
             testRemoveBadPos = false,
             testRemoveBadPosCount = false,
             testRemoveBadRange = false;

if testPopBackEmpty {
  var A: [1..0] int;
  A.pop_back();
}
if testPopFrontEmpty {
  var A: [1..0] int;
  A.pop_front();
}
if testInsertBadPos {
  var A: [1..5] int;
  A.insert(7, 3);
}
if testRemoveBadPos {
  var A: [1..5] int;
  A.remove(7);
}
if testRemoveBadPosCount {
  var A: [1..5] int;
  A.remove(4, 4);
}
if testRemoveBadRange {
  var A: [1..5] int;
  A.remove(4..8);
}
