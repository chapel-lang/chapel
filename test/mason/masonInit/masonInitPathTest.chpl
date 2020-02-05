use FileSystem;
use Spawn;
use Path;
use MasonInit;
use MasonUtils;

proc main(){
  const args = ['init','asd'];
  mkdir("asd");
  masonInit(args);
  runCommand('rm -rf asd');
}

