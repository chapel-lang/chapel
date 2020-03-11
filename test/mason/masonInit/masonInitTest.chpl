use FileSystem;
use MasonInit;
use MasonUtils;

proc main(){
  const args = ['init'];
  masonInit(args);

  //check if files have been created 
  const pwd = getEnv('PWD');
  if isDir(pwd + '/src'){
      writeln("Init is correctly done");
      runCommand('rm -rf src example test Mason.toml .git');
  }
  else {
    writeln("Init is incorrect");
  }
}
