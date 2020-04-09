use Futures;
use Time;

config const doGet = false;

proc test() {
  var F = async(lambda(x: int) { sleep(1); return 42 + x; }, 23);
  if doGet then
    F.get();
}

proc main() {
  test();
}
