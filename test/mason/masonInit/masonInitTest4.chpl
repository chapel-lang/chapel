use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){
  const newArgs = ['mason','new','testSrc'];
  masonNew(newArgs);
  
  const initArgs = ['mason','init','testSrc'];
  masonInit(initArgs);

  rmTree('testSrc');  
}
