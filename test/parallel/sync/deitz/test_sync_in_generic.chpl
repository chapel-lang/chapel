use Time;

class C {
  type t;
  var x: t;
}

type t = sync int;

var c = C(t);

begin writeln(c.x);

sleep(2);

c.x = 2;
