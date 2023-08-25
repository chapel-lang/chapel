use Time;

class zone: Timezone {
  /* The offset from UTC this class represents */
  proc utcoffset(dt: dateTime): timeDelta {
    return new timeDelta();
  }

  /* The `timeDelta` for daylight saving time */
  proc dst(dt: dateTime): timeDelta {
    return new timeDelta();
  }

  /* The name of this time zone */
  proc tzname(dt: dateTime): string {
    return "ZONE";
  }

  /* Convert a `time` in UTC to this time zone */
  proc fromutc(dt: dateTime): dateTime {
    return new dateTime(0,0,0);
  }
}

const TZ = new shared zone?();

var t = new time(1,2,3,4,TZ);
