

class C { }

record R {
  var sc: shared C?;
}

proc main {
  var r = new R();
}
