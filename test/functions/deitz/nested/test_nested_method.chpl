class C {
  var x = "hello world";
  def foo() {
    def bar(s: string) {
      if s == "0" then
        writeln(x);
      else
        bar(0);
    }
    bar(1);
  }
}

var c = new C();
c.foo();
