import MapTest;

class T {
  var value = 0;
}

type t = unmanaged T;

// Test that using map.this() / map[i] indexing of a nonnilable type
// generate an error.
MapTest.testMap(t, testIndexing = true);
