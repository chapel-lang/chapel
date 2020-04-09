proc testit(br) {
  writeln();
  writeln(br.type:string);
  for b in br do
    writeln(b);
  writeln("---");
}

testit(false..true);
for b in false..true do
  writeln(b);
writeln();

testit(true..false);
for b in true..false do
  writeln(b);

testit(false..false);
for b in false..false do
  writeln(b);

testit(true..true);
for b in true..true do
  writeln(b);

testit(false..#2);
for b in false..#2 do
  writeln(b);

testit(false..true by 2);
for b in false..true by 2 do
  writeln(b);

testit(false..true by 2 align true);
for b in false..true by 2 align true do
  writeln(b);
