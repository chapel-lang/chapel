use Spawn;

param printchplenv = CHPL_HOME + "/util/printchplenv";

var sub = spawnshell("export CHPL_TARGET_COMPILER=gcc && " +
                     "export CHPL_TARGET_CPU=none && " +
                     printchplenv + " 2>&1 | grep \"Warning:\"");
sub.wait();
