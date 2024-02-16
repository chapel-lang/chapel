use ParallelIO, IO, Map, FileSystem, Random;

const fileName = "things.txt";

config const n = 1000,
             nTasks = 4;

proc main() {
    const thingsActual = makeThingsFile(fileName, n);

    var things = new map(int, int, parSafe=true);
    forall t in readDelimited(fileName, t=thing, delim=",", nTasks=nTasks)
        with (ref things) do
            mapAddOrSet(things, t.k, t.v);

    // ensure the file was read correctly
    for (k, v) in zip(things.keys(), things.values()) do
        assert(thingsActual[k] == v);
}

record thing: serializable {
    var k: int;
    var v: int;

    proc ref deserialize(reader: fileReader(?), ref deserializer) throws {
        reader.readLiteral("{");
        this.k = reader.read(int);
        reader.readLiteral("->");
        this.v = reader.read(int);
        reader.readLiteral("}");
    }

    proc serialize(writer: fileWriter(?), ref serializer) throws {
        writer.write("{", k, "->", v, "}");
    }
}

proc makeThingsFile(path: string, n: int): map(int, int) {
    var m = new map(int, int),
        rng = new randomStream(int),
        f = open(path, ioMode.cwr),
        w = f.writer(locking=false);

    for 0..n {
        const t = new thing(rng.next(0, 100), rng.next(0, 100));
        m[t.k] += t.v;
        w.write(t, ",");
    }
    w.writeln();

    return m;
}

proc mapAddOrSet(ref m: map(int, int), k: int, v: int) {
    record adder {
        const val: int;

        proc this(k: int, ref v: int) {
            v += this.val;
            return none;
        }
    }

    if m.contains(k)
        then m.update(k, new adder(v)); // update 'v' in place
        else if !m.add(k, v)            // try to add 'k' and 'v'
            // another task added 'k' in the meantime, so update it instead
            then m.update(k, new adder(v));
}
