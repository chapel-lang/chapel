var r: range = 1..5;
writeln(r);

var ur: boundedRange(uint,false) = 2:uint..6;
writeln(ur);

for i in r do
  writeln((max(i.type), i));

for u in ur do
  writeln((max(u.type), u));
