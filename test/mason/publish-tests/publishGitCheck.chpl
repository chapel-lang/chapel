
use MasonPublish;
use MasonUtils;
use Spawn;
use TOML;


proc main(){
  runCommand('mason new testdir',true);
  var pwd = getEnv("PWD");
  here.chdir(pwd + '/testdir');
  if isGitExist() == false{
    here.chdir(pwd);
    runCommand('rm -rf testdir',true);
    writeln("Passed! Should return empty string as no repo was initialized");
    return 0;
  }
  else {
    return 1;
  }
}