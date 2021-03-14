//This should no longer be implicitly zippered!
//An error will result if it is considered zippered.
for i in (1..8, 2..60, 3..39) do writeln(i);

for i in (1,2,3) {
  writeln(i);
}

for i in ((1,2),(3,4),(5,6),(7,8),(9,10)) {
    writeln(i);
}

forall i in ((1,2),(3,4),(5,6),(7,8),(9,10)) {}

coforall i in ((1,2),(3,4),(5,6),(7,8),(9,10)) {}

writeln("SUCCESS");
