use List, IO;

var mylist = new list([1,2,3,4,5]);
var otherlist: mylist.type;

var f = openmem();

f.writer(kind=ionative).write(mylist);
f.reader(kind=ionative).read(otherlist);

assert(mylist.size == otherlist.size);
assert(mylist == otherlist);
