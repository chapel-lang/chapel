config const n = 100;

on here.gpus[0] var Arr1: [1..n] int;
on here.gpus[0] var Arr2: [1..n] int;

writeln(Arr1.locale);
writeln(Arr2.locale);

Arr2 = 1;

Arr1 = Arr2;  // bulk transfer initiated by the host

writeln(Arr1[42]);
