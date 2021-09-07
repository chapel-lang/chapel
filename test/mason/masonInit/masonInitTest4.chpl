use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){
  const newArgs = ['new','testSrc'];
  masonNew(newArgs);

  const initArgs = ['init','testSrc'];
  masonInit(initArgs);

  rmTree('testSrc');
}
