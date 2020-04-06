// Simple case ///////////////////////////
writeln("x y");
var x = 5;
ref y = x;
writeln(x, ' ', y);
x += 1;
writeln(x, ' ', y);
y += 1;
writeln(x, ' ', y, '\n');

// Array element modification ////////////
var A = [1, 2, 3, 4, 5];
writeln("A  : ", A);
for i in A.domain {
  ref element = A[i];
  element += 1;
}
writeln("A' : ", A, '\n');


// Array of Array element modification ///
var B = [[1, 2], [3, 4]];
writeln("B  : ", B);
for i in B.domain {
  for j in B[1].domain {
    ref element = B[i][j];
    element += 1;
  }
}
writeln("B' : ", B, '\n');


// Records ///////////////////////////////
record Point {
  var x: real;
  var y: real;
}

record Rect {
  var upLeft: Point;
  var botRight: Point;
}

var rectangles = [
  new Rect(new Point(1.0, 1.0), new Point(4.0, 5.0)),
  new Rect(new Point(2.0, 3.0), new Point(6.0, 7.0))
];

writeln("rectangles:");
for r in rectangles do
  writeln(r);

for r in rectangles {
  ref upLeft = r.upLeft;
  ref botRight = r.botRight;
  upLeft.x += 1.0;
  botRight.y -= 1.0;
}

writeln('\n', "rectangles':");
for r in rectangles do
  writeln(r);

// Class ////////////////////////////////
class Foo {
  var x = 1.0;

  proc doubleMe() {
    x *= 2;
  }
}

var myFoo = new borrowed Foo();
ref myrefFoo = myFoo;
writeln('\n', "myFoo     myrefFoo");
writeln(myFoo, ' ', myrefFoo);
myFoo.doubleMe();
writeln(myFoo, ' ', myrefFoo);
myrefFoo.doubleMe();
writeln(myFoo, ' ', myrefFoo);



// Ref Functions ////////////////////////
proc testRefReturn(ref x: int) ref {
  ref y = x;
  return y;
}
var fx = 10;
ref fy = testRefReturn(fx);
writeln('\n', "fx fy");
writeln(fx, ' ', fy);
fx += 5;
fy += 1;
writeln(fx, ' ', fy);

