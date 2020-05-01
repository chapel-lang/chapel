use IO;

enum cleaningSupplies {MrClean, Windex, SoftScrub, mop, broom};
var item1: cleaningSupplies;
var item2: cleaningSupplies;

var f = open("freadEnum.txt", iomode.r).reader();

f.read(item1, item2);
writeln(item1, " ", item2);
