use List, IO;

var mylist = new list([1,2,3,4,5]);
var otherlist: mylist.type;

var f = openMemFile();

f.writer(serializer=new binarySerializer(), locking=false).write(mylist);
f.reader(deserializer=new binaryDeserializer(), locking=false).read(otherlist);

assert(mylist.size == otherlist.size);
assert(mylist == otherlist);
