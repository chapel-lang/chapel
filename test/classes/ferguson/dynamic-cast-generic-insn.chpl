class Parent {
  var x:int;
}

class Child : Parent {
  var y: int;
}

class Grandchild : Child {
  var z: int;
  param p;
}

config param n = 10;
var A:[1..n] unmanaged Parent?;
for param i in 1..n {
  A[i] = new unmanaged Grandchild(p=i, x=i, y=i, z=i);
}

var first:borrowed Parent = A[1]!;
writeln("Attempting dynamic cast to Child");
var c = first:borrowed Child;
writeln(c);
writeln("Attempting dynamic cast to Grandchild");
var gc = first:borrowed Grandchild(1);
writeln(gc);

for a in A do delete a;
