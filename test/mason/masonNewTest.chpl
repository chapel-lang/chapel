
use MasonUtils;
use FileSystem;
use MasonNew;

proc main() {
  const args = ['mason', 'new', 'Test'];
  masonNew(args);

  // Confirm structure
  var pwd = getEnv('PWD');
  if isDir(pwd + '/Test/src') {
    if isFile(pwd + '/Test/src/Test.chpl') {
      writeln('File structure: correct');
      rmTree('Test');
    }
  }
  else {
    writeln('File structure: incorrect');
  }
}
