
param dim=3;

class C {
  var x:dim*int;
}

proc run(c:C) {
  for i in 1..dim do
    c.x(i) = 1;
}

var c = new C();
run(c);
writeln(c);
delete c;
