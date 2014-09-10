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

var x : DataBlock(int) = new DataBlock(int);

x(1) = 1;
x(2) = 2;
x(3) = 3;

writeln(x(1));
writeln(x(2));
writeln(x(3));

var y : DataBlock(string) = new DataBlock(string);

y(1) = "hello";
y(2) = "world";
y(3) = "!";

writeln(y(1));
writeln(y(2));
writeln(y(3));
