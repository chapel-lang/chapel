var s1: real;
var flag1: sync bool;
var done: sync bool;

extern proc printf(x...);

proc main() {
  s1 = 1.0;
  on Locales(1) do begin with (ref s1) {
    const tmp = flag1;
    printf("%s\n", "s1 is: " + s1);
    done = true;
  }
  s1 = 2.0;
  flag1 = true;
  done;

  var s2: real;
  var flag2: sync bool;

  s2 = 1.0;
  on Locales(1) do begin with (ref s2) {
    const tmp = flag2;
    printf("%s\n", "s2 is: " + s2);
  }
  s2 = 2.0;
  flag2 = true;
}
