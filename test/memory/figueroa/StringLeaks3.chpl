// This test case shows that there is a memory leak due to the second assignment
// to s.  s should be freed just before it is assigned to again.

config const n = 1;

def main () {
  var s1 = "Good bye",
      s2 = s1;
  if n > 0 then
    s2 = "Nos vemos";
  writeln(s2);
}
