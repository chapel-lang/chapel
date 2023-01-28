use DistributedList;

config const n = 10000;

var a = [i in 0..<n] i;
var dl = new distributedList(a);
var a_ = dl.toArray();

writeln(&& reduce (a_ == a));
