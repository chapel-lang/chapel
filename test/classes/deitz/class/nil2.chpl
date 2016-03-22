class C {
  var x : int = 10;
}

var c : C;

if c != nil then
  writeln(c.x);
else
  writeln("c is nil");

c = new C();

if c != nil then
  writeln(c.x);
else
  writeln("c is nil");

delete c;
