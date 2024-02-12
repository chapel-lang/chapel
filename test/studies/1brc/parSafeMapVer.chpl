use IO, Time, Map, List, Sort, ParallelIO;

config const filename = "million_entries.txt";
config param parallel = true;
config const timeExecution = false;

// ----------------------------------------------------------------------------

record minner {
  const val: real;
  proc this(k: bytes, ref v: real) { v = min(this.val, v); return none; }
}

record maxxer {
  const val: real;
  proc this(k: bytes, ref v: real) { v = max(this.val, v); return none; }
}

record summer {
  const val: real;
  proc this(k: bytes, ref v: real) { v += this.val; return none; }
}

proc mapAddOrUpdate(ref m: map(bytes, real), k: bytes, v: real, type op) {
  if m.contains(k)
    then m.update(k, new op(v));
    else if !m.add(k, v) then m.update(k, new op(v));
}

// ----------------------------------------------------------------------------

record cityTemp : readDeserializable {
  var city : bytes;
  var temp : real;

  proc ref deserialize(reader: fileReader(?), ref deserializer) throws {
    this.city = reader.readThrough(b";", stripSeparator=true);
    this.temp = reader.read(real);
  }
}

var cityMin   : map(bytes,real,parSafe=parallel);
var cityMax   : map(bytes,real,parSafe=parallel);
var citySum   : map(bytes,real,parSafe=parallel);
var cityCount : map(bytes,real,parSafe=parallel);

var watch : stopwatch;

watch.start();

if(parallel) {
  forall ct in readDelimited(filename, t=cityTemp) with (ref cityMin, ref cityMax, ref citySum, ref cityCount) {
    mapAddOrUpdate(cityMin,   ct.city, ct.temp, minner);
    mapAddOrUpdate(cityMax,   ct.city, ct.temp, maxxer);
    mapAddOrUpdate(citySum,   ct.city, ct.temp, summer);
    mapAddOrUpdate(cityCount, ct.city, 1,       summer);
  }
} else {
  var reader = open(filename, ioMode.r).reader(locking=false);
  var ct : cityTemp;
  while reader.read(ct) {
    cityMin.addOrReplace(  ct.city, min(cityMin.get(ct.city, max(real)), ct.temp));
    cityMax.addOrReplace(  ct.city, max(cityMax.get(ct.city, min(real)), ct.temp));
    citySum.addOrReplace(  ct.city, citySum.get(ct.city, 0) + ct.temp);
    cityCount.addOrReplace(ct.city, cityCount.get(ct.city, 0)  + 1);
  }
}

watch.stop();

// Report results
var cities = cityMin.keys();
sort(cities);

for city in cities {
  writef("%20s: %7.1dr %7.1dr %7.1dr\n",
         city.decode(),
         cityMin.get(city,0),
         citySum.get(city,0) / cityCount.get(city,0),
         cityMax.get(city,0));
}
writeln();

if timeExecution then
  writeln("elapsed time: ", watch.elapsed());
