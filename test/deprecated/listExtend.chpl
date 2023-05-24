use List;

config const n = 3;

var theList = new list(int);

var r = 1..n;
var a: [r] int;
var otherList = new list(int);
for i in r do otherList.pushBack(i);

theList.extend(r);
theList.extend(a);
theList.extend(otherList);

writeln(theList);
