record callable {
  proc this() {
    writeln("I'm a callable thingie!");
  }
}

record R {
  var myCallable_: callable;

  proc fn do return myCallable_;
  proc ref fn ref do return myCallable_;
}
var r = new R();
r.fn();
