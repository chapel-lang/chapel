
param dim=3;

class C {
  var x:dim*int;
}

proc run(c:borrowed C) {
  for i in 0..#dim do
    c.x(i) = 1;
}

var c = new unmanaged C();
run(c);
writeln(c);
delete c;
