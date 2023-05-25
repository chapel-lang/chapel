use ChainTable;

var table = new chainTable(string, int, 2);

for (letter, number) in zip(["a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k"], 0..) {
    table.fillSlot(table.getSlotFor(letter), letter, number);
}

writeln("2 buckets:");
writeln(table.buckets[0]);
writeln(table.buckets[1]);

table.__rehash(4);
writeln("4 buckets:");
writeln(table.buckets[0]);
writeln(table.buckets[1]);
writeln(table.buckets[2]);
writeln(table.buckets[3]);
