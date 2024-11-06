use Python;
use CTypes only c_ptr, c_long;
use Time;

config const n = 10;
config const print = true;
config const timeit = false;

// gadget to call a c_ptr, since Chapel can't call c_fn_ptr's directly
require "callFuncGadget.h";
extern proc callFunc(func: c_ptr(void), x: c_long): c_long;

proc callApply(arr, func: borrowed Function) {
  var res: arr.type;
  for i in arr.domain {
    res(i) = func(res.eltType, arr(i));
  }
  return res;
}

proc callApplyByAddress(arr, func: c_ptr(void)) {
  // ideally we can cast the c_ptr to a chapel FCP, but that doesn't work yet
  // the work around is the 'callFunc' gadget
  // var chplFunc = func : (proc(_: int): int);
  var res: arr.type;
  for i in arr.domain {
    res(i) = callFunc(func, arr(i));
  }
  return res;
}

proc main() {
  var data: [1..#n] int;

  var interp = new Interpreter();

  // Ideally we could just write the function in a Chapel string and compile it,
  // but thats not possible yet
  var lib = new Module(interp, "lib");
  var applyFunc = new Function(lib, "apply");
  var applyFuncAddr = applyFunc.getAttr(int, "address"): c_ptr(void);

  {
    data = 1..#n;
    var t = new stopwatch();
    t.start();
    var res = callApply(data, applyFunc);
    t.stop();
    if timeit then
      writeln("calling the numba func directly took ", t.elapsed(), " seconds");
    if print then
      writeln("calling the numba func directly: ", res);
  }

  {
    data = 1..#n;
    var t = new stopwatch();
    t.start();
    var res = callApplyByAddress(data, applyFuncAddr);
    t.stop();
    if timeit then
      writeln("calling the numba func by address took ", t.elapsed(), " seconds");
    if print then
      writeln("calling the numba func by address: ", res);
  }


}
