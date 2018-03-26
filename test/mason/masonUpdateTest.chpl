
use MasonUpdate;
use MasonUtils;
use TOML;

config const lf: string;
config const tf: string;

proc main() {
  // In order to avoid updating the .good/lock file every release we store
  // a sentinel 'CHPL_CUR_FULL' representing the current Chapel version. Before
  // passing anything to UpdateLock we need to replace that sentinel with the
  // actual current version.

  const currentVersion = getChapelVersionStr();

  var temp = opentmp();
  {
    var w = temp.writer();
    for line in open(lf, iomode.r).lines() do
      w.write(line.replace('CHPL_CUR_FULL', currentVersion));
    w.close();
  }

  var args : [1..0] string;
  UpdateLock(args, tf, temp.tryGetPath());
  var lock = open(temp.tryGetPath(), iomode.r);
  var lockFile =  parseToml(lock);
  writeln(lockFile);
  lock.close();
  delete lockFile;
}
