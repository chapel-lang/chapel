// check that the vectorizeOnly actually results in the vector pragmas being
// generated

for i in vectorizeOnly(1..10) do writeln(i);
for i in vectorizeOnly(1..10, 1..10) do writeln(i);
for i in zip(vectorizeOnly(1..10, 1..10)) do writeln(i);

forall i in vectorizeOnly(1..10) do writeln(i);
forall i in vectorizeOnly(1..10, 1..10) do writeln(i);
forall i in zip(vectorizeOnly(1..10, 1..10)) do writeln(i);
