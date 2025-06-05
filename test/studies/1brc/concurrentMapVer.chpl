use IO, Math, ParallelIO, ConcurrentMap, Sort, Time;

config const fileName = "measurements.txt",
             nTasks = here.maxTaskPar,
             printOutput = false,
             timeExecution = false;

proc main() {
    var t = new stopwatch();
    if timeExecution then t.start();

    // map to store temperature statistics for each city
    var cityTempStats = new ConcurrentMap(bytes, tempData);

    // read and aggregate temperature data for each city
    forall ct in readDelimited(fileName, t=cityTemp, delim="\n", nTasks=nTasks)
        with (var token = cityTempStats.getToken())
            do cityTempStats.update(ct.city, new adder(ct.temp), token);

    if timeExecution then writeln("elapsed time: ", t.elapsed());

    // print temperature statistics for each city
    if printOutput {
        var results = cityTempStats.toArray();
        sort(results, new comparator());
        for (city, td) in results do writef("%20s: %?", city.decode(), td);
    }
}

// record to store temperature stats for a particular city
record tempData: writeSerializable {
    var min: real = Types.max(real);
    var max: real = Types.min(real);
    var total: real = 0.0;
    var count: int = 0;
}

// how to add a temperature to a tempData record
inline operator tempData.+=(ref td: tempData, temp: real) {
    td.min = Math.min(td.min, temp);
    td.max = Math.max(td.max, temp);
    td.total += temp;
    td.count += 1;
}

// how to write a tempData record to a file (min/avg/max)
proc tempData.serialize(writer: fileWriter(?), ref serializer) throws {
    writer.writef("%7.1dr %7.1dr %7.1dr\n", this.min, this.total / this.count, this.max);
}

// record representing a <city>;<temperature> pair
record cityTemp: readDeserializable {
    var city: bytes;
    var temp: real;
}

// how to read a cityTemp record from a file
proc ref cityTemp.deserialize(reader: fileReader(?), ref deserializer) throws {
    this.city = reader.readThrough(b";", stripSeparator=true);
    this.temp = reader.read(real);
}

// used to facilitate atomic updates to tempData records
record adder {
    var temp: real;
    proc this(ref td: tempData) do td += this.temp;
}

// used to sort tempData records by city name
record comparator: keyComparator { }
proc comparator.key(k: (bytes, tempData)) { return k[0]; }
