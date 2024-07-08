use FileSystem;
use MasonNew;

proc main() {
  var args = ['new', 'Test'];
  masonNew(args);

  // Confirm structure
  var pwd = here.cwd();
  if isDir(pwd + '/Test/src') {
    if isFile(pwd + '/Test/src/Test.chpl') {
      writeln('File structure: correct');
      rmTree('Test');
    }
  }
  else {
    writeln('File structure: incorrect');
  }

  var args2 = ['new', 'Test', '--app'];
  masonNew(args2);

  // Confirm structure
  if isDir(pwd + '/Test/src') {
    if isFile(pwd + '/Test/src/Test.chpl') {
      writeln('File structure: correct');
      rmTree('Test');
    }
  }
  else {
    writeln('File structure: incorrect');
  }

  var args3 = ['new', 'Test', '--lib'];
  masonNew(args3);

  // Confirm structure
  if isDir(pwd + '/Test/src') {
    if isFile(pwd + '/Test/src/Test.chpl') {
      writeln('File structure: correct');
      rmTree('Test');
    }
  }
  else {
    writeln('File structure: incorrect');
  }

  var args4 = ['new', 'Test', '--light'];
  masonNew(args4);

  // Confirm structure
  if isDir(pwd + '/Test') {
    if isFile(pwd + '/Test/Mason.toml') {
      writeln('File structure: correct');
      rmTree('Test');
    }
  }
  else {
    writeln('File structure: incorrect');
  }
}
