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
      rmTree('src');
      rmTree('.git');
      remove('Mason.toml');
      remove('.gitignore');
  }
  else {
    writeln("Init is incorrect");
  }
}
