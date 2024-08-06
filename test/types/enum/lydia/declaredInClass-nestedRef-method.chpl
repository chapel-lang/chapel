// According to the spec, it is legal to define an enum type within a class.
// However, we seem to have issues with accessing its constant values in
// practice.

proc main() {
  var sampleBug = new Sample();
  writeln(sampleBug.tag);
}


class Sample {

  proc doSomething() do return 10;

  enum inner {
    red = doSomething(), green, blue
  }

  // give it a value.. still error
  var tag: inner = inner.red;
}
