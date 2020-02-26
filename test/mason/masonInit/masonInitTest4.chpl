use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){
  const newArgs = ['mason','new','testSrc'];
  masonNew(newArgs);
  
  const initArgs = ['init','testSrc'];
  masonInit(initArgs);
  
  runCommand('rm -rf testSrc');
}
