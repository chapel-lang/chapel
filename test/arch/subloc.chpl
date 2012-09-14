// subloc.chpl

extern proc chpl_task_getSubLoc() : int(32);
extern proc chpl_task_setSubLoc(subloc:int(32)) : void;

writeln("Running on sublocale ", chpl_task_getSubLoc());

chpl_task_setSubLoc(13);

writeln("We claim to be running on sublocale ", chpl_task_getSubLoc());
writeln("but really only allocations will happen there.");



