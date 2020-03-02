
use MasonPublish;
use MasonUtils;
use MasonNew;

const dir = here.cwd();

proc main() {
  masonNew(['new', 'publishCheck','']);
  here.chdir(dir + '/publishCheck');
  masonPublish(['mason', 'publish', '--dry-run', '../bad-registry']);
}