var A: [i in 1..3] real = i;
var B: [i in 1..4] real = i;

class C {
  var x = A;
}

var c = new C();
writeln("c is: ", c);

var c2 = new C(B);
writeln("c2 is: ", c2);
