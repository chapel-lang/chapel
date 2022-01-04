
use MasonNew;
use MasonPublish;
use MasonUtils;

const dir = here.cwd();

proc main() {
  masonNew(['new','publishCheck']);
  here.chdir(dir + '/publishCheck');
  masonPublish(['publish', '../bad-regsitry']);
}