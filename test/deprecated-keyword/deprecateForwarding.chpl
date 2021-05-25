// Trying to deprecate a forwarding statement shouldn't be allowed
class C {
  var x: Other;

  deprecated "don't forward any more" forwarding x;
}

record Other {
  proc foo() {
    writeln("In Other.foo()");
  }
}

proc main() {
  var c = new C(new Other());
  c.foo();
}
