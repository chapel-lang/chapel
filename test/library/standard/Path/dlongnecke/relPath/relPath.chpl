use FileSystem;
use Path;


const pathSep2 = '//';


proc doTest(name: string, start: string="") {
  writeln(name, ' and ', start, ' becomes ', relPath(name, start));
}



// Single slash, no start.
doTest(pathSep);

// Single slash, no common prefix (easy).
doTest(pathSep, 'foo/bar/baz');

// Single slash, common prefix, start < name (means it's empty).
doTest(pathSep, "");

// Single slash, common prefix, start > name.
doTest(pathSep, '/foo/bar/baz');

// Single slash, common prefix, start = name.
doTest(pathSep, pathSep);



// Double slash, no start.
doTest(pathSep2);

// Double slash, no common prefix (easy).
doTest(pathSep2, 'foo/bar/baz');

// Double slash, common prefix, start < name (means it's empty).
doTest(pathSep2, "");

// Double slash, common prefix, start > name.
doTest(pathSep2, (pathSep2 + 'foo/bar/baz'));

// Double slash, common prefix, start = name.
doTest(pathSep2, pathSep2);




// Absolute path, no start.
doTest('/foo/bar/baz');

// Absolute path, no common prefix (easy).
doTest('/foo/bar/baz', 'hello/world');

// Absolute path, common prefix, start < name.
doTest('/foo/bar/baz', '/foo/bar');

// Absolute path, common prefix, start > name.
doTest('/foo/bar/baz', '/foo/bar/baz/ding');

// Absolute path, common prefix, start = name.
doTest('/foo/bar/baz', '/foo/bar/baz');




// Absolute path ('//'), no start.
doTest('//foo/bar/baz');

// Absolute path ('//'), no common prefix (easy).
doTest('//foo/bar/baz', 'hello/world');

// Absolute path ('//'), common prefix, start < name.
doTest('//foo/bar/baz', '//foo/bar');

// Absolute path ('//'), common prefix, start > name.
doTest('//foo/bar/baz', '//foo/bar/baz/ding');

// Absolute path ('//'), common prefix, start = name.
doTest('//foo/bar/baz', '//foo/bar/baz');




// Relative path, no start.
doTest('foo/bar/baz');

// Relative path, no common prefix (easy).
doTest('foo/bar/baz', 'meaning/of/life');

// Relative path, common prefix, start < name.
doTest('foo/bar/baz', 'foo/bar');

// Relative path, common prefix, start > name.
doTest('foo/bar/baz', 'foo/bar/baz/ding');

// Relative path, common prefix, start = name.
doTest('foo/bar/baz', 'foo/bar/baz');



// OTHER CASES (I did not have enough energy to commit to all permutations):



// Relative path, start has leading up-levels.
doTest('foo/bar/baz', '../baz/ding/');

// Relative path, start has trailing up-levels.
doTest('foo/bar/baz', 'foo/bar/..');

// Relative path, leading up-levels, no start.
doTest('../../foo/bar');

// Relative path, trailing up-levels, no start.
doTest('foo/bar/baz/../../');
