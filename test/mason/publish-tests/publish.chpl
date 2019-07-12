
use MasonPublish;
use MasonUtils;
use MasonNew;


const dir = here.cwd();

proc main() throws {
  try! {
    masonNew(['mason', 'new' , 'publishCheck']);
    const args = ['mason', 'publish'];
    here.chdir(dir + '/publishCheck');
    masonPublish(args);
    here.chdir(dir);
    rmTree('publishCheck');
  }
  catch {
    here.chdir(dir);
    rmTree('publishCheck/');
    exit(1);
  }
}
