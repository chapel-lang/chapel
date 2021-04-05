import Path.{replaceBasename};

// the original path doesn't have a basename. returns "/foo/bar/baz"
writeln(replaceBasename("/foo/bar/", "baz"));
writeln(replaceBasename("", ""));
writeln(replaceBasename("/", ""));
writeln(replaceBasename("foo/", ""));
writeln(replaceBasename("foo/bar", ""));
writeln(replaceBasename("", "foo"));
writeln(replaceBasename("foo/bar.txt", "baz"));

writeln(replaceBasename("foo.c","d.chpl"));
writeln(replaceBasename("/foo/baz.c","d.chpl"));
writeln(replaceBasename("foo/baz ending.","d.chpl"));
writeln(replaceBasename("baz ending.","d.chpl"));
writeln(replaceBasename("baz/ending.","d.chpl"));
writeln(replaceBasename("../foo","d.chpl"));
writeln(replaceBasename(".foo","d.chpl"));
writeln(replaceBasename("bar/foo.c/","d.chpl/"));
