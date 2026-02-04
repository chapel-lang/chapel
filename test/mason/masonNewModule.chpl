use FileSystem;
use MasonNew;
use MasonUtils;


proc main() throws {
  const args : [0..3] string;
  args = ['new', 'project-testSrc', '--name', 'project'];
  try! masonNew(args);
  if isFile("./project-testSrc/src/project.chpl") {
    writeln("Project.chpl has been successfully created");
  }
  rmTree('project-testSrc');
}
