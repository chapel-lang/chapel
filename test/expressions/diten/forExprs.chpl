config param useTensor = false;

var nums = [1,2,3,4];
var fruit = ["Apples", "Peaches", "Pears", "Bananas"];

writeln([i in 0..#10] i);
writeln([i in 0..#20] if i%2 == 0 then i);

if useTensor {
  iter tensor(A, B...) {
    for a in A do
      for b in tensor((...B)) do
        yield (a, (... b));
  }

  iter tensor(A) {
    for a in A do
      yield (a,);
  }

  writeln(for i in tensor(nums,fruit) do i);
  writeln(for i in tensor(nums,fruit) do if i(2)[1] == "P" then i);
  writeln(for i in tensor(nums,fruit) do if i(2)[1] == "P" && i(1)%2 == 1 then i);
  writeln(for i in zip(nums, fruit) do if i(1)%2 == 0 then i);
} else {
  writeln(for i in nums do for f in fruit do (i,f));
  writeln(for i in nums do for f in fruit do if f[1] == "P" then (i,f));
  writeln(for i in nums do for f in fruit do if f[1] == "P" && i%2 == 1 then (i,f));
  writeln(for i in zip(nums, fruit) do if i(1)%2 == 0 then i);
}
