// Can't write to the index variable
// when the loop's iterator is 'const ref'.

var globInt:    int;
var globString: string;
var globRange:  range;
var globDomain: domain(1);
var globArray:  [globDomain] int;

proc mutate(ref obj) { }

iter crefIterG(ref arg) const ref { yield arg; }

iter crefIter1() const ref { yield globInt; }
iter crefIter2() const ref { yield globString; }
iter crefIter3() const ref { yield globRange; }
iter crefIter4() const ref { yield globDomain; }
iter crefIter5() const ref { yield globArray; }

for IDX in crefIterG(globInt) do
  mutate(IDX);  // illegal

for IDX in crefIterG(globString) do
  mutate(IDX);  // illegal

for IDX in crefIterG(globRange) do
  mutate(IDX);  // illegal

for IDX in crefIterG(globDomain) do
  mutate(IDX);  // illegal

for IDX in crefIterG(globArray) do
  mutate(IDX);  // allowed, see #13268

for IDX in crefIter1() do
  mutate(IDX);  // illegal

for IDX in crefIter2() do
  mutate(IDX);  // illegal

for IDX in crefIter3() do
  mutate(IDX);  // illegal

for IDX in crefIter4() do
  mutate(IDX);  // illegal

for IDX in crefIter5() do
  mutate(IDX);  // allowed, see #13268
