class a {
  def foo() {
    var bvar = new b();
    bvar.foo();
  }
  class b {
    def foo() {
      var cvar = new c(9);
      writeln("in a.b.foo(), cvar.x is: ", cvar.x);
    }
    class c {
      var x: int;
    }
  }
}

def main() {
  var avar = new a();
  avar.foo();
}
