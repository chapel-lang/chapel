config const n = 0;
var r = 1..n;

proc makeArray(i:int) {
  var A:[1..i] int;
  A[1] = i;
  return A;
}

// AA is an array of arrays
var AA = for i in r do makeArray(i);
writeln(AA);

writeln("AA domain ", AA.domain);
var element:AA.eltType;
writeln("AA (element) eltType domain ", element.domain);

// same, except iterate over an iterator

iter ITR() { if numLocales < 0 then yield 555; }

var BB = for idx in ITR() do [idx];
compilerWarning(BB.type:string);
writeln(BB.domain);
