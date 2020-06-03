use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){  
  mkdir("testSrc");
  const initArgs = ['mason','init','testSrc'];
  masonInit(initArgs);
  
  remove('testSrc/src/testSrc.chpl');
  masonInit(initArgs);

  if isFile("./testSrc/src/testSrc.chpl") {
    writeln("File found !");
  }

 rmTree("testSrc");
}
