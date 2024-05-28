use GPU;
use GpuDiagnostics;
use ChplConfig;

config const useExpr = true;
config const n = 100;

inline proc doSumReduce(const ref Arr) {
  return if useExpr then + reduce Arr else gpuSumReduce(Arr);
}

inline proc doMinReduce(const ref Arr) {
  return if useExpr then min reduce Arr else gpuMinReduce(Arr);
}

inline proc doMaxReduce(const ref Arr) {
  return if useExpr then max reduce Arr else gpuMaxReduce(Arr);
}

proc testType(type t) {
  proc test(param op: string, type t) {
    on here.gpus[0] {
      var Arr: [0..#n] t;

      foreach i in Arr.domain do Arr[i] = i:t;

      var res;
      select op {
        when "sum" do res=doSumReduce(Arr);
        when "min" do res=doMinReduce(Arr);
        when "max" do res=doMaxReduce(Arr);
        when "minloc" do res=gpuMinLocReduce(Arr);
        when "maxloc" do res=gpuMaxLocReduce(Arr);
      }

      // we cast to `t` to paper over the difference between the integral values
      // returned from two different ways (`gpuSumReduce` and `+ reduce`) of
      // sum-reducing bool arrays
      writeln(op, ": ", if isTuple(res) then res:(t,int) else res:t);
    }
  }

  writeln("Testing type ", t:string);
  test("sum", t);
  test("min", t);
  test("max", t);
  test("minloc", t);
  test("maxloc", t);
  writeln();
}

testType(bool);
testType(int(8));
testType(int(16));
testType(int(32));
testType(int(64));
testType(uint(8));
testType(uint(16));
testType(uint(32));
testType(uint(64));
testType(real(32));
testType(real(64));
