use Time;

class zone: Timezone {
  /* The offset from UTC this class represents */
  override proc utcOffset(dt: dateTime): timeDelta {
    return new timeDelta();
  }

  /* The `timeDelta` for daylight saving time */
  override proc dst(dt: dateTime): timeDelta {
    return new timeDelta();
  }

  /* The name of this time zone */
  override proc tzname(dt: dateTime): string {
    return "ZONE";
  }

  /* Convert a `time` in UTC to this time zone */
  override proc fromUtc(dt: dateTime): dateTime {
    return new dateTime(0,0,0);
  }
}

const TZ = new shared zone?();

var t = new time(1,2,3,4,TZ);
var dt = dateTime.now(TZ);

// unstable timezone convenience methods
t.utcOffset();
t.dst();
t.tzname();
dt.utcOffset();
dt.dst();
dt.tzname();
