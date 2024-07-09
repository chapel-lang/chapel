class DataBlock {
  type t;
  var x1 : t;
  var x2 : t;
  var x3 : t;
  proc this(i : int) : t {
    return (if i == 1 then x1 else (if i == 2 then x2 else x3));
  }
}

var ownX = new owned DataBlock(int);
var x : borrowed DataBlock(int) = ownX.borrow();

x.x1 = 1;
x.x2 = 2;
x.x3 = 3;

writeln(x(1));
writeln(x(2));
writeln(x(3));

var ownY = new owned DataBlock(string);
var y : borrowed DataBlock(string) = ownY.borrow();

y.x1 = "hello";
y.x2 = "world";
y.x3 = "!";

writeln(y(1));
writeln(y(2));
writeln(y(3));
