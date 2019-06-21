
use MasonPublish;
use MasonUtils;

proc test(arg: [] string) {
  masonPublish(arg);
}

proc check () {
  const res0 = test(['mason', 'publish','-h']);
  const res1 = test(['mason', 'publish', '--help']);
  if res0 == res1 {
    exit(0);
  }
  else {
    writeln('Error matching that help outputs');
  }
}