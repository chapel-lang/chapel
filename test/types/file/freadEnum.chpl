enum cleaningSupplies {MrClean, Windex, SoftScrub, mop, broom};
var item1: cleaningSupplies;
var item2: cleaningSupplies;

var f: file = new file(filename = "freadEnum.txt", mode = FileAccessMode.read);
f.open();

f.read(item1, item2);
writeln(item1, " ", item2);
