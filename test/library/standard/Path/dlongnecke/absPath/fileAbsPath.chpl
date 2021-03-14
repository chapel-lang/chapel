use IO, Path;


const fileDoesNotExist = 'ThisFileDoesNotExist.txt';
const filePathRelative = 'test.txt';
const filePathAbsolute = absPath(filePathRelative);


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

  // NOTE: Don't use, appears to print canonical path instead of path used to
  // open the file.
  // -
  // writeln(f.path())

  // Print the abspath of the path used to open.
  writeln(absPath(somepath));

  // Get expected output for our test file.
  writeln(f.absPath());
  f.close();
}


doTests(fileDoesNotExist);
doTests(filePathRelative);
doTests(filePathAbsolute);
