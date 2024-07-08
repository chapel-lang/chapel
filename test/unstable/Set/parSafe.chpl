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

record Values {
    iter these() {
        for i in 0..10 do
            yield i;
    }
}

var v = new Values();

var s7 = new set(int, v, parSafe=true);
var s8 = new set(int, v, parSafe=false);
writeln(s7.parSafe == true);

var s9: set(int);
var s10: set(int, parSafe=true);
var s11: set(int, false);

type a = set(int);
type b = set(int, parSafe=true);
