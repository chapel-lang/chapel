const b1 = b"foo";
const b2 = b"bar";

config var b = b1;

for l in Locales do on l {
  select b {
    when b1 do
      writeln("b1");
    when b2 do
      writeln("b2");
  }
}

coforall l in Locales do on l {
  select b {
    when b1 do
      writeln("b1");
    when b2 do
      writeln("b2");
  }
}
