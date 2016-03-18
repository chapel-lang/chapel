var R = max(int)-10..max(int);
writeln(R);
var count = 0;
for i in R {
  count += 1;
  write(" ", i);
  if (count > 11) then
    halt("expected number of iterations exceeded");
}
writeln();
