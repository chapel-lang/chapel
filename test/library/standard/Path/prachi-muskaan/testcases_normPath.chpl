/* Test cases to validate the normPath(name: string) in Path.chpl */

use Path;

writeln(normPath("foo//bar"));
writeln(normPath("foo/bar/"));
writeln(normPath("foo/./bar"));
writeln(normPath("foo/baz/../bar"));
writeln(normPath("/foo/baz/../../bar"));
writeln(normPath("foo/baz/././bar"));
writeln(normPath("foo/baz/./../bar"));
writeln(normPath("foo/bar/."));
writeln(normPath("foo/bar/.."));
writeln(normPath("/../"));
writeln(normPath("../"));
writeln(normPath("./"));