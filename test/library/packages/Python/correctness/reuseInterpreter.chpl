use Python;
use BlockDist;

var interpreters = blockDist.createArray(0..<numLocales, owned Interpreter?);

inline proc getInterpreter(): borrowed Interpreter {
  return interpreters[here.id]!;
}
proc initPythonInterpreters() throws {
  forall interp in interpreters {
    interp = new Interpreter();
  }
}

proc distributedApply(const ref x: [?d] ?t, funcStr: string): [d] t throws {
  var ret = blockDist.createArray(d, t);

  coforall l in d.targetLocales() do on l {
    var func = new Function(getInterpreter(), funcStr);
    for sd in d.localSubdomains() {
      for i in sd {
        ret[i] = func(t, x[i]);
      }
    }
  }
  return ret;
}

config const n = 100;
proc main() {

  initPythonInterpreters();

  var arr = blockDist.createArray(0..<n, int);
  arr = arr.domain;
  writeln("before: ", arr);
  var res = distributedApply(arr, "lambda x,: x + 1");
  writeln("after: ", res);
}
