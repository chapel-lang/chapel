class C {
  var x : integer = 10;
}

var c : C;

if c != nil then
  writeln(c.x);
else
  writeln("c is nil");

c = C();

if c != nil then
  writeln(c.x);
else
  writeln("c is nil");
