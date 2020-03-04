use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){
  const newArgs : [0..2] string;
  const initArgs : [0..2] string;
  newArgs = ['mason','new','testSrc'];
  masonNew(newArgs);
  runCommand('rm -rf testSrc/src');
  initArgs = ['mason', 'init','testSrc'];
  masonInit(initArgs);
  //check if src and src/testSrc.chpl was created
  if isFile("./testSrc/src/testSrc.chpl") && isDir("./testSrc/src") {
    writeln("src has been successfully created");
  }

  rmTree("testSrc");
}
