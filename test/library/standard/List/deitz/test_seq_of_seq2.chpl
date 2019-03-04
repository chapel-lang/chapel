use LinkedLists;

var s1 : LinkedList(int)       = makeList(1, 2, 3, 4);
var s2 : LinkedList(int)       = makeList(5, 6, 7, 8);

writeln(s1);
writeln(s2);

var ss : LinkedList(LinkedList(int)) = makeList(s1, s2);

writeln(ss);

for s in ss do
  writeln(s);

for s in ss do
  for i in s do
    writeln(i);

ss.destroy();
s2.destroy();
s1.destroy();
