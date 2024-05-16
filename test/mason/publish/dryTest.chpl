
use MasonPublish;
use MasonUtils;
use MasonNew;

const dir = here.cwd();

proc dry() {
  masonNew(['new', 'publishCheck']);
  here.chdir(dir + '/publishCheck');
  masonPublish(['publish', '--dry-run']);
}

dry();
