use IO;

var r = openreader("hbb.txt");
const methionine = "ATG";

for cp in methionine.codepoints() {
    writeln(cp);
}

var next: string,
    mCount = 0;

while r.readUntil(next, methionine, -1, true, true) {
    mCount += 1;
    writeln(next);
}

writeln(mCount);
