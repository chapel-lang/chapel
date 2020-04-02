const tup = (false, 1, 2.3, "hi", 4, 5.6, true);

for t in tup {
  writeln();
  writeln("Classifying ", t, ":");
  if (t.type == bool) {
    writeln("bool!");
    continue;
  } else {
    writeln("not bool...");
  }
  if (t.type == int) {
    writeln("int!");
    continue;
  } else {
    writeln("not int...");
  }
  if (t.type == string) {
    writeln("string!");
    continue;
  } else {
    writeln("not string...");
  }
  writeln("something else: ", t.type:string);
}
