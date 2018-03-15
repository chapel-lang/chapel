use Time;

config param nCheckpoints = 1024 * 1024;
extern proc chpl_qsbr_checkpoint();
// For now we are not actually registered automatically, but we will in checkpoint
chpl_qsbr_checkpoint();

var t : Timer;
t.start();
for i in 1 .. nCheckpoints {
	chpl_qsbr_checkpoint();
}
t.stop();
writeln(t.elapsed() / nCheckpoints);