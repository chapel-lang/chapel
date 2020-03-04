use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){  
  const initArgs : [0..2] string;
  mkdir("testSrc");
  initArgs = ['mason','init','testSrc'];
  masonInit(initArgs);
  
  runCommand('rm -rf testSrc/src/testSrc.chpl');
  masonInit(initArgs);

  if isFile("./testSrc/src/testSrc.chpl") {
    writeln("File found !");
  }

 rmTree("testSrc");
}
