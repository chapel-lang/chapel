var i = 0;

label lab1 
while (i <= 10) {
  writeln("i = ", i);
  i += 1;
  if (i > 5) then break lab1;
}
