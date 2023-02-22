import MapTest;

class T {
  var value = 0;
}

type t = (shared T, shared T);

// Test that indexing fails: Cannot assign to const variable
MapTest.testMap(t, testIndexing = true);
