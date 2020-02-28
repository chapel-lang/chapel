use MasonUtils;
use FileSystem;
use MasonNew;


proc main() {
  const args = ['new', 'project-testSrc', '--name', 'project'];
  masonNew(args);
  if isFile("./project-testSrc/src/project.chpl") {
    writeln("Project.chpl has been successfully created");
  }
  runCommand('rm -rf project-testSrc');
}
