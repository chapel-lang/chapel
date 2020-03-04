use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){
  const newArgs : [0..2] string;
  const initArgs : [0..2] string;
  newArgs = ['mason','new','testSrc'];
  masonNew(newArgs);
  runCommand('rm -rf testSrc/.git');
  initArgs = ['mason','init','testSrc'];
  masonInit(initArgs);
  //check if src and src/testSrc.chpl was created
  if isDir("./testSrc/.git") {
    writeln(".git has been successfully created");
  }

  rmTree("testSrc");
}
