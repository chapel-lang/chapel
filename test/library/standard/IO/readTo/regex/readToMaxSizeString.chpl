use IO, Regex;

var r = openReader("test.txt");
const bars = new regex("\\|{3,}");

// separator doesn't appear in first 6 codepoints, should throw.
try {
  r.readTo(bars, maxSize=6);
} catch e {
  writeln(e);
  write(r.readAll());
}
r.close();

r = openReader("test.txt");
write("before: ", r.readTo(bars, maxSize=10), " after: ", r.readAll());
r.close();

r = openReader("test.txt");
write("before: ", r.readTo(bars, maxSize=15), " after: ", r.readAll());
r.close();
