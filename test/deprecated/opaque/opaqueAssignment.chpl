var dom1: domain(opaque);
var dom2: domain(opaque);

var newIdx = dom2.create();

dom1 = dom2;
var arr1: [dom1] int;
var arr2: [dom2] int;
writeln(arr1(newIdx));
writeln(arr2(newIdx));
