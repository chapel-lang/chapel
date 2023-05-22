use List;

var l = new list(int);

l.append(1);
l.append([2, 3, 4]);
l.append(5..6);
l.append(new list(7..9));

writeln(l);
writeln(l.pop());
