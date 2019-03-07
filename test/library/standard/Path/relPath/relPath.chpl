use FileSystem;
use Path;


const pathSep2 = '//';


proc test(name: string, start: string="") {
  writeln('name= ', name, ' start= ', start);
  writeln(relPath(name, start));
}


// Single slash, no start.
test(pathSep);

// Single slash, no common prefix (easy).
test(pathSep, 'foo/bar/baz');

// Single slash, common prefix, start < name (means it's empty).
test(pathSep, "");

// Single slash, common prefix, start > name.
test(pathSep, '/foo/bar/baz');

// Single slash, common prefix, start = name.
test(pathSep, pathSep);


// Double slash, no start.
test(pathSep2);

// Double slash, no common prefix (easy).
test(pathSep2, 'foo/bar/baz');

// Double slash, common prefix, start < name (means it's empty).
test(pathSep2, "");

// Double slash, common prefix, start > name.
test(pathSep2, (pathSep2 + 'foo/bar/baz'));

// Double slash, common prefix, start = name.
test(pathSep2, pathSep2);


// Absolute path, no start.
test('/foo/bar/baz');

// Absolute path, no common prefix (easy).
test('/foo/bar/baz', 'hello/world');

// Absolute path, common prefix, start < name.
test('/foo/bar/baz', '/foo/bar');

// Absolute path, common prefix, start > name.
test('/foo/bar/baz', '/foo/bar/baz/ding');

// Absolute path, common prefix, start = name.
test('/foo/bar/baz', '/foo/bar/baz');


// Absolute path ('//'), no start.
test('//foo/bar/baz');

// Absolute path ('//'), no common prefix (easy).
test('//foo/bar/baz', 'hello/world');

// Absolute path ('//'), common prefix, start < name.
test('//foo/bar/baz', '//foo/bar');

// Absolute path ('//'), common prefix, start > name.
test('//foo/bar/baz', '//foo/bar/baz/ding');

// Absolute path ('//'), common prefix, start = name.
test('//foo/bar/baz', '//foo/bar/baz');


// Relative path, no start.
test('foo/bar/baz');

// Relative path, no common prefix (easy).
test('foo/bar/baz', 'meaning/of/life');

// Relative path, common prefix, start < name.
test('foo/bar/baz', 'foo/bar');

// Relative path, common prefix, start > name.
test('foo/bar/baz', 'foo/bar/baz/ding');

// Relative path, common prefix, start = name.
test('foo/bar/baz', 'foo/bar/baz');


// Relative path, start has leading up-levels.
test('foo/bar/baz', '../baz/ding/');

// Relative path, start has trailing up-levels.
test('foo/bar/baz', 'foo/bar/..');

// Relative path, leading up-levels, no start.
test('../../foo/bar');

// Relative path, trailing up-levels, no start.
test('foo/bar/baz/../../');


// Move up a directory, perform some tests, then go back.
var old = here.cwd();
here.chdir('..');


// Relative path, start has leading up-levels.
test('foo/bar/baz', '../baz/ding/');

// Relative path, start has trailing up-levels.
test('foo/bar/baz', 'foo/bar/..');

// Relative path, leading up-levels, no start.
test('../../foo/bar');

// Relative path, trailing up-levels, no start.
test('foo/bar/baz/../../');

// Use current directory and old working directory.
test(here.cwd(), old);


// Restore old path.
here.chdir(old);
