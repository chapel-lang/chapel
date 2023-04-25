//
// This test expresses different closure patterns that we might like to
// support. None of these currently work, as first-class-procedures cannot
// refer to outer variables today.
//

proc test0() {
  type T = int;
  type P = proc(_: T): T;
  const compose = proc(f: P, g: P): P {
    var p = proc(x: T): T { return g(f(x)); };
    var ret = p:P;
    return ret;
  };

  const inc = proc(x: int) { return x + 1; };
  const inc2 = compose(inc, inc);
  writeln(inc2(0));
}

proc main() {
  const tests = [test0];
  for test in tests {
    writeln("--- ", test:string, " ---");
    test();
  }
}

