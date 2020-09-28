use CPtr;
class A {
  proc fn() : c_ptr(int) {
    return c_nil : c_ptr(int);
  }
}

// A simpler way to expose the issue.
proc fnSimple(arg) return 5;

proc main() {
  var a = new borrowed A();
  on Locales[0] {
    forall 1..1 {
      var ptr = a.fn();
      var ptrSimple = fnSimple(a);
      writeln(ptr == c_nil, ptrSimple);
    }
  }
}
