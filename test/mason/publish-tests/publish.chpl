
use MasonPublish;
use MasonUtils;

proc main() throws {
  try! {
    const args = ['mason', 'publish'];
    runCommand('mason new test_dir');
    var dir = getEnv('PWD');
    here.chdir(dir + '/test_dir');
    runCommand('mason publish');
  }
}



