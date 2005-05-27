class DataBlock {
  type t;
  var x1 : t;
  var x2 : t;
  var x3 : t;
  function this(i : integer) : t {
    if i == 1 then
      return x1;
    else if i == 2 then
      return x2;
    else
      return x3;
  }
  function =this(i : integer, val : t) {
    if i == 1 then
      x1 = val;
    else if i == 2 then
      x2 = val;
    else
      x3 = val;
  }
}

var x : DataBlock = DataBlock(integer);

x(1) = 1;
x(2) = 2;
x(3) = 3;

writeln(x(1));
writeln(x(2));
writeln(x(3));

var y : DataBlock = DataBlock(string);

y(1) = "hello";
y(2) = "world";
y(3) = "!";

writeln(y(1));
writeln(y(2));
writeln(y(3));
