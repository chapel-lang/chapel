use FileSystem;
use Spawn;
use Path;
use MasonInit;
use MasonUtils;

proc main(){
  const args = ['mason','init','testSrc'];
  mkdir("testSrc");
  masonInit(args);
  rmTree("testSrc");
}

