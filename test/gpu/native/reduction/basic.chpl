
inline proc chpl_doGpuReduce(param op: string, ref A: [] ?t) {
  proc chplTypeToCTypeName(type t) param {
    select t {
      when int(8)   do return "int8_t";
      when int(16)  do return "int16_t";
      when int(32)  do return "int32_t";
      when int(64)  do return "int64_t";
      when uint(8)  do return "uint8_t";
      when uint(16) do return "uint16_t";
      when uint(32) do return "uint32_t";
      when uint(64) do return "uint64_t";
      when real(32) do return "float";
      when real(64) do return "double";
    }
    return "unknown";
  }

  proc getExternFuncName(param op: string, type t) param: string {
    return "chpl_gpu_"+op+"_reduce_"+chplTypeToCTypeName(t);
  }

  use CTypes;

  param externFunc = getExternFuncName(op, t);

  if op == "sum" || op == "min" || op == "max" {
    var val: t;
    extern externFunc proc reduce_fn(data, size, ref val);
    reduce_fn(c_ptrTo(A), A.size, val);
    return val;
  }
  else {
    var idx: int(32);
    var val: t;
    extern externFunc proc reduce_fn(data, size, ref val, ref idx);
    reduce_fn(c_ptrTo(A), A.size, val, idx);
    return (idx, val);
  }
}

inline proc gpuSumReduce(ref A: [] ?t): t { return chpl_doGpuReduce("sum", A); }
inline proc gpuMinReduce(ref A: [] ?t): t { return chpl_doGpuReduce("min", A); }
inline proc gpuMaxReduce(ref A: [] ?t): t { return chpl_doGpuReduce("max", A); }
inline proc gpuMinLocReduce(ref A: [] ?t) { return chpl_doGpuReduce("minloc", A); }
inline proc gpuMaxLocReduce(ref A: [] ?t) { return chpl_doGpuReduce("maxloc", A); }

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
