use MasonUtils;
use FileSystem;
use MasonNew;


proc main() {
  const args : [0..4] string;
  args = ['mason', 'new', 'project-testSrc', '--name', 'project'];
  masonNew(args);
  if isFile("./project-testSrc/src/project.chpl") {
    writeln("Project.chpl has been successfully created");
  }
  rmTree('project-testSrc');
}
