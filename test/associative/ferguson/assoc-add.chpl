config const n = 10000;
config const checkMemberOnAdd = false;

var d:domain(int);

on Locales[numLocales-1] {
  forall i in 1..n with (ref d) {
    d += i;
    if checkMemberOnAdd then
      assert(d.member(i));
  }
}

writeln(d.size);

for i in 1..n {
  if !d.member(i) then
    writeln("Error: d does not contain ", i);
}

var i = 1;
for j in d.sorted() {
  assert(j == i);
  i += 1;
}

