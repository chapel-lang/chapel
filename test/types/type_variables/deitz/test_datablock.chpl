class DataBlock {
  type t;
  var x1 : t;
  var x2 : t;
  var x3 : t;
  function this(i : integer) : t {
    return (if i == 1 then x1 else (if i == 2 then x2 else x3));
  }
}

var x : DataBlock = DataBlock(integer);

x.x1 = 1;
x.x2 = 2;
x.x3 = 3;

writeln(x(1));
writeln(x(2));
writeln(x(3));

var y : DataBlock = DataBlock(string);

y.x1 = "hello";
y.x2 = "world";
y.x3 = "!";

writeln(y(1));
writeln(y(2));
writeln(y(3));
