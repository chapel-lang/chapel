use IO;

const methionine = "ATG";

var next: string,
    mCount = 0;

var r = openreader("hbb.txt");

while r.readUntil(next, methionine, -1, true, false) {
    mCount += 1;
    writeln(next);
}
writeln(mCount);

const all = openreader("hbb.txt").readAll(string).split(methionine);
for a in all do writeln(a);
