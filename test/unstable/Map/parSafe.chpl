use Map;

var m1 = new map(int, int);
var m2 = new map(int, int, parSafe=true);
var m3 = new map(int, int, parSafe=false);

class MyClass {
  var i: int;
}

var m4 = new map(int, shared MyClass);
var m5 = new map(int, shared MyClass, parSafe=true);
var m6 = new map(int, shared MyClass, parSafe=false);
