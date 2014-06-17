var A: [1..3] real = [i in 1..3] i;
var B: [1..4] real = [i in 1..4] i;

class C {
  var x = A;
}

var c = new C();
writeln("c is: ", c);

var c2 = new C(B);
writeln("c2 is: ", c2);
