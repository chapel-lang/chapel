use Time;

/* NAIVE TESTS */
var myNaiveDateTime = dateTime.now();
var myNaiveTime = myNaiveDateTime.timetz();

var timeUtcOffset : timeDelta = myNaiveTime.utcOffset();
var timeDst : timeDelta = myNaiveTime.dst();
var timeTzname : string = myNaiveTime.tzname();

var dateTimeUtcOffset : timeDelta;
// utcOffset = myNaiveDateTime.utcOffset(); // halts
var dateTimeDst : timeDelta;
// dst = myNaiveDateTime.dst(); // halts
var dateTimeTzname : string = myNaiveDateTime.tzname();

// should get the same thing via time or dateTime
assert(timeUtcOffset == dateTimeUtcOffset);
assert(timeDst == dateTimeDst);
assert(timeTzname == dateTimeTzname);

// can't call myNaive[Date]Time.timezone.[method] here because timezone is nil

/* AWARE TESTS */
class FixedOffset: Timezone {
  var offset: timeDelta;
  var name: string;
  var dstoffset: timeDelta;

  proc init(offset: timeDelta, name: string, dstoffset: timeDelta=new timeDelta(42)) {
    this.offset = offset;
    this.name = name;
    this.dstoffset = dstoffset;
  }

  proc init(offset: int, name, dstoffset:int=42) {
    this.offset = new timeDelta(minutes=offset);
    this.name = name;
    this.dstoffset = new timeDelta(minutes=dstoffset);
  }

  override proc utcOffset(dt) {
    return offset;
  }
  override proc tzname(dt) {
    return name;
  }
  override proc dst(dt) {
    return dstoffset;
  }
}

var est = new shared FixedOffset(-300, "EST", 1);
var myAwareDateTime = dateTime.now(est);
var myAwareTime = myAwareDateTime.timetz();

timeUtcOffset = myAwareTime.utcOffset();
timeDst = myAwareTime.dst();
timeTzname = myAwareTime.tzname();

// compare against behavior of alternative
assert(timeUtcOffset == myAwareTime.timezone!.utcOffset(dateTime.now()));
assert(timeDst == myAwareTime.timezone!.dst(dateTime.now()));
assert(timeTzname == myAwareTime.timezone!.tzname(new dateTime()));

dateTimeUtcOffset = myAwareDateTime.utcOffset();
dateTimeDst = myAwareDateTime.dst();
dateTimeTzname = myAwareDateTime.tzname();

// should get the same thing via time or dateTime
assert(timeUtcOffset == dateTimeUtcOffset);
assert(timeDst == dateTimeDst);
assert(timeTzname == dateTimeTzname);

// compare against behavior of alternative
assert(dateTimeUtcOffset == myAwareDateTime.timezone!.utcOffset(dateTime.now()));
assert(dateTimeDst == myAwareDateTime.timezone!.dst(dateTime.now()));
assert(dateTimeTzname == myAwareDateTime.timezone!.tzname(new dateTime()));
