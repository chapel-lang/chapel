
use MasonPublish;
use MasonUtils;
use MasonNew;

proc main() throws {
  try! {
    const dir = getEnv('PWD');
    const args = ['mason', 'publish'];
    here.chdir(dir + '/publishCheck');
    masonPublish(args);
  }
}



