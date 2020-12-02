// this is a test case for issue #15628
// based on test/arrays/intents/in/default-value-linkage.chpl

proc builder(D : domain(1)) {
  var R : [D] int;
  return R;
}

var DD = {1..4};
var AA : [DD] int;
defaultsValue(A=AA);

proc defaultsValue(in D = {1..4}, in A = builder(D)) {
  writeln("A.domain is ", A.domain);
  writeln("Modifying D");
  D = {1..5};
  writeln("A.domain is ", A.domain);
  D = {1..4}; // putting it back
  writeln("Modifying DD");
  DD = {1..6};
  writeln("A.domain is ", A.domain);
  DD = {1..4}; // putting it back
}
