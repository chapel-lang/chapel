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

proc dataFileSum(region, tls): int {
    var sum = 0;
    var r = openReader(fileName, region=region);
    forall line in r.lines(targetLocales=tls) with (+ reduce sum) {
        sum += line:int;
    }
    return sum;
}

createDataFile();

// whole file
assert(dataFileSum(0.., Locales) == nsum);
assert(dataFileSum(0.., [Locales.first,]) == nsum);
assert(dataFileSum(0.., [Locales.last,]) == nsum);

// skipping first 9 lines
assert(dataFileSum(18.., Locales) == nsum - 45);
assert(dataFileSum(18.., [Locales.first,]) == nsum - 45);
assert(dataFileSum(18.., [Locales.last,]) == nsum - 45);

remove(fileName);
