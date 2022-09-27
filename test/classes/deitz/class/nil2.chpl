class C {
  var x : int = 10;
}

var c : borrowed C?;

if c != nil then
  writeln(c!.x);
else
  writeln("c is nil");

var ownedTmp = new owned C();
c = ownedTmp.borrow();

if c != nil then
  writeln(c!.x);
else
  writeln("c is nil");
