use DateTime;

class zone: TZInfo {
  /* The offset from UTC this class represents */
  proc utcoffset(dt: datetime): timedelta {
    return new timedelta();
  }

  /* The `timedelta` for daylight saving time */
  proc dst(dt: datetime): timedelta {
    return new timedelta();
  }

  /* The name of this time zone */
  proc tzname(dt: datetime): string {
    return "ZONE";
  }

  /* Convert a `time` in UTC to this time zone */
  proc fromutc(dt: datetime): datetime {
    return new datetime(0,0,0);
  }
}

const TZ = new shared zone?();

var t = new time(1,2,3,4,TZ);
