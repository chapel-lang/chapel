var i = 0, j = 0, k = 0;

label lab1 
while (i <= 3 && k < 10) {
  k += 1;
  j = 0;
  label lab2 
  while (j <= 3) {
    writeln("i = ", i, " j = ", j);
    j += 1;
    if (j > 1) then break lab1;
  }  
  i += 1;
}
