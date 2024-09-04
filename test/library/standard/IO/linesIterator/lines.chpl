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

proc dataFileSum(region, stripNewline, type retType, targetLocs): int {
    var sum = 0;
    var r = openReader(fileName, region=region);

    if targetLocs.type == nothing {
        forall line in r.lines(stripNewline, retType) with (+ reduce sum) {
            sum += line:int;
        }
    } else {
        forall line in r.lines(stripNewline, retType, targetLocs) with (+ reduce sum) {
            sum += line:int;
        }
    }

    return sum;
}

createDataFile();

// whole file
assert(dataFileSum(0.., true, string, Locales) == nsum);
assert(dataFileSum(0.., true, string, [Locales.first,]) == nsum);
assert(dataFileSum(0.., true, string, [Locales.last,]) == nsum);
assert(dataFileSum(0.., true, string, none) == nsum);

assert(dataFileSum(0.., false, string, Locales) == nsum);
assert(dataFileSum(0.., false, string, [Locales.first,]) == nsum);
assert(dataFileSum(0.., false, string, [Locales.last,]) == nsum);
assert(dataFileSum(0.., false, string, none) == nsum);

assert(dataFileSum(0.., true, bytes, Locales) == nsum);
assert(dataFileSum(0.., true, bytes, [Locales.first,]) == nsum);
assert(dataFileSum(0.., true, bytes, [Locales.last,]) == nsum);
assert(dataFileSum(0.., true, bytes, none) == nsum);

assert(dataFileSum(0.., false, bytes, Locales) == nsum);
assert(dataFileSum(0.., false, bytes, [Locales.first,]) == nsum);
assert(dataFileSum(0.., false, bytes, [Locales.last,]) == nsum);
assert(dataFileSum(0.., false, bytes, none) == nsum);


// skipping first 9 lines
assert(dataFileSum(18.., true, string, Locales) == nsum - 45);
assert(dataFileSum(18.., true, string, [Locales.first,]) == nsum - 45);
assert(dataFileSum(18.., true, string, [Locales.last,]) == nsum - 45);
assert(dataFileSum(18.., true, string, none) == nsum - 45);

assert(dataFileSum(18.., false, string, Locales) == nsum - 45);
assert(dataFileSum(18.., false, string, [Locales.first,]) == nsum - 45);
assert(dataFileSum(18.., false, string, [Locales.last,]) == nsum - 45);
assert(dataFileSum(18.., false, string, none) == nsum - 45);

assert(dataFileSum(18.., true, bytes, Locales) == nsum - 45);
assert(dataFileSum(18.., true, bytes, [Locales.first,]) == nsum - 45);
assert(dataFileSum(18.., true, bytes, [Locales.last,]) == nsum - 45);
assert(dataFileSum(18.., true, bytes, none) == nsum - 45);

assert(dataFileSum(18.., false, bytes, Locales) == nsum - 45);
assert(dataFileSum(18.., false, bytes, [Locales.first,]) == nsum - 45);
assert(dataFileSum(18.., false, bytes, [Locales.last,]) == nsum - 45);
assert(dataFileSum(18.., false, bytes, none) == nsum - 45);

remove(fileName);
