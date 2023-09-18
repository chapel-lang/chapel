class DataBlock {
  type t;
  var x1 : t;
  var x2 : t;
  var x3 : t;
  proc this(i : int) ref : t {
    if i == 1 then
      return x1;
    else if i == 2 then
      return x2;
    else
      return x3;
  }
}

var ownX = new owned DataBlock(int);
var x : borrowed DataBlock(int) = ownX.borrow();

x(1) = 1;
x(2) = 2;
x(3) = 3;

writeln(x(1));
writeln(x(2));
writeln(x(3));

var ownY = new owned DataBlock(string);
var y : borrowed DataBlock(string) = ownY.borrow();

y(1) = "hello";
y(2) = "world";
y(3) = "!";

writeln(y(1));
writeln(y(2));
writeln(y(3));
