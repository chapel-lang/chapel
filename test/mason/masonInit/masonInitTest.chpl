use FileSystem;
use MasonInit;
use MasonUtils;

proc main(){
  const args = ['mason', 'init', '-d'];
  masonInit(args);

  //check if files have been created 
  const pwd = getEnv('PWD');
  if isDir(pwd + '/src'){
      writeln("Init is correctly done");
      rmTree('src');
      rmTree('example');
      rmTree('.git');
      rmTree('test');
      remove('Mason.toml');
  }
  else {
    writeln("Init is incorrect");
  }
}
