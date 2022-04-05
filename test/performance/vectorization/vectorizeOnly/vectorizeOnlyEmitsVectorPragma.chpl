// check that 'foreach' actually results in the vector pragmas being
// generated

foreach i in 1..10 do writeln(i);
foreach i in zip(1..10, 1..10) do writeln(i);
foreach i in zip(1..10, 1..10) do writeln(i);

foreach i in 1..10 do writeln(i);
foreach i in zip(1..10, 1..10) do writeln(i);
foreach i in zip(1..10, 1..10) do writeln(i);
