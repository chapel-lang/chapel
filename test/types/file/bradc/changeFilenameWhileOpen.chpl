var f = file(filename = "f.out", mode = "w");

f.open();
f.filename = "g.out";
f.close();

writeln("Should never get here!\n");
