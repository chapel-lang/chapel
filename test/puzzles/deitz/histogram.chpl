var n = 12;
var A: [1..n] int = (1,7,2,9,6,7,1,3,7,5,7,9);

var H: [0..9] int;

H(A) += 1;

for i in 0..9 {
  write(i, " ");
  for j in 1..H(i) do
    write("*");
  writeln();
}

writeln();

var m = max reduce H;
for i in 1..m by -1 {
  for j in 0..9 do
    write(if H(j) >= i then "*" else " ");
  writeln();
}
writeln("0123456789");
