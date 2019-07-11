use MasonPublish;
use MasonUtils;
use Spawn;
use TOML;
use FileSystem;

proc main(){
  cloneMasonReg('test',here.cwd());
  return 0;
}