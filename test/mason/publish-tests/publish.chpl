
use MasonPublish;
use MasonUtils;
use MasonNew;

proc main() throws {
  const dir = here.cwd();
  try! {
    masonNew(['mason', 'new' , 'publishCheck', '--no-vcs']);
    const args = ['mason', 'publish'];
    here.chdir(dir + '/publishCheck'); 
    masonPublish(args);
    here.chdir(dir);
    rmTree('publishCheck');
  }
  catch {
    here.chdir(dir);
    rmTree('publishCheck');
    exit(1);
  }
}



