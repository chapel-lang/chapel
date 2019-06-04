use Time;

iter zipper(A1 : [?D1], A2 : [?D2]) ref {
	if D1 == D2 {
		for idx in D1 do yield (A1[idx], A2[idx]);
	} else {
		for (a1, a2) in zip (A1, A2) do yield (a1, a2);
	}
}

iter zipper (A1 : [?D1], A2 : [?D2], param tag : iterKind) ref where tag == iterKind.standalone {
	if D1 == D2 {
		forall idx in D1 do yield (A1[idx], A2[idx]);
	} else {
		forall (a1, a2) in zip (A1, A2) do yield (a1, a2);
	}
}

proc domainQuerySerial(A1 : [?D1], A2 : [?D2]) {
  for idx in D1 do A1[idx] += A2[idx];
}

proc domainQueryParallel(A1 : [?D1], A2 : [?D2]) {
  forall idx in D1 do A1[idx] += A2[idx];
}

config const printTiming : bool = true;
config const N = 256 * 1024 * 1024;
var D = {1..N};
var A1 : [D] int = 1..#D.size;
var A2 : [D] int = 1..#D.size;
var timer = new Timer();

timer.start();
for (a1, a2) in zip(A1, A2) {
	a1 += a2;
}
timer.stop();
if printTiming then writeln("Serial 'zip': ", timer.elapsed());
timer.clear();

timer.start();
forall (a1, a2) in zip(A1, A2) {
	a1 += a2;
}
timer.stop();
if printTiming then writeln("Parallel 'zip': ", timer.elapsed());
timer.clear();

timer.start();
for (a1, a2) in zipper(A1, A2) {
	a1 += a2;
}
timer.stop();
if printTiming then writeln("Serial 'zipper': ", timer.elapsed());
timer.clear();

timer.start();
forall (a1, a2) in zipper(A1, A2) {
	a1 += a2;
}
timer.stop();
if printTiming then writeln("Parallel 'zipper': ", timer.elapsed());
timer.clear();

timer.start();
for idx in D {
	A1[idx] += A2[idx];
}
timer.stop();
if printTiming then writeln("Serial Domain Indexing: ", timer.elapsed());
timer.clear();

timer.start();
forall idx in D {
	A1[idx] += A2[idx];
}
timer.stop();
if printTiming then writeln("Parallel Domain Indexing: ", timer.elapsed());
timer.clear();

timer.start();
domainQuerySerial(A1, A2);
timer.stop();
if printTiming then writeln("Serial Domain Query Indexing: ", timer.elapsed());
timer.clear();

timer.start();
domainQueryParallel(A1, A2);
timer.stop();
if printTiming then writeln("Parallel Domain Query Indexing: ", timer.elapsed());
timer.clear();
