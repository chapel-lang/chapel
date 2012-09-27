var x: real;

writeln("x is stored at: ", chpldev_refToString(x));

var A: [1..3] real = (1.1, 2.2, 3.3);
var str1: [1..3] string;
var str2: [1..3] string;

for (a,s) in zip(A,str1) {
  s = chpldev_refToString(a);
}

for i in 1..3 {
  str2(i) = chpldev_refToString(A(i));
}

writeln("str1 = ", str1);
writeln("str2 = ", str2);

for i in 1..3 do
  if (str1(i) != str2(i)) then
    writeln("mismatch at position ", i);
