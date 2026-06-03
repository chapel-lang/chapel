
use MasonNewInit;
use MasonPublish;
use MasonUtils;


const dir = here.cwd();

proc main() {
  try! masonNew(['new' , 'publishCheck']);
  defer {
    here.chdir(dir);
    rmTree('publishCheck');
  }
  here.chdir(dir + '/publishCheck');
  try {
    publishPackage('dummy','dir', true);
  } catch e {
    writeln(e.message());
  }
}
