use ChainTable;

var table = new chainTable(uint, int, 10);

// add four entries to the table in the same bucket
for i in [0, 5, 8, 13] {
    writeln("i: ", i);
    var slot = table.getSlotFor(i);
    writeln("s: ", slot);
    table.fillSlot(slot, i, 100 * i);
    var fslot = table.getFullSlotFor(i);
    writeln("f: ", fslot);
    writeln(table.buckets[0]);
    writeln("------------------------");
}
writeln("size: ", table.numEntries, "\n");

// remove 5 and 13
var k;
var v;
table.remove(table.getSlotFor(5), k, v);
writeln(k, ", ", v);
writeln(table.buckets[0]);

table.remove(table.getSlotFor(13), k, v);
writeln(k, ", ", v);
writeln(table.buckets[0]);
writeln("size: ", table.numEntries, "\n");

// re-add 5, and replace 13 w/ 16
for i in [5, 16] {
    writeln("i: ", i);
    var slot = table.getSlotFor(i);
    writeln("s: ", slot);
    table.fillSlot(slot, i, 100 * i);
    var fslot = table.getFullSlotFor(i);
    writeln("f: ", fslot);
    writeln(table.buckets[0]);
    writeln("------------------------");
}
writeln("size: ", table.numEntries, "\n");

// update an existing entry
var s8 = table.getSlotFor(8);
table.fillSlot(s8, 8, 88);
writeln(table.buckets[0]);
writeln("size: ", table.numEntries);
