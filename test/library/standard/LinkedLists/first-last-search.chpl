use LinkedLists;

var list = new LinkedList (int);
writeln(list.search(1));  //searching an empty list
list.append(1);
writeln(list.search(1)); //searching for an item present
writeln(list.front()); //front of a single item list
writeln(list.back()); //back of a single item list
writeln(list.search(2)); //searching for an item not present
list.append(2);
writeln(list.front());
writeln(list.back());
writeln(list.search(2));
list.append(3);
writeln(list.back());
list.prepend(0);
writeln(list.front());
