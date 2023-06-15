// make sure the inner y is used

const y = 100;
var x = 1;
do {
  const y = x + 1;
  x += 1;
  writeln((x,y));
} while y < 10;
