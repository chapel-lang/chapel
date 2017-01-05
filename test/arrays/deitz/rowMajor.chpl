var D = {1..5, 1..5};
var A: [D] int;

var j = 1;
for i in D {
  A(i) = j;
  j += 1;
}

writeln(A);
writeln(A._value.dataChunk(0)(0));
writeln(A._value.dataChunk(0)(1));
