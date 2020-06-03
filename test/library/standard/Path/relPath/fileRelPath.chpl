use IO;
use FileSystem;
use Path;


const fileDoesNotExist = 'ThisFileDoesNotExist.txt';
const filePathRelative = 'test.txt';
const filePathAbsolute = absPath(filePathRelative);


// Perform a single test, validating the input.
proc test(f: file, start: string="") {
  writeln('filepath= ', f.path, ' start= ', start);
  writeln(f.relPath(start));
}


proc doTests(somepath: string) {
  var f : file;
  try {
    f = open(somepath, iomode.r);
  } catch {
    writeln('Failed to open file!');
    return;
  }

  // Print path used to open file.
  writeln(somepath);

  // Print the abspath of the path used to open.
  writeln(absPath(somepath));

  // Print the path contained in this file.
  writeln(f.path);

  // No start.
  test(f);

  // Fixed relative start value.
  test(f, 'foo/bar/baz');

  // Fixed absolute start value.
  test(f, '/foo/bar/baz');

  // Start is path separator.
  test(f, '/');

  // Start is double path separator.
  test(f, '//');

  // Start is a triple path separator.
  test(f, '///');

  // Start is current dir.
  test(f, '.');

  // Start is parent dir.
  test(f, '..');

  // Start is chain of parent dirs.
  test(f, '../..');

  // Go up a directory.
  var old = here.cwd();
  here.chdir('..');

  // Start is old working directory.
  test(f, old);

  // Start is current working directory.
  test(f, joinPath(old, '..'));

  // Restore old path.
  here.chdir(old);

  f.close();
}


doTests(fileDoesNotExist);
doTests(filePathRelative);
doTests(filePathAbsolute);
