
use MasonNew;
use MasonPublish;
use MasonUtils;


const dir = here.cwd();

proc main() throws {
  try! {
    masonNew(['new' , 'publishCheck']);
    here.chdir(dir + '/publishCheck');
    publishPackage('dummy','dir', true);
    here.chdir(dir);
    rmTree('publishCheck');
  }
  catch {
    here.chdir(dir);
    rmTree('publishCheck/');
    exit(1);
  }
}
