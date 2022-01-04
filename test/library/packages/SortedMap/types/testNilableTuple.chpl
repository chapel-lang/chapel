import MapTest;

class T {
  var value = 0;
}

type t = (shared T?, shared T?);

MapTest.testMap(t);
