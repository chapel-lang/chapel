
use MasonPublish;
use MasonUtils;
use MasonNewInit;

const dir = here.cwd();

proc dry() {
  masonNew(['new', 'publishCheck']);
  here.chdir(dir + '/publishCheck');
  masonPublish(['publish', '--dry-run']);
}

dry();
