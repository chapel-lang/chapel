use IO, FileSystem;

config const fileName = "lines.txt",
             n = 10000;

const nsum = (n * (n + 1)) / 2;

proc createDataFile() {
    var w = openWriter(fileName);
    for i in 1..n {
        w.writeln(i);
    }
}

proc dataFileSum(region, snl, tls, type t = string): int {
    var sum = 0;
    var r = openReader(fileName, region=region);
    forall line in r.lines(stripNewline=snl, targetLocales=tls, t=t) with (+ reduce sum) {
        sum += line:int;
    }
    return sum;
}

createDataFile();

// whole file
assert(dataFileSum(0.., true, Locales) == nsum);
assert(dataFileSum(0.., true, [Locales.first,]) == nsum);
assert(dataFileSum(0.., true, [Locales.last,]) == nsum);

assert(dataFileSum(0.., false, Locales) == nsum);
assert(dataFileSum(0.., false, [Locales.first,]) == nsum);
assert(dataFileSum(0.., false, [Locales.last,]) == nsum);

assert(dataFileSum(0.., true, Locales, bytes) == nsum);
assert(dataFileSum(0.., true, [Locales.first,], bytes) == nsum);
assert(dataFileSum(0.., true, [Locales.last,], bytes) == nsum);

assert(dataFileSum(0.., false, Locales, bytes) == nsum);
assert(dataFileSum(0.., false, [Locales.first,], bytes) == nsum);
assert(dataFileSum(0.., false, [Locales.last,], bytes) == nsum);

// skipping first 9 lines
assert(dataFileSum(18.., true, Locales) == nsum - 45);
assert(dataFileSum(18.., true, [Locales.first,]) == nsum - 45);
assert(dataFileSum(18.., true, [Locales.last,]) == nsum - 45);

assert(dataFileSum(18.., false, Locales) == nsum - 45);
assert(dataFileSum(18.., false, [Locales.first,]) == nsum - 45);
assert(dataFileSum(18.., false, [Locales.last,]) == nsum - 45);

assert(dataFileSum(18.., true, Locales, bytes) == nsum - 45);
assert(dataFileSum(18.., true, [Locales.first,], bytes) == nsum - 45);
assert(dataFileSum(18.., true, [Locales.last,], bytes) == nsum - 45);

assert(dataFileSum(18.., false, Locales, bytes) == nsum - 45);
assert(dataFileSum(18.., false, [Locales.first,], bytes) == nsum - 45);
assert(dataFileSum(18.., false, [Locales.last,], bytes) == nsum - 45);

remove(fileName);
