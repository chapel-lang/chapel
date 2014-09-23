use Time;

class C {
  type t;
  var x: t;
}

type t = sync int;

var c = new C(t);

begin {
  writeln(c.x.readFE());
  delete c;
}

sleep(2);

c.x = 2;
