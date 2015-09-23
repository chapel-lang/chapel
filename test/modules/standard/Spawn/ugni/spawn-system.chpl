use Spawn;

// This test is used to demonstrate the current limitations of Spawn under ugni
// as well as what situations do work. See JIRA issue 133 for more info:
// chapel.atlassian.net/browse/CHAPEL-113

config const pipeStdout = false;

var stdoutArg=FORWARD;
if pipeStdout then stdoutArg=PIPE;

var sub = spawn(["ls", "../test.txt"], stdout=stdoutArg);
sub.wait();
assert(sub.running == false);
assert(sub.exit_status == 0);
