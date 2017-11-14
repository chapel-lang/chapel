// This test began life as a bug report
// from a branch using forwarding for _array._value.

class C {
  proc helper(x) {
    writeln("HI ", x.type:string);
  }
}

class Wrapper {
  forwarding var inner;
}

var b = new C();
b.helper(42);
var f = new Wrapper(b);
f.helper(b);
var ff = new Wrapper(f);
ff.helper(f);

delete ff;
delete f;
delete b;
