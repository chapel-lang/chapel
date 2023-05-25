config const case = 1;

if case == 1 then
  for (i,j) in zip(250:uint(8).., 1..) do
    writeln((i,j));

if case == 2 then
  for (i,j) in zip(250:uint(8).., 1..10) do
    writeln((i,j));

if case == 3 then
  for (i,j) in zip(250:uint(8).., 1..2) do
    writeln((i,j));

