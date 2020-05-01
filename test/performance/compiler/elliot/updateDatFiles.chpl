// Simple test to check that the updateDatFiles.py script is still working

use Spawn;
use FileSystem;
use IO;

const develPath = "%s/util/devel/".format(CHPL_HOME);
const origDat = "compilerPerformance.dat";
const tempDat = "compilerPerformance.tmp";
const testDatFile = "\"%s/%s\"".format(here.cwd(), tempDat);

copy(origDat, tempDat);
var command = "cd %s && python -c 'import updateDatFiles; updateDatFiles.test(%s)'".format(develPath, testDatFile);
spawnshell(command).wait();
spawnshell("cat %s".format(tempDat)).wait();
remove(tempDat);
