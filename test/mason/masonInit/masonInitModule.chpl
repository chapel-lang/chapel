use FileSystem;
use MasonInit;
use MasonUtils;
use MasonNew;

proc main(){
  mkdir("project-testSrc");
  const initArgs: [0..3] string;
  initArgs = ['mason', 'init', 'project-testSrc', '--name=project'];
  masonInit(initArgs);
  //check if src and src/testSrc.chpl was created
  if isFile("./project-testSrc/src/project.chpl") {
    writeln("Project.chpl has been successfully created");
  }
  rmTree("project-testSrc");
}
