use FileSystem;
use MasonPublish;
use MasonUtils;
use Spawn;
use TOML;
use MasonNew;

proc main(){
  masonNew(['mason', 'new', 'publishGitDir']);
  var pwd = getEnv("PWD");
  here.chdir(pwd + '/publishGitDir');
  if doesGitOriginExist() == false {
    here.chdir(pwd);
    rmTree('publishGitDir');
    writeln("Passed! Should return empty string as no repo was initialized");
    return 0;
  }
  else {
    return 1;
  }
}