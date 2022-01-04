// Error: condition on an int

var nums = [2, 5, 0, 3];

var i = 0;
while const n = nums[i] do   // error
  writeln(n);

writeln("done");
