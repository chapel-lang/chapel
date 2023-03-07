use Collectives;

var bar: barrier;
bar = new barrier(1);
bar.barrier();

var barArr: [1..1] barrier;
barArr[1] = new barrier(1);
barArr[1].barrier();

var barArr2: [1..10] barrier = new barrier(1);
for bar2 in barArr2 do bar2.barrier();
