use iters;
config const myVerbose = false;

for i in i0() do if myVerbose then write(i, " ");
if init != 0 then halt("init incorrect (", init, ")");
if myVerbose then writeln();
for i in i1() do if myVerbose then write(i, " ");
if init != off then halt("init incorrect (", init, ")");
if myVerbose then writeln();
for i in i2() do if myVerbose then write(i, " ");
if init != 2*off then halt("init incorrect (", init, ")");
if myVerbose then writeln();
for i in i3() do if myVerbose then write(i, " ");
if init != 3*off then halt("init incorrect (", init, ")");
if myVerbose then writeln();
for i in i4() do if myVerbose then write(i, " ");
if init != 4 then halt("init incorrect (", init, ")");
if myVerbose then writeln();
for i in i5() do if myVerbose then write(i, " ");
if init != 5 then halt("init incorrect (", init, ")");
if myVerbose then writeln();
for i in i6() do if myVerbose then write(i, " ");
if init != 6 then halt("init incorrect (", init, ")");
if myVerbose then writeln();
for i in i7() do if myVerbose then write(i, " ");
if init != 7 then halt("init incorrect (", init, ")");
if myVerbose then writeln();

