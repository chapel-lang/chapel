config const n = 6;
config const printOutput=false;

var A: [1..n] int = 777;
var B: [1..n] int = -1;
assert(A._value.oneDData); // fend off multi-ddata
var Ad = A._value.theDataChunk(0);
assert(B._value.oneDData); // fend off multi-ddata
var Bd = B._value.theDataChunk(0);
__primitive("chpl_comm_array_get",
            __primitive("array_get", Bd,
                        B._value.getDataIndex(1, getChunked=false)),
            0,
            __primitive("array_get", Ad,
                        A._value.getDataIndex(1, getChunked=false)),
            n);
if printOutput then writeln(B);
for i in B.domain do
  if A(i) != B(i) then
    writeln("Error: B(", i, ")=", B(i));

on Locales(numLocales-1) {
  var C: [1..n] int = -1;
  assert(C._value.oneDData); // fend off multi-ddata
  var Cd = C._value.theDataChunk(0);
  __primitive("chpl_comm_array_get",
              __primitive("array_get", Cd,
                          C._value.getDataIndex(1, getChunked=false)),
              0,
              __primitive("array_get", Ad,
                          A._value.getDataIndex(1, getChunked=false)),
              n);
  if printOutput then writeln(C);
  for i in C.domain do
    if A(i) != C(i) then
      writeln("Error: C(", i, ")=", C(i));

}

on Locales(numLocales-1) {
  var D: [1..n] int = 777;
  assert(D._value.oneDData); // fend off multi-ddata
  var Dd = D._value.theDataChunk(0);
  on Locales(0) {
    var E: [1..n] int = -1;
    assert(E._value.oneDData); // fend off multi-ddata
    var Ed = E._value.theDataChunk(0);
    __primitive("chpl_comm_array_get",
                __primitive("array_get", Ed,
                            E._value.getDataIndex(1, getChunked=false)),
                numLocales-1,
                __primitive("array_get", Dd,
                            D._value.getDataIndex(1, getChunked=false)),
                n);
    if printOutput then writeln(E);
    for i in E.domain do
      if D(i) != E(i) then
        writeln("Error: E(", i, ")=", E(i));
  }
}
