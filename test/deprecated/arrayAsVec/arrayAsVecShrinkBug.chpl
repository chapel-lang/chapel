config const nTrials = 1;
var A: [1..0] int;

for trial in 1..nTrials {
  for i in 1..1000 {
    A.push_back(i);
  }
  for 1..1000 {
    A.pop_back();
  }
}
