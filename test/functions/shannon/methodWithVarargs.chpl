class myClass {
  var x: int;

}

function myClass.setMember(a: int ...?rank) {
  var mc : myClass = myClass();
  for i in 1..rank do
    mc.x = a(i);

  return mc;
}

var a: myClass = myClass();
a = a.setMember(1, 2, 3, 4, 5);

writeln(a);
