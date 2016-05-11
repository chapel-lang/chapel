
config const n = 10;
config const verbose = false;

var d:domain(int);

extern proc printf(fmt:c_string, sz:c_int);

on Locales[numLocales-1] {
  for i in 1..n {
    printf("Adding %i\n", i:c_int);
    d += i;
  }

  var gotN = false;
  for i in d {
    if verbose then
      printf("Read %i\n", i:c_int);
    assert( 1 <= i && i <= n);
    if i == n then
      gotN = true;
  }
  assert(gotN);
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

