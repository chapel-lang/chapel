use iters;
config const myVerbose = false;

for t in zip(i0(), i1()) do if myVerbose then write(t, " ");
if initial != off then halt("initial incorrect (", initial, ")");
if myVerbose then writeln();
for t in zip(i0(), i1(), i2()) do if myVerbose then write(t, " ");
if initial != 3*off then halt("initial incorrect (", initial, ")");
if myVerbose then writeln();
for t in zip(i0(), i1(), i2(), i3()) do if myVerbose then write(t, " ");
if initial != 6*off then halt("initial incorrect (", initial, ")");
if myVerbose then writeln();
for t in zip(i0(), i1(), i2(), i3(), i4()) do if myVerbose then write(t, " ");
if myVerbose then writeln();
if initial != 4 then halt("initial incorrect (", initial, ")");

