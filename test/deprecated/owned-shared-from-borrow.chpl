// This test shows a pattern that is allowed in 1.18
// for legacy code reasons but should be removed in 1.19.

// In particular the following updates should be made:

// owned initializer from pointer should accept only unmanaged,
// and not borrow.

// shared initializer from pointer should accept only unmanaged,
// and not borrow.

// Owned synonym for owned should be removed.

// Shared synonym for shared should be removed.

class MyClass { }

proc testOwned() {
  var mine = new owned MyClass();
  var other = new Owned(mine.borrow());
}
testOwned();

proc testShared() {
  var mine = new shared MyClass();
  var other = new Shared(mine.borrow());
}
testShared();
