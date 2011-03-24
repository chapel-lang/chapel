var x: int;

proc main() {
  coforall i in LocaleSpace do
    atomic foo(i);
  if (x == numLocales) then writeln("SUCCESS");
}

proc foo(i) {
  on x.locale do atomic x = x + 1;
}