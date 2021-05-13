var s$: sync bool = true;

iter foo() {
  s$.readFE();
  for i in 1..3 do
    yield i;
  s$.writeEF(true);
}

for i in foo() do
  writeln(i);

for i in foo() do
  writeln(i);

iter bar() {
  s$.readFE();
  var r = 1..3;
  s$.writeEF(true);
  for i in r do
    yield i;
}

for i in bar() do
  writeln(i);

for i in bar() do
  writeln(i);

for i in bar() do
  for j in bar() do
    writeln((i,j));

/* for i in foo() do */
/*   for j in foo() do */
/*     writeln((i,j)); */
