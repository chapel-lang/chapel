config const n = 2;
var r = 1..n;

proc makeArray(D) {
  var A:[D] int;
  A[1] = 1;
  return A;
}

var D = {1..10};
writeln("D is ", D);
// AA is an array of arrays
var AA = for i in r do makeArray(D);

for subarray in AA {
  writeln("subarray ", subarray);
  writeln("subarray domain ", subarray.domain);
  var B:subarray.type;
  writeln("subarray (B) type domain ", B.domain);
}

{
  writeln("AA domain ", AA.domain);
  var element:AA.eltType;
  writeln("AA (element) eltType domain ", element.domain);
}

writeln("Reassigning D");
D = {1..3};
writeln("D is ", D);

for subarray in AA {
  writeln("subarray ", subarray);
  writeln("subarray domain ", subarray.domain);
  var B:subarray.type;
  writeln("subarray (B) type domain ", B.domain);
}

{
  writeln("AA domain ", AA.domain);
  var element:AA.eltType;
  writeln("AA (element) eltType domain ", element.domain);
}
