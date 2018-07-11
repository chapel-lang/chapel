use SharedObject;

class C { }

record R {
  var sc: Shared(C);
}

proc main {
  var r = new R();
}
