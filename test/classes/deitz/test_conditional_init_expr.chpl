class C {
  var x: int;
  var y: int = if x < 3 then 0 else 1;
}

var c1 = new C();
var c2 = new C(6);
writeln((c1, c2));
delete c1;
delete c2;
