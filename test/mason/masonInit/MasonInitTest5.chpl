use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){  
  mkdir("testSrc");
  const initArgs = ['init','testSrc'];
  masonInit(initArgs);
  
  runCommand('rm -rf testSrc/src/testSrc.chpl');
  masonInit(initArgs);

  if isFile("./testSrc/src/testSrc.chpl") {
    writeln("File found !");
  }

  runCommand('rm -rf testSrc');
}
