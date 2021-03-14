use iters;
config const myVerbose = false;

for t in zip(i0(), i1()) do if myVerbose then write(t, " ");
if init != off then halt("init incorrect (", init, ")");
if myVerbose then writeln();
for t in zip(i0(), i1(), i2()) do if myVerbose then write(t, " ");
if init != 3*off then halt("init incorrect (", init, ")");
if myVerbose then writeln();
for t in zip(i0(), i1(), i2(), i3()) do if myVerbose then write(t, " ");
if init != 6*off then halt("init incorrect (", init, ")");
if myVerbose then writeln();
for t in zip(i0(), i1(), i2(), i3(), i4()) do if myVerbose then write(t, " ");
if myVerbose then writeln();
if init != 4 then halt("init incorrect (", init, ")");

