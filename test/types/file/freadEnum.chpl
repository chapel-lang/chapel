enum cleaningSupplies {MrClean, Windex, SoftScrub, mop, broom};
var item1: cleaningSupplies;
var item2: cleaningSupplies;

var f: file = file(filename = "freadEnum.txt", mode = "r");
f.open;

fread(f, item1, item2);
writeln(item1, " ", item2);
