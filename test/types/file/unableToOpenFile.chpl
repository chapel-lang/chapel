var f : file = file(filename = "_test_cannotOpenMe.txt");

writeln(f.filename);
writeln(f.mode);
f.open;
f.close;
