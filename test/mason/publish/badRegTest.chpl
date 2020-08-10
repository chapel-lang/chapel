
use MasonPublish;
use MasonUtils;
use MasonNew;

const dir = here.cwd();

proc main() {
  masonNew(['mason','new','publishCheck']);
  here.chdir(dir + '/publishCheck');
  masonPublish(['mason', 'publish', '../bad-regsitry']);
}