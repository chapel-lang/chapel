use Time;

class FixedOffset: Timezone {
  var offset: timeDelta;
  var name: string;
  var dstoffset: timeDelta;
  proc init(offset: timeDelta, name: string, dstoffset: timeDelta = new timeDelta(minutes=42)) {
    this.offset = offset;
    this.name = name;
    this.dstoffset = dstoffset;
  }
  proc init(offset: int, name, dstoffset:int=42) {
    this.offset = new timeDelta(minutes=offset);
    this.name = name;
    this.dstoffset = new timeDelta(minutes=dstoffset);
  }

  override proc utcOffset(dt: dateTime(?)) {
    return offset;
  }
  override proc tzname(dt: dateTime(?)) {
    return name;
  }
  override proc dst(dt: dateTime(?)) {
    return dstoffset;
  }
  override proc fromUtc(dt: dateTime(?)) {
    var dtoff = dt.utcOffset();
    var dtdst = dt.dst();
    var delta = dtoff - dtdst;
    var dt2 = dt + delta;
    dtdst = dt2.dst();
    return dt2 + dtdst;
  }
}

config param tzCompareOperation : string;

const myTimezone = new shared FixedOffset(3, "MyTimezone");

var naiveDt : dateTime(tz_aware=false) = dateTime.now();
var awareDt : dateTime(tz_aware=true) = dateTime.now(myTimezone);

// Each of these should cause a compiler error, so we can only test one
// at a time, chosen via compiler flag.
select tzCompareOperation {
  when "lt" do writeln(naiveDt < awareDt);
  when "gt" do writeln(naiveDt > awareDt);
  when "leq" do writeln(naiveDt <= awareDt);
  when "geq" do writeln(naiveDt >= awareDt);
  when "eq" do writeln(naiveDt == awareDt);
  when "neq" do writeln(naiveDt == awareDt);
  otherwise do assert(false); // test invoked incorrectly
}
