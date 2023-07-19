use IO, Regex;

var r = openReader("test.txt");
const bars = new regex(b"\\|{3,}");

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
