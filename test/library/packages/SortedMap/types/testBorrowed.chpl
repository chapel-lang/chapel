import MapTest;

class T {
  var value = 0;
}

type t = borrowed T;

MapTest.testMap(t);
