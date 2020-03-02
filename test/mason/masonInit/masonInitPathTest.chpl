use FileSystem;
use Spawn;
use Path;
use MasonInit;
use MasonUtils;

proc main(){
  const args = ['init','testSrc'];
  mkdir("testSrc");
  masonInit(args);
  runCommand('rm -rf testSrc');
}

