use CTypes;
extern proc system(command: c_ptrConst(c_char)): int;

writeln("about to execute system command");
system("echo hi > externSystem.appendme");
writeln("done executing system command");
