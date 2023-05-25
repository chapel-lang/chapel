use UnrolledLinkedList;

config const n = 3;

var theList = new unrolledLinkedList(int);

var r = 1..n;
var a: [r] int;
var otherList = new unrolledLinkedList(int);
for i in r do otherList.append(i);

theList.extend(r);
theList.extend(a);
theList.extend(otherList);

writeln(theList);
