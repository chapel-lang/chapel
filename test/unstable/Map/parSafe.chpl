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

var m7 = new map(string,int,parSafe=true);
var m8 = new map(string,int,parSafe=false);

m7.addOrReplace("a",1);
m8["a"] = 1;
writeln(m7.parSafe == true);

var m9: map(int, int);
var m10: map(int, int, true);
var m11: map(int, int, parSafe=false);

type a = map(int, int);
type b = map(int, int, false);
