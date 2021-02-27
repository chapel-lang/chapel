use Path;

writeln(splitExt("foo.tmp"));

writeln(splitExt("bar/foo.tmp"));

writeln(splitExt("bar/foo"));

writeln(splitExt("/bar/foo/"));

writeln(splitExt("./foo"));

writeln(splitExt("../foo"));

writeln(splitExt("../foo.tmp"));

writeln(splitExt(".foo"));
