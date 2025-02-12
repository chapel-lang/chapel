
use Python;
use Time;

config const print = true;
config const time = false;
config const n = 10;
config const runSubInterp = false;

config const lambdaStr = "lambda x,: x + 1 if x % 2 != 0 else x";
proc makeEven(x: int): int do return if x % 2 != 0 then x+1 else x;

//
// Calling a python Lambda function from Chapel serially
//
proc serialPythonApply(interp: borrowed, type t, arr, l) {
  var lambdaFunc = new Python.Function(interp, l);
  var res: [arr.domain] t;
  for i in arr.domain {
    res(i) = lambdaFunc(t, arr(i));
  }
  return res;
}
//
// Calling a python Lambda function from Chapel in parallel
//
proc parallelPythonApply(interp: borrowed, type t, arr, l) {
  var lambdaFunc = new Python.Function(interp, l);
  var res: [arr.domain] t;
  var ts = new threadState();
  ts.save();
  forall i in arr.domain with (var gil = new Python.GIL()) {
    res(i) = lambdaFunc(t, arr(i));
  }
  ts.restore();
  return res;
}
//
// Calling a python Lambda function from Chapel in parallel using SubInterpreter
//
proc parallelPythonApplySubInterp(interp: borrowed, type t, arr, l) {

  record funcPair {
    var interp: owned SubInterpreter?;
    var func: owned Value?;
    proc init(parent: borrowed, s) {
      init this;
      interp = try! (new SubInterpreter(parent));
      func = try! (interp!.compileLambda(s));
    }
    inline proc this(type t, a) throws {
      return func!(t, a);
    }
  }

  var res: [arr.domain] t;
  forall i in arr.domain with (var lambdaFunc = new funcPair(interp, l)) {
    res(i) = lambdaFunc(t, arr(i));
  }
  return res;
}


//
// Calling a Chapel FCP from Chapel serially
//
proc serialChapelApply(type t, arr, f) {
  var res: [arr.domain] t;
  for i in arr.domain {
    res(i) = f(arr(i));
  }
  return res;
}
//
// Calling a Chapel FCP from Chapel in parallel
//
proc parallelChapelApply(type t, arr, f) {
  var res: [arr.domain] t;
  forall i in arr.domain {
    res(i) = f(arr(i));
  }
  return res;
}

proc main() {
  var data: [1..#n] int = 1..#n;
  if print then writeln("data: ", data);


  var interp = new Python.Interpreter();

  {
    data = 1..#n;
    var s = new stopwatch();
    s.start();
    var res = serialPythonApply(interp, int, data, lambdaStr);
    s.stop();
    if time then
      writeln("Elapsed time (Serial Python): ", s.elapsed(), " seconds");
    if print then
      writeln("Serial Python result: ", res);
  }

  {
    data = 1..#n;
    var s = new stopwatch();
    s.start();
    var res = parallelPythonApply(interp, int, data, lambdaStr);
    s.stop();
    if time then
      writeln("Elapsed time (Parallel Python): ", s.elapsed(), " seconds");
    if print then
      writeln("Parallel Python result: ", res);
  }

  if runSubInterp {
    data = 1..#n;
    var s = new stopwatch();
    s.start();
    var res = parallelPythonApplySubInterp(interp, int, data, lambdaStr);
    s.stop();
    if time then
      writeln("Elapsed time (Parallel Python SubInterpreter): ", s.elapsed(), " seconds");
    if print then
      writeln("Parallel Python SubInterpreter result: ", res);
  }

  {
    data = 1..#n;
    var s = new stopwatch();
    s.start();
    var res = serialChapelApply(int, data, makeEven);
    s.stop();
    if time then
      writeln("Elapsed time (Serial Chapel): ", s.elapsed(), " seconds");
    if print then
      writeln("Serial Chapel result: ", res);
  }

  {
    data = 1..#n;
    var s = new stopwatch();
    s.start();
    var res = parallelChapelApply(int, data, makeEven);
    s.stop();
    if time then
      writeln("Elapsed time (Parallel Chapel): ", s.elapsed(), " seconds");
    if print then
      writeln("Parallel Chapel result: ", res);
  }



}
