use MasonPublish;
use MasonUtils;
use Spawn;
use TOML;


proc main(){
  forkMasonReg();
  var pwd = getEnv("PWD");
  here.chdir(pwd +'/mason-registry');
  here.chdir(pwd);
  runCommand('rm -rf mason-registry');
  return 0;
}