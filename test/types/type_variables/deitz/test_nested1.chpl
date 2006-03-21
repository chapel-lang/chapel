class C {
  type t;
  var x = 1;
  var y : t;
  fun foo() {
    fun bar(_x) {
      writeln("default bar");
    }
    fun bar(_x : string) {
      writeln("bar of string");
    }
    bar(x);
    bar("hello world");
  }
}

var c = C(int);
c.foo();
writeln(c);
