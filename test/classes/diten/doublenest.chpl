class a {
  proc foo() {
    var bvar = new b();

    bvar.foo();

    delete bvar;
  }

  class b {
    proc foo() {
      var cvar = new c(9);

      writeln("in a.b.foo(), cvar.x is: ", cvar.x);

      delete cvar;
    }

    class c {
      var x: int;
    }
  }
}

proc main() {
  var avar = new a();

  avar.foo();

  delete avar;
}
