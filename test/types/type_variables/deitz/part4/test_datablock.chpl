class DataBlock {
  type t;
  var x1 : t;
  var x2 : t;
  var x3 : t;
  proc this(i : int) : t {
    return (if i == 1 then x1 else (if i == 2 then x2 else x3));
  }
}

var x : borrowed DataBlock(int) = new borrowed DataBlock(int);

x.x1 = 1;
x.x2 = 2;
x.x3 = 3;

writeln(x(1));
writeln(x(2));
writeln(x(3));

var y : borrowed DataBlock(string) = new borrowed DataBlock(string);

y.x1 = "hello";
y.x2 = "world";
y.x3 = "!";

writeln(y(1));
writeln(y(2));
writeln(y(3));
