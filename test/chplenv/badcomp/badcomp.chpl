use Spawn;

param printchplenv = CHPL_HOME + "/util/printchplenv";

var sub = spawnshell("export CHPL_TARGET_COMPILER=gcc && " + printchplenv + 
                     "| grep \"Warning: Unknown compiler\"");
sub.wait();
