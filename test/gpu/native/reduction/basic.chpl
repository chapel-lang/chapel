use GPU;
use ChplConfig;

config const n = 100;

proc testType(type t) {
  proc test(param op: string, type t) {
    on here.gpus[0] {
      var Arr: [0..#n] t;

      foreach i in Arr.domain do Arr[i] = i:t;

      var res;
      select op {
        when "sum" do res=gpuSumReduce(Arr);
        when "min" do res=gpuMinReduce(Arr);
        when "max" do res=gpuMaxReduce(Arr);
        when "minloc" do res=gpuMinLocReduce(Arr);
        when "maxloc" do res=gpuMaxLocReduce(Arr);
      }

      writeln(op, ": ", res);
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
