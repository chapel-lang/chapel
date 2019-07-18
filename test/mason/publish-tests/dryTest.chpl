
use MasonPublish;
use MasonUtils;



proc dry() {
  masonPublish(['mason', 'publish', '--dry-run']);
}

dry();