class C {
  var x = "hello world";
  proc foo() {
    proc bar(s: string) {
      if s == "0" then
        writeln(x);
      else
        bar(0:string);
    }
    bar(1:string);
  }
}

var c = new borrowed C();
c.foo();
