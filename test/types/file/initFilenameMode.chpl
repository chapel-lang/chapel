var f : file = new file(filename = "_test_filename.txt");

writeln("filename set in constructor:  ", f.filename);
writeln("default mode:  ", f.mode);
f.mode = "w";
writeln("mode reset to:  ", f.mode);
