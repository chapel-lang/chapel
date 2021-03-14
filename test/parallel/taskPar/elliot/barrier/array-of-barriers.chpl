use Barriers;

var bar: Barrier;
bar = new Barrier(1);
bar.barrier();

var barArr: [1..1] Barrier;
barArr[1] = new Barrier(1);
barArr[1].barrier();

var barArr2: [1..10] Barrier = new Barrier(1);
for bar2 in barArr2 do bar2.barrier();
