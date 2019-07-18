
use MasonPublish;
use MasonUtils;


const dir = here.cwd();

proc dry() {
  masonNew(['mason', 'new', 'publishCheck']);
  here.chdir(dir + '/publishCheck');
  masonPublish(['mason', 'publish', '--dry-run']);
}

dry();