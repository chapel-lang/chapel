use iters;
config const myVerbose = false;

for i in i0() do if myVerbose then write(i, " ");
if initial != 0 then halt("initial incorrect (", initial, ")");
if myVerbose then writeln();
for i in i1() do if myVerbose then write(i, " ");
if initial != off then halt("initial incorrect (", initial, ")");
if myVerbose then writeln();
for i in i2() do if myVerbose then write(i, " ");
if initial != 2*off then halt("initial incorrect (", initial, ")");
if myVerbose then writeln();
for i in i3() do if myVerbose then write(i, " ");
if initial != 3*off then halt("initial incorrect (", initial, ")");
if myVerbose then writeln();
for i in i4() do if myVerbose then write(i, " ");
if initial != 4 then halt("initial incorrect (", initial, ")");
if myVerbose then writeln();
for i in i5() do if myVerbose then write(i, " ");
if initial != 5 then halt("initial incorrect (", initial, ")");
if myVerbose then writeln();
for i in i6() do if myVerbose then write(i, " ");
if initial != 6 then halt("initial incorrect (", initial, ")");
if myVerbose then writeln();
for i in i7() do if myVerbose then write(i, " ");
if initial != 7 then halt("initial incorrect (", initial, ")");
if myVerbose then writeln();

