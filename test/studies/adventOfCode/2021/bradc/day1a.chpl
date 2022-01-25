use IO;

var vals: [0..2] int;
read(vals);
var prevsum = + reduce vals;
var replace = 0;
var bigger = 0;
do {
  var depth: int;
  const success = read(vals[replace]);
  replace += 1;
  replace %= 3;
  var newsum = + reduce vals;
  if newsum > prevsum then
    bigger += 1;
  prevsum = newsum;
} while (success);
writeln(bigger);

