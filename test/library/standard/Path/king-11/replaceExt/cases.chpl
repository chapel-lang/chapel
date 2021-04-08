import Path.{replaceExt};
// the original path doesn't have an extension.  returns "foo.txt"
writeln(replaceExt("foo", ".txt"));
// preceding . in extension is optional
writeln(replaceExt("foo.bar", "txt"));
writeln(replaceExt("foo.bar", ".txt"));
// no basename throw error
try {
  writeln(replaceExt("foo/", "txt"));
}
catch e {
  writeln(e.message());
}
// remove ext
writeln(replaceExt("foo.chpl",""));
// path separator error throw
try {
  writeln(replaceExt("/foo/baz/go",".chp/l"));
} catch e {
  writeln(e.message());
}
// spaces in ext error throw
try {
  writeln(replaceExt("/foo/baz/go",".chpl       henlo"));
} catch e {
  writeln(e.message());
}
try {
  writeln(replaceExt("", ""));
}
catch e {
  writeln(e.message());
}
// basename starting with .
writeln(replaceExt(".bashrc", "newext"));
// basename starting with . and containing .
writeln(replaceExt(".exec.out.tmp", "newext"));
writeln(replaceExt("./foo", "newext"));
writeln(replaceExt("/./foo", "newext"));
try {
  writeln(replaceExt("/./", "newext"));
}
catch e {
  writeln(e.message());
}
