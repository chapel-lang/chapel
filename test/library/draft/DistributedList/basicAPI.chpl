use DistributedList;
var dl = new distributedList(int);

// ---------- additions -------------

writeln("\nappend values...");
writeln(dl.size == 0);
for i in 0..<25 do write(dl.append(i), " ");
writeln(dl.size == 25);
writeln(dl);

writeln("\nappend array...");
writeln(dl.append([1, 2, 3, 4, 5]));
writeln(dl.size == 30);
writeln(dl);

writeln("\ninsert values...");
writeln(dl.insert(5, -5));
writeln(dl.insert(21, -20));
writeln(dl.size == 32);
writeln(dl);

writeln("\ninsert array...");
writeln(dl.insert(12, [1, 2, 3, 4, 5]));
writeln(dl.size == 37);
writeln(dl);

// ---------- queries -------------

writeln("\nfirst/last...");
writeln(dl.first());
writeln(dl.last());

writeln("\ncontains...");
writeln(dl.contains(5));
writeln(dl.contains(99));

writeln("\ncount...");
writeln(dl.count(1) == 3);

writeln("\nfind...");
writeln("all: \t", dl.find(1));
writeln("10..: \t", dl.find(1, 10));
writeln("0..10: \t", dl.find(1, 0, 10));
writeln("14..: \t", dl.find(1, 14));
writeln("6..10: \t", dl.find(1, 6, 10));

writeln("\nindices...");
writeln(dl.indices);

writeln("\ngetValue...");
writeln(dl.getValue(20));

// ---------- modifications -------------

writeln("\nthis()...");
writeln(dl[5] == -5);
dl[5] = 100;
writeln(dl[5] == 100);

writeln("\nset...");
writeln(dl.set(100, -1));
writeln(dl.set(0, -1));
writeln(dl.set(20, -1));
writeln(dl.size == 37);
writeln(dl);

// ---------- removals -------------

writeln("\nremove...");
writeln(dl.remove(1, 2));
writeln(dl);
writeln(dl.size == 35);

writeln("\npop...");
writeln(dl.pop());
writeln(dl.pop(5));
writeln(dl);
writeln(dl.size == 33);

writeln("\nclear...");
dl.clear();
writeln(dl.size == 0);
writeln(dl.isEmpty());
writeln(dl);
