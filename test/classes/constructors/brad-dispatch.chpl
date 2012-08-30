class C {
  var x:int;
  var y:int;
  proc C(a:int,b:int) { x = a; y = b; writeln(this); }
}

writeln("C(1,2);");
var c = new C(1,2);
writeln("C(x=1, y=2);");
delete c; c = new C(x=3, y=4);
writeln("C(a=5,6);");
delete c; c = new C(a=1,2);
writeln("C(7,y=8);");
delete c;
