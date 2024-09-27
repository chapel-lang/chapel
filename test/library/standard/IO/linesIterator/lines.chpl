use IO, FileSystem;

config const fileName = "lines.txt",
             n = 9999;

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


// skipping last 5 lines
//                    digit-len     + \n + ('region' not inclusive)
const nBytes = 5 * ((n:string).size + 1) + 1,
      fileLen = try! open(fileName, mode=ioMode.r).size,
      toRead = fileLen - nBytes,
      nsum2 = ((n - 5) * ((n - 5) + 1)) / 2;

assert(dataFileSum(..toRead, true, string, Locales) == nsum2);
assert(dataFileSum(..toRead, true, string, [Locales.first,]) == nsum2);
assert(dataFileSum(..toRead, true, string, [Locales.last,]) == nsum2);
assert(dataFileSum(..toRead, true, string, none) == nsum2);

assert(dataFileSum(..toRead, false, string, Locales) == nsum2);
assert(dataFileSum(..toRead, false, string, [Locales.first,]) == nsum2);
assert(dataFileSum(..toRead, false, string, [Locales.last,]) == nsum2);
assert(dataFileSum(..toRead, false, string, none) == nsum2);

assert(dataFileSum(..toRead, true, bytes, Locales) == nsum2);
assert(dataFileSum(..toRead, true, bytes, [Locales.first,]) == nsum2);
assert(dataFileSum(..toRead, true, bytes, [Locales.last,]) == nsum2);
assert(dataFileSum(..toRead, true, bytes, none) == nsum2);

assert(dataFileSum(..toRead, false, bytes, Locales) == nsum2);
assert(dataFileSum(..toRead, false, bytes, [Locales.first,]) == nsum2);
assert(dataFileSum(..toRead, false, bytes, [Locales.last,]) == nsum2);
assert(dataFileSum(..toRead, false, bytes, none) == nsum2);

remove(fileName);
