config const s = "test";

for l in Locales do on l {
  writeln(s);
}

coforall l in Locales do on l {
  writeln(s);
}
