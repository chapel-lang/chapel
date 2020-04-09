config const n = 6;
config const printOutput=false;

var A: [1..n] int = 777;
var B: [1..n] int = -1;
var Ad = A._value.theData;
var Bd = B._value.theData;
__primitive("chpl_comm_array_get",
            __primitive("array_get", Bd,
                        B._value.getDataIndex(1)),
            0,
            __primitive("array_get", Ad,
                        A._value.getDataIndex(1)),
            n);
if printOutput then writeln(B);
for i in B.domain do
  if A(i) != B(i) then
    writeln("Error: B(", i, ")=", B(i));

on Locales(numLocales-1) {
  var C: [1..n] int = -1;
  var Cd = C._value.theData;
  __primitive("chpl_comm_array_get",
              __primitive("array_get", Cd,
                          C._value.getDataIndex(1)),
              0,
              __primitive("array_get", Ad,
                          A._value.getDataIndex(1)),
              n);
  if printOutput then writeln(C);
  for i in C.domain do
    if A(i) != C(i) then
      writeln("Error: C(", i, ")=", C(i));

}

on Locales(numLocales-1) {
  var D: [1..n] int = 777;
  var Dd = D._value.theData;
  on Locales(0) {
    var E: [1..n] int = -1;
    var Ed = E._value.theData;
    __primitive("chpl_comm_array_get",
                __primitive("array_get", Ed,
                            E._value.getDataIndex(1)),
                numLocales-1,
                __primitive("array_get", Dd,
                            D._value.getDataIndex(1)),
                n);
    if printOutput then writeln(E);
    for i in E.domain do
      if D(i) != E(i) then
        writeln("Error: E(", i, ")=", E(i));
  }
}
