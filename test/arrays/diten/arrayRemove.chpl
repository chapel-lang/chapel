var A: [1..10] int = [i in 1..10] i;

writeln(A);
while !A.isEmpty() {
  A.remove(1);
  writeln(A);
}

for i in 1..10 do A.push_back(i);

writeln(A);
while !A.isEmpty() {
  A.remove(A.domain.high);
  writeln(A);
}

for i in 1..10 do A.push_back(i);
A.remove(4..7);
writeln(A);
