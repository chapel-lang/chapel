
use DistributedList;
var dl = new distributedList(int);

writeln(dl.size);

for i in 0..<10 do
    dl.append(i);

writeln(dl);
writeln(dl.size);

dl.insert(5, 100);
writeln(dl);
writeln(dl.size);

writeln(dl.pop(2));
writeln(dl);
writeln(dl.size);

dl.insert(7, [1, 2, 3, 4]);
writeln(dl);
writeln(dl.size);

writeln(dl.contains(3));
writeln(dl.remove(3, 2));
writeln(dl.contains(3));
writeln(dl);
writeln(dl.size);

writeln(dl.first());
writeln(dl.last());

writeln(dl.set(4, 200));
writeln(dl);
writeln(dl.size);

writeln(dl.toArray());
