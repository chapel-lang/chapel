// This test case shows that there is a memory leak due to the second assignment
// to s.  s should be freed, but only if it was assigned to twice.

config const n = 1;

def main () {
  var s = "Good bye";
  if n > 0 then
    s = "Nos vemos";
  writeln(s);
}
