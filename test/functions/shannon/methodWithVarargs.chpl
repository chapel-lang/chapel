class myClass {
  var x: integer;

}

function myClass.setMember(a: integer ...?rank) {
  var mc : myClass = myClass();
  for i in 1..rank do
    mc.x = a(i);

  return mc;
}

var a: myClass = myClass();
a.setMember(1, 2, 3, 4, 5);

writeln(a);
