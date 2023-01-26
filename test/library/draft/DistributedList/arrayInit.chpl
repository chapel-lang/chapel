use DistributedList;

var a = [i in 0..<1000] i;
var dl = new distributedList(a);
var a_ = dl.toArray();

writeln(&& reduce (a_ == a));
