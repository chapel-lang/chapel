const tup = (false, 1, 2.3, "hi", 4, 5.6, true);

for t in tup {
  writeln();
  writeln("Classifying ", t, ":");
  if (t.type == string) {
    writeln("Ack, found string!");
    break;
  }
  writeln("it's a(n): ", t.type:string);
}
