var t = 11;

writeln("Scope of do while loop:");
do {
  t += 1;
  writeln(t);
} while (t <= 10);

t = 11;
writeln("Scope of while loop:");
while (t <= 10) {
  t += 1;
  writeln(t);
}
