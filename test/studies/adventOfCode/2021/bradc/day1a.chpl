use IO;

var vals: [0..2] int;
read(vals);
var prevsum = + reduce vals;
var replace = 0;
var bigger = 0;
while read(vals[replace]) {
  replace += 1;
  replace %= 3;
  var newsum = + reduce vals;
  if newsum > prevsum then
    bigger += 1;
  prevsum = newsum;
}
writeln(bigger);

