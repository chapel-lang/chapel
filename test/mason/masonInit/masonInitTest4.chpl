use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){
  const newArgs : [0..2] string;
  const initArgs : [0..2] string;
  newArgs = ['mason','new','testSrc'];
  masonNew(newArgs);
  
  initArgs = ['mason','init','testSrc'];
  masonInit(initArgs);
  
  runCommand('rm -rf testSrc');
}
