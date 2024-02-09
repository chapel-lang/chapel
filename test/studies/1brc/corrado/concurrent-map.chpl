use IO, Math, ParallelIO, ConcurrentMap, Sort;

config const fileName = "measurements.txt",
             nTasks = 8;

proc main() {
    // map to store temperature statistics for each city
    var cityTempStats = new ConcurrentMap(bytes, tempData);

    // read and aggregate temperature data for each city
    forall ct in readDelimited(fileName, t=cityTemp, delim="\n", nTasks=nTasks)
        with (var token = cityTempStats.getToken())
            do cityTempStats.update(ct.city, new adder(ct.temp), token);

    // sort the results by city name
    var results = cityTempStats.toArray();
    sort(results, new comparator());

    // print the results
    var first = true;
    write("{");
    for (city, td) in results {
        if first then first = false; else write(", ");
        write(city, "=", td);
    }
    writeln("}");
}

// record to store temperature stats for a particular city
record tempData: writeSerializable {
    var min: real;
    var max: real;
    var total: real;
    var count: int;

    proc init(temp: real) {
        this.min = temp;
        this.max = temp;
        this.total = temp;
        this.count = 1;
    }

    proc init() {
        this.min = 0;
        this.max = 0;
        this.total = 0;
        this.count = 0;
    }
}

// how to add two tempData records together
inline operator tempData.+=(ref td: tempData, temp: real) {
    td.min = Math.min(td.min, temp);
    td.max = Math.max(td.max, temp);
    td.total += temp;
    td.count += 1;
}

// how to write a tempData record to a file (min/avg/max)
proc tempData.serialize(writer: fileWriter(?), ref serializer) throws {
    writer.writef("%.1dr/%.1dr/%.1dr", this.min, this.total / this.count, this.max);
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

record adder {
    var temp: real;
    proc this(ref td: tempData) do td += this.temp;
}

record comparator { }
proc comparator.key(k: (bytes, tempData)) { return k[0]; }
