class C {
  var whoami = "local";
}
var globalC = new shared C("global");

record rec {

}

proc rec.doSomething(a: int = 0, b: shared C = globalC) {
  writeln(a, " ", b);
}

var r: rec;
r.doSomething();
r.doSomething(1);
r.doSomething(1, new shared C());
r.doSomething(b=new shared C());

proc f() {
  var r: rec;
  r.doSomething();
  r.doSomething(1);
  r.doSomething(1, new shared C());
  r.doSomething(b=new shared C());
}
f();
