use FileSystem;
use Subprocess;
use Path;
use MasonInit;
use MasonUtils;

proc main(){
  const args = ['init','testSrc'];
  mkdir("testSrc");
  masonInit(args);
  rmTree("testSrc");
}

