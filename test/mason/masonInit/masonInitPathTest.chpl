use FileSystem;
use Spawn;
use Path;
use MasonInit;
use MasonUtils;

proc main(){
  const args: [0..2] string;
  args = ['mason','init','testSrc'];
  mkdir("testSrc");
  masonInit(args);
  rmTree("testSrc");
}

