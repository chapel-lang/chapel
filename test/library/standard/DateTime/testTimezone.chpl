use Time;

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

proc test_empty() {
  var t = new time();
  assert(t.hour == 0);
  assert(t.minute == 0);
  assert(t.second == 0);
  assert(t.microsecond == 0);
  assert(t.timezone.borrow() == nil);
}

proc test_zones() {
  var est = new shared FixedOffset(-300, "EST", 1);
  var utc = new shared FixedOffset(0, "UTC", -2);
  var met = new shared FixedOffset(60, "MET", 3);
  var t1 = new time( 7, 47, tz=est);
  var t2 = new time(12, 47, tz=utc);
  var t3 = new time(13, 47, tz=met);
  var t4 = new time(microsecond=40);
  var t5 = new time(microsecond=40, tz=utc);

  assert(t1.timezone == est);
  assert(t2.timezone == utc);
  assert(t3.timezone == met);
  assert(t4.timezone.borrow() == nil);
  assert(t5.timezone == utc);

  assert(t1.utcOffset() == new timeDelta(minutes=-300));
  assert(t2.utcOffset() == new timeDelta(minutes=0));
  assert(t3.utcOffset() == new timeDelta(minutes=60));

  assert(t1.tzname() == "EST");
  assert(t2.tzname() == "UTC");
  assert(t3.tzname() == "MET");

  assert(t1.dst() == new timeDelta(minutes=1));
  assert(t2.dst() == new timeDelta(minutes=-2));
  assert(t3.dst() == new timeDelta(minutes=3));

  assert(t1 == t2);
  assert(t1 == t3);
  assert(t2 == t3);

/*
  assert(str(t1) == "07:47:00-05:00");
  assert(str(t2) == "12:47:00+00:00");
  assert(str(t3) == "13:47:00+01:00");
  assert(str(t4) == "00:00:00.000040");
  assert(str(t5) == "00:00:00.000040+00:00");
*/

  assert(t1.isoFormat() == "07:47:00-05:00");
  assert(t2.isoFormat() == "12:47:00+00:00");
  assert(t3.isoFormat() == "13:47:00+01:00");
  assert(t4.isoFormat() == "00:00:00.000040");
  assert(t5.isoFormat() == "00:00:00.000040+00:00");

  // %z conversion uses local timezone instead of the one in the tz
  //assert(t1.strftime("%H:%M:%S %%Z=%Z %%z=%z") ==
  //                             "07:47:00 %Z=EST %z=-0500");
  //assert(t2.strftime("%H:%M:%S %Z %z") == "12:47:00 UTC +0000");
  //assert(t3.strftime("%H:%M:%S %Z %z") == "13:47:00 MET +0100");
  //var yuck = new FixedOffset(-1439, "%z %Z %%z%%Z");
  //t1 = new time(23, 59, tz=yuck);
  //assert(t1.strftime("%H:%M %%Z='%Z' %%z='%z'") ==
  //                   "23:59 %Z='%z %Z %%z%%Z' %z='-2359'");
}

proc test_replace() {
  var z100 = new shared FixedOffset(100, "+100");
  var zm200 = new shared FixedOffset(new timeDelta(minutes=-200), "-200");
  var args = (1, 2, 3, 4);
  var base = new time((...args), z100);
  assert(base == base.replace(tz=base.timezone));

  var i = 0;
  for (name, newval) in (("hour", 5),
                         ("minute", 6),
                         ("second", 7),
                         ("microsecond", 8)) {
    var newargs = args;
    newargs[i] = newval;
    var expected = new time((...newargs), z100);
    var got: time;
    if name == "hour" then
      got = base.replace(hour=newval, tz=base.timezone);
    else if name == "minute" then
      got = base.replace(minute=newval, tz=base.timezone);
    else if name == "second" then
      got = base.replace(second=newval, tz=base.timezone);
    else if name == "microsecond" then
      got = base.replace(microsecond=newval, tz=base.timezone);

    assert(expected == got);
    i += 1;
  }

  {
    var newargs = args;
    var expected = new time((...newargs), zm200);
    var got = base.replace(tz=zm200);
    assert(expected == got);
  }

  // Ensure we can get rid of a tz.
  assert(base.tzname() == "+100");
  var base2 = base.replace(tz=nil);
  assert(base2.timezone.borrow() == nil);
  assert(base2.tzname() == "");

  // Ensure we can add one.
  var base3 = base2.replace(tz=z100);
  assert(base == base3);
  assert(base.timezone == base3.timezone);
}

proc test_mixed_compare() {
  var t1 = new time(1, 2, 3);
  var t2 = new time(1, 2, 3);
  assert(t1 == t2);
  t2 = t2.replace(tz=nil);
  assert(t1 == t2);
  t2 = t2.replace(tz=new shared FixedOffset(0, ""));

  // In time w/ identical tz objects, utcoffset is ignored.
  class Varies: Timezone {
    var offset: timeDelta;
    var name = "Var";
    proc init() {
      offset = new timeDelta(minutes=22);
    }
    override proc utcOffset(dt: dateTime) {
      offset += new timeDelta(minutes=1);
      return offset;
    }
    override proc dst(dt: dateTime) {
      return new timeDelta(0);
    }
    override proc tzname(dt: dateTime) {
      return name;
    }
  }

  var v = new shared Varies();
  t1 = t2.replace(tz=v);
  t2 = t2.replace(tz=v);
  assert(t1.utcOffset() == new timeDelta(minutes=23));
  assert(t2.utcOffset() == new timeDelta(minutes=24));
  assert(t1 == t2);

  // But if they're not identical, it isn't ignored.
  t2 = t2.replace(tz=new shared Varies());
  assert(t1 < t2);  // t1's offset counter still going up
}

test_empty();
test_zones();
test_replace();
test_mixed_compare();
