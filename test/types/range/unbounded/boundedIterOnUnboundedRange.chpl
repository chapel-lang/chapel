config const case = 1;

iter range.yieldThree() {
  yield 1;
  yield 2;
  yield 3;
}

var r = 1..;

if case == 1 then
  for (i,j) in zip(r.yieldThree(), 1..4) do
    writeln((i,j));

if case == 2 then
  for (i,j) in zip(r.yieldThree(), 1..2) do
    writeln((i,j));

if case == 3 then
  for (i,j) in zip(1..4, r.yieldThree()) do
    writeln((i,j));

if case == 4 then
  for (i,j) in zip(1..2, r.yieldThree()) do
    writeln((i,j));

