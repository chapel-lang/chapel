var D = {1..0};
var A: [D] int;

config const numPushes = 1000;
config const numIters = 10;

var sum = 0;

for it in 1..numIters {
  for i in 1..numPushes {
    A.push_back(i);
  }
  for i in 1..numPushes {
    A.push_front(-i);
  }
  while !A.isEmpty() {
    sum += A.tail();
    A.pop_back();
  }
}
writeln(sum);
