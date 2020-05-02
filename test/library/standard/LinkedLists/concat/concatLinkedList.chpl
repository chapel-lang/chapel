use LinkedLists;

type L = LinkedList(int);

var l1: L, l2: L;

l1.push_back(4);
l1.push_back(6);
l2.push_back(3);
l2.push_back(5);

writeln(concat(l1, l2));
