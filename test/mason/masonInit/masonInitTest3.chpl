use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){
  const newArgs = ['mason','new','testSrc'];
  masonNew(newArgs);
  runCommand('rm -rf testSrc/.git');
  const initArgs = ['init','testSrc'];
  masonInit(initArgs);
  //check if src and src/testSrc.chpl was created
  if isDir("./testSrc/.git") {
    writeln(".git has been successfully created");
  }

  runCommand('rm -rf testSrc');
}
