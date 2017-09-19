
use MasonUpdate;
use TOML;

config const lf: string;
config const tf: string;

proc main() {
  var args : [1..0] string;
  UpdateLock(args, tf, lf);
  var lock = open(lf, iomode.r);
  var lockFile =  parseToml(lock);
  writeln(lockFile);
  lock.close();
  delete lockFile;
}
