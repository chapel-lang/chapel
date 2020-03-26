use LinkedLists;

var list = new LinkedList (int);

//testing contains on an empty list
writeln(list.contains(1));

list.append(1);

writeln(list.front());
writeln(list.back());
//testing contains on a list that contains the item
writeln(list.contains(1));
//testing contains on a list that does not contain the item
writeln(list.contains(2));

list.append(2);

writeln(list.front());
writeln(list.back());
writeln(list.contains(2));
writeln(list.contains(3));

list.append(3);

writeln(list.back());

list.destroy();
