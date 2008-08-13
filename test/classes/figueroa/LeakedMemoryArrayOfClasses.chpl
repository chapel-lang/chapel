use Memory;

class C {
  var x: int;
}

var m1 = memoryUsed();
var c: [i in 1..10] C = new C();
delete c;
var m2 = memoryUsed();
writeln("Amount of leaked memory after deleting array c: ", m2-m1);

m1 = memoryUsed();
c = [i in c.domain] new C();
delete [i in c.domain] c[i];
m2 = memoryUsed();
writeln("Amount of leaked memory after deleting individual elements of array c: ", m2-m1);
