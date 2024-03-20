// Simple test to check that the updateDatFiles.py script is still working

use Subprocess;
use FileSystem;
use IO;
use ChplConfig;

const develPath = "%s/util/devel/".format(CHPL_HOME);
const origDat = "compilerPerformance.dat";
const tempDat = "compilerPerformance.tmp";
const testDatFile = "\"%s/%s\"".format(here.cwd(), tempDat);

copy(origDat, tempDat);
var command = "cd %s && python3 -c 'import updateDatFiles; updateDatFiles.test(%s)'".format(develPath, testDatFile);
{
  var p = spawnshell(command);
  p.wait();
}
{
  var p = spawnshell("cat %s".format(tempDat));
  p.wait();
}
remove(tempDat);
