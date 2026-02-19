use FileSystem;
use MasonNew;
use MasonUtils;

proc main() throws {
  var args = ['new', 'Test'];
  try! masonNew(args);

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
  try {
    masonNew(args2);
  } catch e: MasonError {
    writeln(e.message());
  }

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
  try {
    masonNew(args3);
  } catch e: MasonError {
    writeln(e.message());
  }

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
  try {
    masonNew(args4);
  } catch e: MasonError {
    writeln(e.message());
  }

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
