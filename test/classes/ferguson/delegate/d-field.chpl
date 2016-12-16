
record Wrapper {
  var instance; // e.g. some class
  delegate instance;
}

class C {
  var field:int;
}

var r = new Wrapper(new C());
r.field = 11;
writeln(r.field);


