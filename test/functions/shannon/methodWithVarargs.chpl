class myClass {
  var x: int;
}

proc myClass.setMember(a: int ...?rank) {
  var mc : unmanaged myClass = new unmanaged myClass();

  for i in 0..rank-1 do
    mc.x = a(i);

  return mc;
}

var a: unmanaged myClass = new unmanaged myClass();
var t          = a;

a = a.setMember(1, 2, 3, 4, 5);

writeln(a);

delete a;
delete t;

