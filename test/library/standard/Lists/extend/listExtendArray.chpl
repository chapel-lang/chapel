use Lists;

config type listType = int;

var lst1 = new List(listType, false);

for i in 1..5 do
  lst1.append(i);

var lst2 = new List(listType, false);
lst2.extend(lst1);

writeln(lst1);
writeln(lst2);

