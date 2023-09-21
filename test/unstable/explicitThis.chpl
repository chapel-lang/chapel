{
  record R {
    proc this() {
      writeln("no argument this");
    }
  }
  var r = new R();
  r.this(); // should warn
  r(); // should not warn
}

{
  record R {
    proc this(x) {
      writeln("argument is ", x);
    }
  }
  var r = new R();
  r.this(7); // should warn
  r("7"); // should not warn
}

{
  record R {
    proc this() {
      writeln("called from method");
    }
    proc foo() {
      this(); // should not warn
      this.this(); // should warn
    }
  }
  var r = new R();
  r.foo();
}

{
  class C {
    proc this() {
      writeln("no argument this");
    }
  }
  var c = new C();
  c.this(); // should warn
  c(); // should not warn
}

{
  class C {
    proc this(x) {
      writeln("argument is ", x);
    }
  }
  var c = new C();
  c.this(7); // should warn
  c("7"); // should not warn
}

{
  class C {
    proc this() {
      writeln("called from method");
    }
    proc foo() {
      this(); // should not warn
      this.this(); // should warn
    }
  }
  var c = new C();
  c.foo();
}
