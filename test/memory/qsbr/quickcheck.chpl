use Time;

config param nQuickChecks = 1024 * 1024;
extern proc chpl_qsbr_quickcheck();
// For now we are not actually registered automatically
chpl_qsbr_quickcheck();

var t : Timer;
t.start();
for i in 1 .. nQuickChecks {
	chpl_qsbr_quickcheck();
}
t.stop();
writeln(t.elapsed() / nQuickChecks);