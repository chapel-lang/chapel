use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){
  const newArgs = ['mason','new','testSrc'];
  masonNew(newArgs);
  runCommand('rm -rf testSrc/src');
  const initArgs = ['init','testSrc'];
  masonInit(initArgs);
  //check if src and src/testSrc.chpl was created
  if isFile("./testSrc/src/testSrc.chpl") && isDir("./testSrc/src") {
    writeln("src has been successfully created");
  }

  runCommand('rm -rf testSrc');
}
