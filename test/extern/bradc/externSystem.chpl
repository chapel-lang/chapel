extern proc system(command: c_string): int;

writeln("about to execute system command");
system("echo hi > externSystem.appendme");
writeln("done executing system command");
