use Set;

var s1 = new set(int);
var s2 = new set(int, parSafe=true);
var s3 = new set(int, parSafe=false);

class MyClass {
  var i: int;
}

var s4 = new set(shared MyClass);
var s5 = new set(shared MyClass, parSafe=true);
var s6 = new set(shared MyClass, parSafe=false);
