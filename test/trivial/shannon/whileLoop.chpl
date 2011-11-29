var i = 0;

while (i <= 5) {
  writeln("i = ", i);
  i += 1;
}

i = 0;
while (i <= 2) do
  i += 1;
writeln("i = ", i, " (should be 3)");

i =0;
do {
  writeln("i = ", i);
  i += 1;
} while (i <=5);

i = 0;
do
  i +=1;
while (i <= 2);
writeln("i = ", i, " (should be 3)");
