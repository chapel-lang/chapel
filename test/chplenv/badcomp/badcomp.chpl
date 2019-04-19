use Spawn;

param printchplenv = CHPL_HOME + "/util/printchplenv";

var sub = spawnshell("export CHPL_TARGET_COMPILER=gcc && " + printchplenv + 
                     " 2>&1 | grep \"Warning: Unknown compiler\" | uniq");
sub.wait();
