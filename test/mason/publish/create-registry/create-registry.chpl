use MasonPublish;
use FileSystem;

proc main(){
  mkdir('pathToRegistry');
  masonPublish(['mason', 'publish', '--create-registry', 'pathToRegistry/registry']);
}

