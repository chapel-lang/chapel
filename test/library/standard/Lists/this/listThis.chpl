use Lists;

type listType = int;
param listLock = true;

var lst = new list(listType, listLock);

for i in 1..8 do
  lst.append(i);

writeln(lst);

for i in lst do
  writeln(i);

writeln(lst);

for i in lst do
  i *= 2;

writeln(lst);

for i in 1..lst.size by -1 do
  lst[i] /= 2;

writeln(lst);

// Perform an in place reversal of the list.
for i in 1..(lst.size / 2) do {
  ref a = lst[lst.size - i + 1];
  ref b = lst[i];
  //
  // Don't rely on the swap operator!
  // See: https://github.com/chapel-lang/chapel/issues/8545
  //
  var t = a;
  a = b;
  b = t;
}

writeln(lst);
