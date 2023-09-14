/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */




/* Support for routines related to measuring the passing of time.

   This module provides support for querying local wall time or UTC time,
   and structures for manipulating dates and times. Note that timezone-naive
   local and UTC time querying methods will produce different results if the
   local time is not UTC, including potentially different calendar dates.

   It also implements a record :record:`~stopwatch` that can measure the
   execution time of sections of a program. The stopwatch has the potential for
   microsecond resolution and is intended to be useful for performance testing.
 */

module Time {
  import HaltWrappers;
  private use CTypes;

  // Returns the number of seconds since midnight.  Has the potential for
  // microsecond resolution if supported by the runtime platform
  private extern proc chpl_now_time():real;




  @chpldoc.nodoc
  // This is comparable to a Posix struct timeval
  extern type _timevalue;



  private extern proc chpl_null_timevalue(): _timevalue;



  // The number of seconds/microseconds since Jan 1, 1970 in UTC
  private extern proc chpl_now_timevalue():  _timevalue;



  // The components of time in the local time zone
  private extern proc chpl_timevalue_parts(t:           _timevalue,

                                           out seconds: int(32),
                                           out minutes: int(32),
                                           out hours:   int(32),
                                           out mday:    int(32),
                                           out month:   int(32),
                                           out year:    int(32),
                                           out wday:    int(32),
                                           out yday:    int(32),
                                           out isdst:   int(32));

  /* Specifies the units to be used when certain functions return a time */
  @deprecated(notes="The 'TimeUnits' type is deprecated. Please specify times in seconds in this module.")
  enum TimeUnits { microseconds, milliseconds, seconds, minutes, hours }

  /* Begin day-of-week enums */

  /* Controls whether :type:`dayOfWeek` starts with `Monday = 0` or
  `Monday = 1`.

    - If true, :type:`dayOfWeek` represents Monday as day 1.
    - If false, :type:`dayOfWeek` represents Monday as day 0. This behavior is
      deprecated and will be removed in an upcoming release.

    The deprecated behavior is on by default. To opt-in to the new behavior,
    recompile your program with ``-scIsoDayOfWeek=true``.

  */
  config param cIsoDayOfWeek = false;
  @chpldoc.nodoc
  param firstDayOfWeekNum = (if cIsoDayOfWeek then 1 else 0);
  // This enum has a compiler-implemented deprecation warning for change-of-behavior
  // controlled by `-scIsoDayOfWeek`, due to implementation issues with a
  // @deprecated conditional on a config value.
  /*
     Days in the week, starting with Monday.
     Monday is represented as 0 when :var:`cIsoDayOfWeek` is false (deprecated),
     or 1 otherwise (future default).

    .. warning::

      In an upcoming release 'dayOfWeek' will represent Monday as 1 instead of
      0 by default. During the deprecation period this behavior can be
      controlled with :var:`cIsoDayOfWeek`.
   */
  enum dayOfWeek {
    Monday = firstDayOfWeekNum,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
  }
  @chpldoc.nodoc
  enum _old_dayOfWeek {
    Monday = 0,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
  }
  @chpldoc.nodoc
  enum _iso_dayOfWeek {
    Monday = 1,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
  }

  /*
    Controls whether the :record:`time`, and :record:`dateTime` types represent
    their use of a :class:`Timezone` at runtime or as a part of their type.

    * When ``true``: The above types represent their timezone awareness as a
      static aspect of the type (stored in the ``tzAware`` param field). In
      this configuration, attempting to compare two ``time`` values with
      different timezone awareness results in a compilation error.

    * When ``false``: The above types represent their timezone awareness using
      the nil-ability of an internal field. When the timezone field is ``nil``,
      the value is not timezone aware. In this configuration, attempting to
      compare two ``time`` values with different timezone awareness results in
      a runtime halt.

  */
  config param DateTimeStaticTZAwareness = false;

  @deprecated(notes="enum 'Day' is deprecated. Please use :enum:`day` instead")
  enum Day       { sunday=0, monday, tuesday, wednesday, thursday, friday, saturday }
  /* Specifies the day of the week */
  @deprecated(notes="enum 'day' is deprecated. Please use :enum:`dayOfWeek` instead")
  enum day       { sunday=0, monday, tuesday, wednesday, thursday, friday, saturday }
  @chpldoc.nodoc
  proc DayOfWeek {
    compilerError("'DayOfWeek' was renamed. Please use 'dayOfWeek' instead");
  }
  /* Days in the week, starting with `Monday` = 1 */
  @deprecated(notes="enum 'isoDayOfWeek' is deprecated. Please use :enum:`dayOfWeek` instead")
  enum isoDayOfWeek {
    Monday =    1,
    Tuesday =   2,
    Wednesday = 3,
    Thursday =  4,
    Friday =    5,
    Saturday =  6,
    Sunday =    7
  }
  @chpldoc.nodoc
  proc ISODayOfWeek {
    compilerError("'ISODayOfWeek was renamed. Please use 'isoDayOfWeek' instead");
  }

  /* End day-of-week enums */

  /* The minimum year allowed in `date` objects */
  @deprecated("'MINYEAR' is deprecated; use `date.min.year` instead")
  param MINYEAR = 1;
  /* The maximum year allowed in `date` objects */
  @deprecated("'MAXYEAR' is deprecated; use `date.max.year` instead")
  param MAXYEAR = 9999;
  @chpldoc.nodoc
  param _MINYEAR = 1;
  @chpldoc.nodoc
  param _MAXYEAR = 9999;

  private const DAYS_IN_MONTH = (-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
  private const DAYS_BEFORE_MONTH = init_days_before_month();

  /* The Unix Epoch date and time */
  const unixEpoch = new dateTime(1970, 1, 1);

  private const DI400Y = daysBeforeYear(401);
  private const DI100Y = daysBeforeYear(101);
  private const DI4Y   = daysBeforeYear(5);

  private proc getTimeOfDay() {
    extern "struct timeval" record timeval {
      var tv_sec: int;
      var tv_usec: int;
    }
    extern proc gettimeofday(ref tv: timeval, tz): int;

    var tv: timeval;
    var ret = gettimeofday(tv, nil);
    assert(ret == 0);
    return (tv.tv_sec, tv.tv_usec);
  }

  private proc tm_zoneType type {
    use ChplConfig;
    if CHPL_TARGET_PLATFORM == "darwin" then
      return c_ptr(c_char); // char *
    else
      return c_ptrConst(c_char); // const char *
  }

  /* Get the `time` since Unix Epoch in seconds
  */
  proc timeSinceEpoch(): timeDelta {
    var (seconds,microseconds):(real,real) = getTimeOfDay();
    microseconds = microseconds/1000000.0;
    return new timeDelta(seconds + microseconds);
  }

  @chpldoc.nodoc
  extern "struct tm" record tm {
    var tm_sec:    c_int;         // seconds [0,61]
    var tm_min:    c_int;         // minutes [0,59]
    var tm_hour:   c_int;         // hour [0,23]
    var tm_mday:   c_int;         // day of month [1,31]
    var tm_mon:    c_int;         // month of year [0,11]
    var tm_year:   c_int;         // years since 1900
    var tm_wday:   c_int;         // day of week [0,6] (Sunday = 0)
    var tm_yday:   c_int;         // day of year [0,365]
    var tm_isdst:  c_int;         // daylight savings flag
    var tm_gmtoff: c_long;        // Seconds east of UTC
    var tm_zone:   tm_zoneType; // Timezone abbreviation
  }

  private proc assertInRange(val, low, high) {
    assert(val >= low && val <= high);
  }

  private proc getLocalTime(t: 2*int) {
    extern type time_t;

    extern proc localtime_r(const ref t: time_t, ref resultp: tm): void;

    const t1: time_t = __primitive("cast", time_t, t(0));
    var breakDownTime: tm;

    localtime_r(t1, breakDownTime);
    return breakDownTime;
  }

  private proc init_days_before_month() {
    var DBM: 13*int;
    for i in 2..12 {
      DBM[i] = DBM[i-1] + DAYS_IN_MONTH[i-1];
    }
    return DBM;
  }

  private proc daysBeforeYear(year: int) {
    const y = year - 1;
    return y*365 + y/4 - y/100 + y/400;
  }

  // assumes callee has checked for valid month range
  private proc daysBeforeMonth(year: int, month: int) {
    assertInRange(month, 1, 12);
    return DAYS_BEFORE_MONTH(month) + if (month > 2 && isLeapYear(year)) then 1 else 0;
  }

  // assumes callee has checked for valid month/day ranges
  private proc ymdToOrd(year: int, month: int, day: int) {
    assertInRange(month, 1, 12);
    const dim = try! daysInMonth(year, month);
    assertInRange(day, 1, dim);
    return daysBeforeYear(year) + daysBeforeMonth(year, month) + day;
  }

  private proc ordToYmd(in n: int) {
    n -= 1;
    const n400 = n / DI400Y;
    n = n % DI400Y;
    var year = n400*400 + 1;

    const n100 = n / DI100Y;
    n = n % DI100Y;

    const n4 = n / DI4Y;
    n = n % DI4Y;

    const n1 = n / 365;
    n = n % 365;

    year += n100*100 + n4*4 + n1;

    if n1 == 4 || n100 == 4 {
      assert(n == 0);
      return (year-1, 12, 31);
    }

    const leapyear = n1 == 3 && (n4 != 24 || n100 == 3);
    assert(leapyear == isLeapYear(year));

    var month = (n+50) >> 5;
    var preceding = daysBeforeMonth(year, month);
    if preceding > n {
      month -= 1;
      if month == 0 {
        month = 12;
        year -= 1;
      }
      const dim = try! daysInMonth(year, month);
      preceding -= dim;
    }
    n -= preceding;
    const dim = try! daysInMonth(year, month);
    assertInRange(n+1, 1, dim);
    return (year, month, n+1);
  }

  /* Return true if `year` is a leap year */
  proc isLeapYear(year: int) : bool {
    return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
  }

  /* Return the number of days in month `month` during the year `year`.
     The number for a month can change from year to year due to leap years.

     :throws IllegalArgumentError: Thrown if `month` is out of range.
*/
  proc daysInMonth(year: int, month: int) : int throws {
    if month < 1 || month > 12 then
      throw new owned IllegalArgumentError("month must be between 1 and 12");
    if month == 2 && isLeapYear(year) then
      return 29;
    else
      return DAYS_IN_MONTH(month);
  }

/* A record representing a date */
  record date {
    var chpl_year, chpl_month, chpl_day: int;

    /* The year represented by this `date` value */
    proc year : int {
      return chpl_year;
    }

    /* The month represented by this `date` value */
    proc month : int {
      return chpl_month;
    }

    /* The day represented by this `date` value */
    proc day : int {
      return chpl_day;
    }

    /* The minimum representable `date` */
    proc type min : date {
      return new date(_MINYEAR, 1, 1);
    }

    /* The maximum representable `date` */
    proc type max : date {
      return new date(_MAXYEAR, 12, 31);
    }

    /* The minimum non-zero difference between two dates */
    proc type resolution : timeDelta {
      return new timeDelta(days=1);
    }
  }


  /* initializers/factories for date values */

  @chpldoc.nodoc
  @unstable("initializing a 'date' without arguments is unstable; it may become illegal or be replaced with a specific uninitialized 'date' sentinel value in the future")
  proc date.init() {
  }

  /* Initialize a new `date` value from a `year`, `month`, and `day`. All
     three arguments are required and must be in valid ranges.  The
     valid ranges are:

     1 <= `year` <= 9999

     1 <= `month` <= 12

     1 <= `day` <= the number of days in the given month and year
  */
  proc date.init(year: int, month: int, day: int) {
    if year < _MINYEAR-1 || year > _MAXYEAR+1 then
      HaltWrappers.initHalt("year is out of the valid range");
    if month < 1 || month > 12 then
      HaltWrappers.initHalt("month is out of the valid range");
    const dim = try! daysInMonth(year, month);
    if day < 1 || day > dim then
      HaltWrappers.initHalt("day is out of the valid range");

    this.chpl_year = year;
    this.chpl_month = month;
    this.chpl_day = day;
  }

  /* A `date` object representing the current day, using naive local time */
  proc type date.today() : date {
    const timeSinceEpoch = getTimeOfDay();
    const td = new timeDelta(seconds=timeSinceEpoch(0),
                             microseconds=timeSinceEpoch(1));

    return unixEpoch.getDate() + td;
  }

  /* A `date` object representing the current day, using naive UTC time */
  proc type date.utcToday() : date {
    var now = chpl_now_timevalue();

    var seconds, minutes, hours, mday, month, year, wday, yday, isdst:int(32);

    chpl_timevalue_parts(now, seconds, minutes, hours, mday, month, year, wday, yday, isdst);

    return new date(year + 1900, month + 1, mday);
  }

  @deprecated(notes="'date.fromTimestamp' is deprecated, please use 'dateTime.createUtcFromTimestamp().getDate()' instead")
  proc type date.fromTimestamp(timestamp) : date {
    return date.createFromTimestamp(timestamp);
  }

  /* The date that is `timestamp` seconds from the epoch */
  @deprecated(notes="'date.createFromTimestamp' is deprecated, please use 'dateTime.createUtcFromTimestamp().getDate()' instead")
  proc type date.createFromTimestamp(timestamp: real) : date {
    const sec = timestamp: int;
    const us = ((timestamp-sec) * 1000000 + 0.5): int;
    const td = new timeDelta(seconds=sec, microseconds=us);
    return unixEpoch.getDate() + td;
  }

  @deprecated(notes="'date.fromOrdinal' is deprecated, please use 'date.createFromOrdinal' instead")
  proc type date.fromOrdinal(ord) : date {
    return date.createFromOrdinal(ord);
  }

  /* The `date` that is `ordinal` days from 1-1-0001 */
  proc type date.createFromOrdinal(ordinal: int) : date {
    if ordinal < 0 || ordinal > 1+date.max.toOrdinal() then
      halt("ordinal (", ordinal, ") out of range");
    const (y,m,d) = ordToYmd(ordinal);
    return new date(y,m,d);
  }

  /* Methods on date values */

  /* Get a new `date` based on this one, optionally with the `year`, `month`
     and/or `day` replaced.
  */
  proc date.replace(year=-1, month=-1, day=-1) : date {
    const newYear = if year != -1 then year else this.year;
    const newMonth = if month != -1 then month else this.month;
    const newDay = if day != -1 then day else this.day;
    return new date(newYear, newMonth, newDay);
  }

  /* Return a filled record matching the C `struct tm` type for the given date */
  @unstable("'date.timetuple' is unstable")
  proc date.timetuple() : tm {
    var timeStruct: tm;

    timeStruct.tm_hour = 0;
    timeStruct.tm_min = 0;
    timeStruct.tm_sec = 0;

    timeStruct.tm_mday = day: int(32);
    timeStruct.tm_mon = month: int(32);
    timeStruct.tm_year = year: int(32);
    timeStruct.tm_wday = this._old_weekday(): int(32);
    timeStruct.tm_yday = (toOrdinal() - (new date(year, 1, 1)).toOrdinal() + 1): int(32);
    timeStruct.tm_isdst = (-1): int(32);
    return timeStruct;
  }

  /* Return the number of days since 1-1-0001 this `date` represents */
  proc date.toOrdinal() : int {
    return ymdToOrd(year, month, day);
  }

  /* Return the day of the week.
   */
  proc date.weekday() : dayOfWeek where cIsoDayOfWeek {
    // January 1 0001 is a Monday
    return try! (((toOrdinal() + 6) % 7) + 1): dayOfWeek;
  }
  @deprecated(notes="The version of 'date.weekday' returning a :enum:`dayOfWeek` starting with `Monday = 0` is deprecated. Recompile with ``-sCIsoDayOfWeek=true`` to opt in to the new behavior of `Monday = 1`")
  proc date.weekday() : dayOfWeek where !cIsoDayOfWeek {
    return try! this._old_weekday():int:dayOfWeek;
  }
  @chpldoc.nodoc
  proc date._old_weekday() : _old_dayOfWeek {
    // January 1 0001 is a Monday
    return try! (((toOrdinal() + 6) % 7)):_old_dayOfWeek;
  }

  /* Return the day of the week as an `isoDayOfWeek`.
     `Monday` == 1, `Sunday` == 7 */
  @deprecated(notes="'date.isoWeekday' is deprecated; use :proc:`date.weekday` instead")
  proc date.isoWeekday() : isoDayOfWeek {
    var offsetForIso = (if cIsoDayOfWeek then 0 else 1);
    return try! (weekday(): int + offsetForIso): isoDayOfWeek;
  }

  /* Return the ISO week date as a tuple containing the ISO week-numbering year,
     ISO week number, and ISO weekday number.
   */
  proc date.isoWeekDate() : (int, int, int) {
    proc findThursday(d: date) {
      var wd = d._old_weekday();
      return d + new timeDelta(days = (_old_dayOfWeek.Thursday:int - wd:int));
    }

    proc findyear(d: date) {
      return findThursday(d).year;
    }

    proc findFirstDayOfYear(year) {
      var thu = findThursday((new date(year, 1, 1)));
      if thu.year < year {
        return thu + new timeDelta(days=4);
      } else { // thu.year == year
        return thu + new timeDelta(days=-3);
      }
    }

    const y = findyear(this);
    const firstDay = findFirstDayOfYear(y);
    const delta = this - firstDay;

    return (y, 1+delta.days/7, (_old_weekday(): int) + 1);
  }

  /* Return the ISO week date as a tuple containing the ISO week-numbering year,
     ISO week number, and ISO weekday number.
   */
  @deprecated(notes="`date.isoCalendar` is deprecated; use :proc:`date.isoWeekDate` instead")
  proc date.isoCalendar() : (int, int, int) {
    return this.isoWeekDate();
  }

  /* Get a `string` representation of this `date` in ISO format
     ``YYYY-MM-DD``.
  */
  operator date.:(x: date, type t: string) {
    var yearstr = x.year: string;
    var monthstr = x.month: string;
    var daystr = x.day: string;

    if x.year < 10 then
      yearstr = "000" + yearstr;
    else if x.year < 100 then
      yearstr = "00" + yearstr;
    else if x.year < 1000 then
      yearstr = "0" + yearstr;

    if x.month < 10 then
      monthstr = "0" + monthstr;
    if x.day < 10 then
      daystr = "0" + daystr;

    return yearstr + "-" + monthstr + "-" + daystr;
  }

  /* Return the date as a `string` in ISO 8601 format: "YYYY-MM-DD" */
  @deprecated(notes="`date.isoFormat` is deprecated; use cast to string instead")
  proc date.isoFormat() : string {
    return this:string;
  }

  /* Return a `string` representing the date */
  @unstable("'date.ctime' is unstable")
  proc date.ctime() : string {
    const month = strftime("%b");
    const wday = strftime("%a");

    const extraSpace = if day < 10 then " " else "";
    return wday + " " + month +
           " " + extraSpace + day:string + " 00:00:00 " + year: string;
  }

  /* Return a formatted `string` matching the `format` argument and the date */
  @unstable("'date.strftime' is unstable")
  proc date.strftime(fmt: string) : string {
    extern proc strftime(s: c_ptr(void), size: c_size_t, format: c_ptrConst(c_char), ref timeStruct: tm);
    const bufLen: c_size_t = 100;
    var buf: [1..bufLen] c_char;
    var timeStruct: tm;

    timeStruct.tm_sec = 0;
    timeStruct.tm_min = 0;
    timeStruct.tm_hour = 0;
    timeStruct.tm_isdst = 0;
    timeStruct.tm_gmtoff = 0;
    timeStruct.tm_zone = nil;

    timeStruct.tm_year = (year-1900): int(32); // 1900 based
    timeStruct.tm_mon = (month-1): int(32);    // 0 based
    timeStruct.tm_mday = day: int(32);
    timeStruct.tm_wday = (_old_weekday(): int(32) + 1) % 7; // shift Sunday to 0
    timeStruct.tm_yday = (this - new date(year, 1, 1)).days: int(32);

    strftime(c_ptrTo(buf), bufLen, fmt.c_str(), timeStruct);
    var str: string;
    try! {
      str = string.createCopyingBuffer(c_ptrTo(buf));
    }
    return str;
  }

  private use IO;

  // This method exists to work around a bug in chpldoc where the
  // 'private use' above this method somehow breaks documentation for the
  // method that follows (formerly 'writeThis')
  @chpldoc.nodoc
  proc date._chpldoc_workaround() { }

  /* Writes this `date` formatted as ``YYYY-MM-DD`` */
  proc date.writeThis(f) throws {
    f.write("%?".format(this));
  }

  // Exists to support some common functionality for `dateTime.readThis`
  @chpldoc.nodoc
  proc ref date._readCore(f) throws {
    const dash = "-";

    chpl_year = f.read(int);
    f._readLiteral(dash);
    chpl_month = f.read(int);
    f._readLiteral(dash);
    chpl_day = f.read(int);
  }

  /* Reads this `date` with the same format used by :proc:`date.writeThis` */
  proc ref date.readThis(f) throws {
    import JSON.jsonDeserializer;

    const binary = f._binary(),
          arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY),
          isjson = (arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary) ||
            isSubtype(f.deserializerType, jsonDeserializer);

    if isjson then
      f._readLiteral('"');

    this._readCore(f);

    if isjson then
      f._readLiteral('"');
  }

  //
  // TODO: need to get this to work with the Json formatter
  //
  @chpldoc.nodoc
  proc date.init(reader: fileReader, ref deserializer) throws {
    this.init();
    readThis(reader);
  }

  /* Operators on date values */
  @chpldoc.nodoc
  operator date.+(d: date, t: timeDelta): date {
    return date.createFromOrdinal(d.toOrdinal() + t.days);
  }

  @chpldoc.nodoc
  operator date.+(t: timeDelta, d: date): date {
    return d + t;
  }

  @chpldoc.nodoc
  operator date.-(d: date, t: timeDelta): date {
    return date.createFromOrdinal(d.toOrdinal() - t.days);
  }

  @chpldoc.nodoc
  operator date.-(d1: date, d2: date): timeDelta {
    return new timeDelta(days=d1.toOrdinal() - d2.toOrdinal());
  }

  @chpldoc.nodoc
  operator date.<(d1: date, d2: date) : bool {
    return d1.toOrdinal() < d2.toOrdinal();
  }

  @chpldoc.nodoc
  operator date.<=(d1: date, d2: date) : bool {
    return d1.toOrdinal() <= d2.toOrdinal();
  }

  @chpldoc.nodoc
  operator date.>(d1: date, d2: date) : bool {
    return d1.toOrdinal() > d2.toOrdinal();
  }

  @chpldoc.nodoc
  operator date.>=(d1: date, d2: date) : bool {
    return d1.toOrdinal() >= d2.toOrdinal();
  }


  /* A record representing a time */
  record time {
    /*
      Whether this ``time`` has an associated :class:`Timezone`.

      Currently only applicable when :param:`DateTimeStaticTZAwareness` is ``true``.
    */
    param tzAware: bool = false;

    var chpl_hour, chpl_minute, chpl_second, chpl_microsecond: int;
    var chpl_tz: shared Timezone?;

    /* The hour represented by this `time` value */
    proc hour : int {
      return chpl_hour;
    }

    /* The minute represented by this `time` value */
    proc minute : int {
      return chpl_minute;
    }

    /* The second represented by this `time` value */
    proc second : int {
      return chpl_second;
    }

    /* The microsecond represented by this `time` value */
    proc microsecond : int {
      return chpl_microsecond;
    }

    /* The timezone represented by this `time` value */
    @deprecated(notes="The ``timezone`` query that returns a nilable value is deprecated; please recompile with '-sDateTimeStaticTZAwareness=true' to make timezone awareness a feature of the ``time`` type")
    proc timezone: shared Timezone?
      where DateTimeStaticTZAwareness == false
        do return chpl_tz;

    proc timezone: shared Timezone
      where DateTimeStaticTZAwareness == true && this.tzAware == true
        do return try!(chpl_tz: shared Timezone);

    @chpldoc.nodoc
    proc timezone: shared Timezone
      where DateTimeStaticTZAwareness == true && tzAware == false
        do compilerError("The 'timezone' query is only availible on a 'time' with 'tzAware=true'");

    // for internal use during deprecation. Replace uses with `timezone` after nil-returning version is removed
    @chpldoc.nodoc
    proc _timezone: shared Timezone? where DateTimeStaticTZAwareness == false do return chpl_tz;
    @chpldoc.nodoc
    proc _timezone: shared Timezone where DateTimeStaticTZAwareness == true do return try!(chpl_tz: shared Timezone);

    @chpldoc.nodoc
    @deprecated(notes="'tzinfo' is deprecated, please use 'timezone' instead")
    proc tzinfo {
      return timezone;
    }

    /* The minimum representable `time` */
    proc type min : time(false) {
      return new time(false);
    }

    /* The maximum representable `time` */
    proc type max : time(false) {
      return new time(23, 59, 59, 999999);
    }

    /* The minimum non-zero difference between two times */
    proc type resolution : timeDelta {
      return new timeDelta(microseconds=1);
    }
  }

  /* initializers/factories for time values */

  @chpldoc.nodoc
  @unstable("initializing a 'time' without arguments is unstable; it may become illegal or be replaced with a specific uninitialized 'time' sentinel value in the future")
  proc time.init(param tzAware: bool) {
    this.tzAware = tzAware;
  }

  /* Initialize a new `time` value from the given `hour`, `minute`, `second`,
     `microsecond`, and `timezone`.  All arguments are optional
   */
  @unstable("tz is unstable; its type may change in the future")
  proc time.init(hour:int=0, minute:int=0, second:int=0, microsecond:int=0,
                 in tz: shared Timezone) where DateTimeStaticTZAwareness == true {
    this.init(hour, minute, second, microsecond, tz, internal=true);
  }

  // Temporary helper for use during DateTimeStaticTZAwareness deprecation
  //  This should be removed, and its implementation moved into the above
  //  initializer when the deprecation is complete. Internal calls to 'new time'
  //  should remove their: `internal=true` argument.
  @chpldoc.nodoc
  proc time.init(hour:int=0, minute:int=0, second:int=0, microsecond:int=0,
                 in tz: shared Timezone, internal: bool)
  where DateTimeStaticTZAwareness == true {
    this.tzAware = true;
    if hour < 0 || hour >= 24 then
      HaltWrappers.initHalt("hour out of range");
    if minute < 0 || minute >= 60 then
      HaltWrappers.initHalt("minute out of range");
    if second < 0 || second >= 60 then
      HaltWrappers.initHalt("second out of range");
    if microsecond < 0 || microsecond >= 1000000 then
      HaltWrappers.initHalt("microsecond out of range");
    this.chpl_hour = hour;
    this.chpl_minute = minute;
    this.chpl_second = second;
    this.chpl_microsecond = microsecond;
    this.chpl_tz = tz;
  }

  @deprecated(notes="The 'time' initializer that accepts a nilable 'tz' is deprecated; please recompile with -sDateTimeStaticTZAwareness=true to use the overload that accepts a non-nilable 'tz'")
  proc time.init(hour:int=0, minute:int=0, second:int=0, microsecond:int=0,
                 in tz: shared Timezone?)
  where DateTimeStaticTZAwareness == false {
    this.init(hour, minute, second, microsecond, tz, internal=true);
  }

  // Temporary helper for use during DateTimeStaticTZAwareness deprecation
  //  This helper and the above initailizer should be removed once the
  //  deprecation is complete. Internal calls to 'new time' should remove
  //  their: `internal=true` argument.
  @chpldoc.nodoc
  proc time.init(hour:int=0, minute:int=0, second:int=0, microsecond:int=0,
                 in tz: shared Timezone?, internal: bool)
  where DateTimeStaticTZAwareness == false {
    if hour < 0 || hour >= 24 then
      HaltWrappers.initHalt("hour out of range");
    if minute < 0 || minute >= 60 then
      HaltWrappers.initHalt("minute out of range");
    if second < 0 || second >= 60 then
      HaltWrappers.initHalt("second out of range");
    if microsecond < 0 || microsecond >= 1000000 then
      HaltWrappers.initHalt("microsecond out of range");
    this.chpl_hour = hour;
    this.chpl_minute = minute;
    this.chpl_second = second;
    this.chpl_microsecond = microsecond;
    this.chpl_tz = tz;
  }

  /* Initialize a new `time` value from the given `hour`, `minute`, `second`,
     `microsecond`.  All arguments are optional
   */
  proc time.init(hour:int=0, minute:int=0, second:int=0, microsecond:int=0) {
    this.tzAware = false;
    if hour < 0 || hour >= 24 then
      HaltWrappers.initHalt("hour out of range");
    if minute < 0 || minute >= 60 then
      HaltWrappers.initHalt("minute out of range");
    if second < 0 || second >= 60 then
      HaltWrappers.initHalt("second out of range");
    if microsecond < 0 || microsecond >= 1000000 then
      HaltWrappers.initHalt("microsecond out of range");
    this.chpl_hour = hour;
    this.chpl_minute = minute;
    this.chpl_second = second;
    this.chpl_microsecond = microsecond;
    this.chpl_tz = nil;
  }

  /* Initialize a new `time` value from the given `hour`, `minute`, `second`,
     `microsecond`, and `timezone`.  All arguments are optional
   */

  @chpldoc.nodoc
  proc time.deinit() {
  }

  /* Methods on time values */

  /* Get a new `time` based on this one, optionally with the `hour` `minute`,
     `second`, and/or `microsecond` replaced.
  */
  proc time.replace(hour=-1, minute=-1, second=-1, microsecond=-1) : time(false) {
    const newhour = if hour != -1 then hour else this.hour;
    const newminute = if minute != -1 then minute else this.minute;
    const newsecond = if second != -1 then second else this.second;
    const newmicrosecond = if microsecond != -1 then microsecond else this.microsecond;

    return new time(newhour, newminute, newsecond, newmicrosecond);
  }

  /* Get a new `time` based on this one, optionally with the `hour` `minute`,
     `second`, `microsecond` and/or `tz` replaced.
  */
  @unstable("tz is unstable; its type may change in the future")
  proc time.replace(hour=-1, minute=-1, second=-1, microsecond=-1,
                    in tz: shared Timezone) : time(true)
    where DateTimeStaticTZAwareness == true
  {
    const newhour = if hour != -1 then hour else this.hour;
    const newminute = if minute != -1 then minute else this.minute;
    const newsecond = if second != -1 then second else this.second;
    const newmicrosecond = if microsecond != -1 then microsecond else this.microsecond;

    return new time(newhour, newminute, newsecond, newmicrosecond, tz, internal=true);
  }

  @unstable("tz is unstable; its type may change in the future")
  proc time.replace(hour=-1, minute=-1, second=-1, microsecond=-1,
                    in tz: shared Timezone?) : time
    where DateTimeStaticTZAwareness == false
  {
    const newhour = if hour != -1 then hour else this.hour;
    const newminute = if minute != -1 then minute else this.minute;
    const newsecond = if second != -1 then second else this.second;
    const newmicrosecond = if microsecond != -1 then microsecond else this.microsecond;

    return new time(newhour, newminute, newsecond, newmicrosecond, tz, internal=true);
  }

  /* Get a `string` representation of this `time` in ISO format
     ``hh:mm:ss.ssssss``, followed by ``±hh:mm`` if a timezone is specified.
  */
  operator time.:(x: time(?), type t: string) {
    proc makeNDigits(n, d) {
      var ret = d: string;
      while ret.size < n {
        ret = "0" + ret;
      }
      return ret;
    }

    var ret = makeNDigits(2, x.hour) + ":" +
              makeNDigits(2, x.minute) + ":" +
              makeNDigits(2, x.second);

    if x.microsecond != 0 {
      ret = ret + "." + makeNDigits(6, x.microsecond);
    }
    var offset = x.utcOffset();

    if DateTimeStaticTZAwareness && tzAware ||
      x._timezone.borrow() != nil
    {
      var sign: string;
      if offset.days < 0 {
        offset = -offset;
        sign = "-";
      } else {
        sign = "+";
      }
      ret = ret + sign + makeNDigits(2, offset.seconds/(60*60)) + ":" +
                         makeNDigits(2, offset.seconds % (60*60) / 60);
    }

    return ret;
  }

  @deprecated(notes="`time.isoFormat` is deprecated; use cast to string instead")
  proc time.isoFormat() : string {
    return this:string;
  }

  /* Return the offset from UTC */
  @unstable("'utcOffset' is unstable")
  proc time.utcOffset() : timeDelta {
    if DateTimeStaticTZAwareness {
      if tzAware
        then return this._timezone.utcOffset(dateTime.now());
        else return new timeDelta();
    } else {
      if this._timezone.borrow() == nil
        then return new timeDelta();
        else return this._timezone!.utcOffset(dateTime.now());
    }
  }

  /* Return the daylight saving time offset */
  @unstable("'dst' is unstable")
  proc time.dst() : timeDelta {
    if DateTimeStaticTZAwareness {
      if tzAware
        then return this._timezone.dst(dateTime.now());
        else return new timeDelta();
    } else {
      if this._timezone.borrow() == nil
        then return new timeDelta();
        else return this._timezone!.dst(dateTime.now());
    }
  }

  /* Return the name of the timezone for this `time` value */
  @unstable("'tzname' is unstable")
  proc time.tzname() : string {
    if DateTimeStaticTZAwareness {
      if tzAware
        then return this._timezone.tzname(new dateTime(1,1,1));
        else return "";
    } else {
      if this._timezone.borrow() == nil
        then return "";
        else return this._timezone!.tzname(new dateTime(1,1,1));
    }
  }

  /* Return a `string` matching the `format` argument for this `time` */
  @unstable("'time.strftime' is unstable")
  proc time.strftime(fmt: string) : string {
    extern proc strftime(s: c_ptr(void), size: c_size_t, format: c_ptrConst(c_char), ref timeStruct: tm);
    const bufLen: c_size_t = 100;
    var buf: [1..bufLen] c_char;
    var timeStruct: tm;

    timeStruct.tm_sec = second: int(32);
    timeStruct.tm_min = minute: int(32);
    timeStruct.tm_hour = hour: int(32);
    timeStruct.tm_year = 0;
    timeStruct.tm_mday = 1;
    timeStruct.tm_mon = 1;

    timeStruct.tm_wday = ((new date(1900, 1, 1))._old_weekday():int(32) + 1) % 7;
    timeStruct.tm_yday = 0;

    if timezone.borrow() != nil {
      timeStruct.tm_gmtoff = utcOffset().abs().seconds: c_long;
      timeStruct.tm_zone = __primitive("cast", tm_zoneType, tzname().c_str());
      timeStruct.tm_isdst = dst().seconds: int(32);
    } else {
      timeStruct.tm_gmtoff = 0;
      timeStruct.tm_zone = __primitive("cast", tm_zoneType, "".c_str());
      timeStruct.tm_isdst = -1;
    }

    strftime(c_ptrTo(buf), bufLen, fmt.c_str(), timeStruct);
    var str: string;
    try! {
      str = string.createCopyingBuffer(c_ptrTo(buf));
    }

    return str;
  }

  /* Writes this `time` formatted as  ``hh:mm:ss.ssssss``,
     followed by ``±hh:mm`` if a timezone is specified
   */
  proc time.writeThis(f) throws {
    f.write(this:string);
  }

  // Exists to support some common functionality for `dateTime.readThis`
  @chpldoc.nodoc
  proc ref time._readCore(f) throws {
    const colon = ":";

    chpl_hour = f.read(int);
    f._readLiteral(colon);
    chpl_minute = f.read(int);
    f._readLiteral(colon);
    chpl_second = f.read(int);
    f._readLiteral(".");
    chpl_microsecond = f.read(int);
  }

  /* Reads this `time` with the same format used by :proc:`time.writeThis` */
  proc ref time.readThis(f) throws {
    import JSON.jsonDeserializer;

    const binary = f._binary(),
          arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY),
          isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary  ||
            isSubtype(f.deserializerType, jsonDeserializer);

    if isjson then
      f._readLiteral('"');

    this._readCore(f);

    if isjson then
      f._readLiteral('"');
  }

  //
  // TODO: need to get this to work with the Json formatter
  //
  @chpldoc.nodoc
  proc time.init(reader: fileReader, ref deserializer) throws {
    this.init();
    readThis(reader);
  }


  /* Operators on time values */

  @chpldoc.nodoc
  operator time.==(t1: time(?), t2: time(?)): bool {
    var dt1 = new dateTime(d=new date(2000, 1, 1), t=t1);
    var dt2 = new dateTime(d=new date(2000, 1, 1), t=t2);
    return dt1 == dt2;
  }

  @chpldoc.nodoc
  operator time.!=(t1: time(?), t2: time(?)) : bool {
    return !(t1 == t2);
  }

  @chpldoc.nodoc
  operator time.<(t1: time(?), t2: time(?)): bool {
    if DateTimeStaticTZAwareness && (t1.tzAware != t2.tzAware)
      then compilerError("both 'time' values must have the same timezone awareness for comparison");

    if !DateTimeStaticTZAwareness && ((t1._timezone.borrow() == nil) != (t2._timezone.borrow() == nil)) {
      halt("both 'time' values must have the same timezone awareness for comparison");
    } else if t1._timezone == t2._timezone {
      const sec1 = t1.hour*3600 + t1.minute*60 + t1.second;
      const usec1 = t1.microsecond;
      const sec2 = t2.hour*3600 + t2.minute*60 + t2.second;
      const usec2 = t2.microsecond;
      if sec1 < sec2 then
        return true;
      else if sec1 == sec2 then
        return usec1 < usec2;
      else
        return false;
    } else {
      // As far as I can tell, python's dateTime.time() comparisons don't
      // pay attention to the timezones.
      // >>> central = pytz.timezone("US/Central")
      // >>> pacific = pytz.timezone("US/Pacific")
      // >>> datetime.time(12,3,4,5,tz=central) >
      //     datetime.time(12,3,4,5,tz=pacific)
      // False
      // >>> datetime.time(12,3,4,6,tz=central) >
      //     datetime.time(12,3,4,5,tz=pacific)
      // True
      //
      // This compares the time on a specific date, and factors in the
      // time zones.
      const dt1 = new dateTime(new date(1900, 1, 1), t1);
      const dt2 = new dateTime(new date(1900, 1, 1), t2);
      return dt1 < dt2;
      //return (t1.replace(tz=nil) - t1.utcOffset()) <
      //       (t2.replace(tz=nil) - t2.utcOffset());
    }
  }

  @chpldoc.nodoc
  operator time.<=(t1: time(?), t2: time(?)): bool {
    if DateTimeStaticTZAwareness && (t1.tzAware != t2.tzAware)
      then compilerError("both 'time' values must have the same timezone awareness for comparison");

    if !DateTimeStaticTZAwareness && ((t1._timezone.borrow() == nil) != (t2._timezone.borrow() == nil)) {
      halt("both 'time' values must have the same timezone awareness for comparison");
    } else if t1._timezone == t2._timezone {
      const sec1 = t1.hour*3600 + t1.minute*60 + t1.second;
      const usec1 = t1.microsecond;
      const sec2 = t2.hour*3600 + t2.minute*60 + t2.second;
      const usec2 = t2.microsecond;
      if sec1 < sec2 then
        return true;
      else if sec1 == sec2 then
        return usec1 <= usec2;
      else
        return false;
    } else {
      const dt1 = new dateTime(new date(1900, 1, 1), t1);
      const dt2 = new dateTime(new date(1900, 1, 1), t2);
      return dt1 <= dt2;
    }
  }

  @chpldoc.nodoc
  operator time.>(t1: time(?), t2: time(?)): bool {
    if DateTimeStaticTZAwareness && (t1.tzAware != t2.tzAware)
      then compilerError("both 'time' values must have the same timezone awareness for comparison");

    if !DateTimeStaticTZAwareness && ((t1._timezone.borrow() == nil) != (t2._timezone.borrow() == nil)) {
      halt("both 'time' values must have the same timezone awareness for comparison");
    } else if t1._timezone == t2._timezone {
      const sec1 = t1.hour*3600 + t1.minute*60 + t1.second;
      const usec1 = t1.microsecond;
      const sec2 = t2.hour*3600 + t2.minute*60 + t2.second;
      const usec2 = t2.microsecond;
      if sec1 > sec2 then
        return true;
      else if sec1 == sec2 then
        return usec1 > usec2;
      else
        return false;
    } else {
      const dt1 = new dateTime(new date(1900, 1, 1), t1);
      const dt2 = new dateTime(new date(1900, 1, 1), t2);
      return dt1 > dt2;
    }
  }

  @chpldoc.nodoc
  operator time.>=(t1: time(?), t2: time(?)): bool {
    if DateTimeStaticTZAwareness && (t1.tzAware != t2.tzAware)
      then compilerError("both 'time' values must have the same timezone awareness for comparison");

    if !DateTimeStaticTZAwareness && ((t1._timezone.borrow() == nil) != (t2._timezone.borrow() == nil)) {
      halt("both 'time' values must have the same timezone awareness for comparison");
    } else if t1._timezone == t2._timezone {
      const sec1 = t1.hour*3600 + t1.minute*60 + t1.second;
      const usec1 = t1.microsecond;
      const sec2 = t2.hour*3600 + t2.minute*60 + t2.second;
      const usec2 = t2.microsecond;
      if sec1 > sec2 then
        return true;
      else if sec1 == sec2 then
        return usec1 >= usec2;
      else
        return false;
    } else {
      const dt1 = new dateTime(new date(1900, 1, 1), t1);
      const dt2 = new dateTime(new date(1900, 1, 1), t2);
      return dt1 >= dt2;
    }
  }

  @deprecated(notes="'datetime' is deprecated, please use :record:`dateTime` instead")
  type datetime = dateTime;

  /* A record representing a combined `date` and `time` */
  record dateTime {
    /*
      Whether this ``dateTime`` has an associated :class:`Timezone`.

      Currently only applicable when :param:`DateTimeStaticTZAwareness` is ``true``.
    */
    param tzAware: bool = false;

    var chpl_date: date;
    var chpl_time: time(tzAware);

    /* The minimum representable `date` and `time` */
    proc type min : dateTime(false) {
      return new dateTime(date.min, time.min);
    }

    /* The maximum representable `date` and `time` */
    proc type max : dateTime(false) {
      return new dateTime(date.max, time.max);
    }

    /* The minimum non-zero difference between two dateTimes */
    proc type resolution : timeDelta {
      return new timeDelta(microseconds=1);
    }

    /* The year represented by this `dateTime` value */
    proc year : int {
      return chpl_date.year;
    }

    /* The month represented by this `dateTime` value */
    proc month : int {
      return chpl_date.month;
    }

    /* The day represented by this `dateTime` value */
    proc day : int {
      return chpl_date.day;
    }

    /* The hour represented by this `dateTime` value */
    proc hour : int {
      return chpl_time.hour;
    }

    /* The minute represented by this `dateTime` value */
    proc minute : int {
      return chpl_time.minute;
    }

    /* The second represented by this `dateTime` value */
    proc second : int {
      return chpl_time.second;
    }

    /* The microsecond represented by this `dateTime` value */
    proc microsecond : int {
      return chpl_time.microsecond;
    }

    /* The timezone represented by this `dateTime` value */
    @deprecated(notes="The ``timezone`` query that returns a nilable value is deprecated; please recompile with '-sDateTimeStaticTZAwareness=true' to make timezone awareness a feature of the ``datetime`` type")
    proc timezone: shared Timezone?
      where DateTimeStaticTZAwareness == false
        do return chpl_time._timezone;

    proc timezone: shared Timezone
      where DateTimeStaticTZAwareness == true && tzAware == true
        do return try!(chpl_time.chpl_tz: shared Timezone);

    @chpldoc.nodoc
    proc timezone: shared Timezone
      where DateTimeStaticTZAwareness == true && tzAware == false
        do compilerError("The 'timezone' query is only availible on a 'dateTime' with 'tzAware=true'");

    // for internal use during deprecation. Replace uses with `timezone` after nil-returning version is removed
    @chpldoc.nodoc
    proc _timezone: shared Timezone? where DateTimeStaticTZAwareness == false do return chpl_time._timezone;
    @chpldoc.nodoc
    proc _timezone: shared Timezone where DateTimeStaticTZAwareness == true do return try!(chpl_time.chpl_tz: shared Timezone);

    @chpldoc.nodoc
    @deprecated(notes="'tzinfo' is deprecated, please use 'timezone' instead")
    proc tzinfo {
      return timezone;
    }
  }

  /* initializers/factories for dateTime values */

  @chpldoc.nodoc
  @unstable("initializing a 'dateTime' without arguments is unstable; it may become illegal or be replaced with a specific uninitialized 'dateTime' sentinel value in the future")
  proc dateTime.init(param tzAware: bool = false) {
    this.tzAware = tzAware;
  }

  /* Initialize a new `dateTime` value from the given `year`, `month`, `day`,
     `hour`, `minute`, `second`, `microsecond` and timezone.  The `year`,
     `month`, and `day` arguments are required, the rest are optional.
   */
  @unstable("tz is unstable; its type may change in the future")
  proc dateTime.init(year:int, month:int, day:int,
                     hour:int=0, minute:int=0, second:int=0, microsecond:int=0,
                     in tz: shared Timezone)
    where DateTimeStaticTZAwareness == true
  {
    this.init(year, month, day, hour, minute, second, microsecond, tz, internal=true);
  }

  // Temporary helper for use during DateTimeStaticTZAwareness deprecation
  //  This should be removed, and its implementation moved into the above
  //  initializer when the deprecation is complete. Internal calls to 'new datetime'
  //  should remove their: `internal=true` argument.
  @chpldoc.nodoc
  proc dateTime.init(year:int, month:int, day:int,
                     hour:int=0, minute:int=0, second:int=0, microsecond:int=0,
                     in tz: shared Timezone, internal: bool)
    where DateTimeStaticTZAwareness == true
  {
    this.tzAware = true;
    chpl_date = new date(year, month, day);
    chpl_time = new time(hour, minute, second, microsecond, tz, internal=true);
  }

  @deprecated(notes="The 'dateTime' initializer that accepts a nilable 'tz' is deprecated; please recompile with -sDateTimeStaticTZAwareness=true to use the overload that accepts a non-nilable 'tz'")
  proc dateTime.init(year:int, month:int, day:int,
                     hour:int=0, minute:int=0, second:int=0, microsecond:int=0,
                     in tz: shared Timezone?)
    where DateTimeStaticTZAwareness == false
  {
    this.init(year, month, day, hour, minute, second, microsecond, tz, internal=true);
  }

  // Temporary helper for use during DateTimeStaticTZAwareness deprecation
  //  This helper and the above initailizer should be removed once the
  //  deprecation is complete. Internal calls to 'new time' should remove
  //  their: `internal=true` argument.
  @chpldoc.nodoc
  proc dateTime.init(year:int, month:int, day:int,
                     hour:int=0, minute:int=0, second:int=0, microsecond:int=0,
                     in tz: shared Timezone?, internal: bool)
    where DateTimeStaticTZAwareness == false
  {
    chpl_date = new date(year, month, day);
    chpl_time = new time(hour, minute, second, microsecond, tz, internal=true);
  }

  /* Initialize a new `dateTime` value from the given `year`, `month`, `day`,
     `hour`, `minute`, `second`, and `microsecond`.  The `year`,
     `month`, and `day` arguments are required, the rest are optional.
   */
  proc dateTime.init(year:int, month:int, day:int,
                     hour:int=0, minute:int=0,
                     second:int=0, microsecond:int=0) {
    tzAware = false;
    chpl_date = new date(year, month, day);
    chpl_time = new time(hour, minute, second, microsecond);
  }

  /* Initialize a new `dateTime` value from the given `date` and `time` */
  proc dateTime.init(d: date, t: time(?) = new time(false)) {
    tzAware = t.tzAware;
    chpl_date = d;
    chpl_time = t;
  }

  /* Return a `dateTime` value representing the current time and date,
     in naive local time.
  */
  proc type dateTime.now() : dateTime(false) {
    const timeSinceEpoch = getTimeOfDay();
    const lt = getLocalTime(timeSinceEpoch);
    return new dateTime(year=lt.tm_year+1900, month=lt.tm_mon+1,
                        day=lt.tm_mday,       hour=lt.tm_hour,
                        minute=lt.tm_min,     second=lt.tm_sec,
                        microsecond=timeSinceEpoch(1));
  }

  /* Return a `dateTime` value representing the current time and date */
  @unstable("tz is unstable; its type may change in the future")
  proc type dateTime.now(in tz: shared Timezone) : dateTime(?) {
    // if tz.borrow() == nil {
    //   const timeSinceEpoch = getTimeOfDay();
    //   const lt = getLocalTime(timeSinceEpoch);
    //   return new dateTime(year=lt.tm_year+1900, month=lt.tm_mon+1,
    //                       day=lt.tm_mday,       hour=lt.tm_hour,
    //                       minute=lt.tm_min,     second=lt.tm_sec,
    //                       microsecond=timeSinceEpoch(1));
    // } else {
      const timeSinceEpoch = getTimeOfDay();
      const td = new timeDelta(seconds=timeSinceEpoch(0),
                               microseconds=timeSinceEpoch(1));
      const utcNow = unixEpoch + td;

      return (utcNow + tz.utcOffset(utcNow)).replace(tz=tz);
    // }
  }

  /* Return a `dateTime` value representing the current time and date in UTC */
  proc type dateTime.utcNow() : dateTime(false) {
    const timeSinceEpoch = getTimeOfDay();
    const td = new timeDelta(seconds=timeSinceEpoch(0),
                             microseconds=timeSinceEpoch(1));
    return unixEpoch + td;
  }

  @deprecated(notes="'dateTime.fromTimestamp' is deprecated, please use 'dateTime.createFromTimestamp' instead")
  proc type dateTime.fromTimestamp(timestamp: real) : dateTime {
    return dateTime.createFromTimestamp(timestamp, nil);
  }

  @deprecated(notes="'dateTime.fromTimestamp' is deprecated, please use 'dateTime.createFromTimestamp' instead")
  proc type dateTime.fromTimestamp(timestamp: real,
                                   in tz: shared Timezone?) : dateTime {
    return dateTime.createFromTimestamp(timestamp, tz);
  }

  /* The `dateTime` that is `timestamp` seconds from the epoch,
     in naive local time.
  */
  proc type dateTime.createFromTimestamp(timestamp: real) : dateTime {
    return dateTime.createFromTimestamp(timestamp, nil);
  }

  /* The `dateTime` that is `timestamp` seconds from the epoch */
  @unstable("tz is unstable; its type may change in the future")
  proc type dateTime.createFromTimestamp(timestamp: real,
                                   in tz: shared Timezone?) : dateTime {
    if tz.borrow() == nil {
      var t = (timestamp: int, ((timestamp - timestamp: int)*1000000): int);
      const lt = getLocalTime(t);
      return new dateTime(year=lt.tm_year+1900, month=lt.tm_mon+1,
                          day=lt.tm_mday,       hour=lt.tm_hour,
                          minute=lt.tm_min,     second=lt.tm_sec,
                          microsecond=t(1));
    } else {
      var dt = dateTime.createUtcFromTimestamp(timestamp);
      return (dt + tz!.utcOffset(dt)).replace(tz=tz);
    }
  }

  @deprecated(notes="'dateTime.utcFromTimestamp' is deprecated, please use 'dateTime.createUtcFromTimestamp' instead")
  proc type dateTime.utcFromTimestamp(timestamp) : dateTime {
    return dateTime.createUtcFromTimestamp(timestamp);
  }

  /* The `dateTime` that is `timestamp` seconds from the epoch in UTC */
  proc type dateTime.createUtcFromTimestamp(timestamp) : dateTime {
    return unixEpoch + new timeDelta(seconds=timestamp: int, microseconds=((timestamp-timestamp: int)*1000000): int);
  }

  @deprecated(notes="'dateTime.fromOrdinal' is deprecated, please use 'new dateTime(date.createFromOrdinal(ordinal))' instead")
  proc type dateTime.fromOrdinal(ordinal) : dateTime {
    return dateTime.createFromOrdinal(ordinal);
  }

  /* The `dateTime` that is `ordinal` days from 1-1-0001 */
  @deprecated(notes="'dateTime.createFromOrdinal' is deprecated; use 'new dateTime(date.createFromOrdinal(ordinal))' instead")
  proc type dateTime.createFromOrdinal(ordinal: int) : dateTime {
    return new dateTime(date.createFromOrdinal(ordinal));
  }

  /* Form a `dateTime` value from a given `date` and `time` */
  @deprecated(notes="`dateTime.combine` is deprecated; use `new dateTime` taking a `date` and `time` argument instead")
  proc type dateTime.combine(d: date, t: time(?a)) : dateTime(a) {
    return new dateTime(d, t);
  }

  /* Methods on dateTime values */

  @deprecated(notes="'dateTime.getdate' is deprecated. Please use :proc:`dateTime.getDate` instead")
  proc dateTime.getdate() : date {
    return chpl_date;
  }

  /* Get the `date` portion of the `dateTime` value */
  proc dateTime.getDate() : date {
    return chpl_date;
  }

  @deprecated(notes="'dateTime.gettime' is deprecated. Please use :proc:`dateTime.getTime` instead")
  proc dateTime.gettime() : time(false) {
    return getTime();
  }

  /* Get the `time` portion of the `dateTime` value, without its timezone value */
  proc dateTime.getTime() : time(false) {
    if DateTimeStaticTZAwareness {
      if tzAware
        then return new time(hour=hour, minute=minute,
                             second=second, microsecond=microsecond);
        else return chpl_time;
    } else {
      if chpl_time._timezone.borrow() == nil
        then return chpl_time;
        else return new time(hour=hour, minute=minute,
                             second=second, microsecond=microsecond);
    }
  }

  /* Get the `time` portion of the `dateTime` value including the
     `tz` field
   */
  @unstable("tz is unstable; its type may change in the future")
  proc dateTime.timetz() : time(true)
    where DateTimeStaticTZAwareness == true
      do return chpl_time;

  /* Get the `time` portion of the `dateTime` value including the
     `tz` field
   */
  @unstable("tz is unstable; its type may change in the future")
  proc dateTime.timetz() : time
    where DateTimeStaticTZAwareness == false
      do return chpl_time;

  /* Get a new `time` based on this one, optionally with the `year`, `month`,
     `day`, `hour` `minute`, `second`, and/or `microsecond` replaced.
  */
  proc dateTime.replace(year=-1, month=-1, day=-1,
                        hour=-1, minute=-1, second=-1, microsecond=-1): dateTime(false) {
    return new dateTime(
      chpl_date.replace(year, month, day),
      chpl_time.replace(hour, minute, second, microsecond)
    );
  }
  /* Get a new `time` based on this one, optionally with the `year`, `month`,
     `day`, `hour` `minute`, `second`, `microsecond` and/or `tz` replaced.
  */
  @unstable("tz is unstable; its type may change in the future")
  proc dateTime.replace(year=-1, month=-1, day=-1,
                        hour=-1, minute=-1, second=-1, microsecond=-1,
                        in tz: shared Timezone) : dateTime(true)
    where DateTimeStaticTZAwareness == true
  {
    return new dateTime(
      chpl_date.replace(year, month, day),
      chpl_time.replace(hour, minute, second, microsecond, tz)
    );
  }

  @unstable("tz is unstable; its type may change in the future")
  proc dateTime.replace(year=-1, month=-1, day=-1,
                        hour=-1, minute=-1, second=-1, microsecond=-1,
                        in tz=this._timezone) : dateTime
    where DateTimeStaticTZAwareness == false
  {
    return new dateTime(
      chpl_date.replace(year, month, day),
      chpl_time.replace(hour, minute, second, microsecond, tz)
    );
  }

  /* Return the date and time converted into the timezone in the argument */
  @unstable("tz is unstable; its type may change in the future")
  proc dateTime.astimezone(in tz: shared Timezone): dateTime(this.tzAware) {
    if this._timezone == tz {
      return this;
    }
    const utc = (this - this.utcOffset()).replace(tz=tz);
    return tz.borrow().fromUtc(utc);
  }

  /* Return the offset from UTC */
  @unstable("'utcOffset' is unstable")
  proc dateTime.utcOffset() : timeDelta {
    // if DateTimeStaticTZAwareness && !tzAware
    //   then compilerError("'utcOffset' called on 'dateTime' without a timezone");

    if DateTimeStaticTZAwareness {
      return this._timezone.utcOffset(this);
    } else {
      if this._timezone.borrow() == nil
        then halt("'utcOffset' called on 'dateTime' without a timezone");
        else return this._timezone!.utcOffset(this);
    }
  }
  /* Return the daylight saving time offset */
  @unstable("'dst' is unstable")
  proc dateTime.dst() : timeDelta {
    if DateTimeStaticTZAwareness && !tzAware
      then compilerError("'dst' called on 'dateTime' without a timezone");

    if DateTimeStaticTZAwareness {
      return this._timezone.dst(this);
    } else {
      if this._timezone.borrow() == nil
        then halt("'dst' called on 'dateTime' without a timezone");
        else return this._timezone!.dst(this);
    }
  }

  /* Return the name of the timezone for this `dateTime` value */
  @unstable("'tzname' is unstable")
  proc dateTime.tzname() : string {
    if DateTimeStaticTZAwareness {
      if this.tzAware
        then return this._timezone.tzname(this);
        else return "";
    } else {
      if this._timezone.borrow() == nil
        then return "";
        else return this._timezone!.tzname(this);
    }
  }

  /* Return a filled record matching the C `struct tm` type for the given
     `dateTime` */
  @unstable("'dateTime.timetuple' is unstable")
  proc dateTime.timetuple() : tm {
    var timeStruct: tm;
    timeStruct.tm_sec = second: int(32);
    timeStruct.tm_min = minute: int(32);
    timeStruct.tm_hour = hour: int(32);
    timeStruct.tm_mday = day: int(32);
    timeStruct.tm_mon = month: int(32);
    timeStruct.tm_year = year: int(32);
    timeStruct.tm_wday = getDate()._old_weekday(): int(32);
    timeStruct.tm_yday = (getDate().toOrdinal() -
        (new date(year, 1, 1)).toOrdinal() + 1): int(32);

    if DateTimeStaticTZAwareness {
      if tzAware {
        timeStruct.tm_isdst = -1;
      } else if dst() == new timeDelta(0) {
        timeStruct.tm_isdst = 0;
      } else {
        timeStruct.tm_isdst = 1;
      }
    } else {
      if this._timezone.borrow() == nil {
        timeStruct.tm_isdst = -1;
      } else if dst() == new timeDelta(0) {
        timeStruct.tm_isdst = 0;
      } else {
        timeStruct.tm_isdst = 1;
      }
    }

    return timeStruct;
  }

  /* Return a filled record matching the C `struct tm` type for the given
     `dateTime` in UTC
   */
  @unstable("'dateTime.utctimetuple' is unstable")
  proc dateTime.utctimetuple() : tm {
    if DateTimeStaticTZAwareness {
      if tzAware {
        var ret = timetuple();
        ret.tm_isdst = 0;
        return ret;
      } else {
        const utc = this.replace() - utcOffset();
        var ret = utc.timetuple();
        ret.tm_isdst = 0;
        return ret;
      }
    } else {
      if this._timezone.borrow() == nil {
        var ret = timetuple();
        ret.tm_isdst = 0;
        return ret;
      } else {
        const utc = this.replace(tz=nil) - utcOffset();
        var ret = utc.timetuple();
        ret.tm_isdst = 0;
        return ret;
      }
    }
  }

  /* Return the number of days since 1-1-0001 this `dateTime` represents */
  @deprecated(notes="`dateTime.toOrdinal` is deprecated; use `dateTime.getDate().toOrdinal()` instead")
  proc dateTime.toOrdinal() : int {
    return getDate().toOrdinal();
  }

  /* Return the day of the week.
   */
  @deprecated(notes="`dateTime.weekday` is deprecated; use `dateTime.getDate().weekday()` instead")
  proc dateTime.weekday() : dayOfWeek where cIsoDayOfWeek {
    return getDate().weekday();
  }
  @deprecated(notes="The version of 'dateTime.weekday' returning a :type:`dayOfWeek` starting with `Monday = 0` is deprecated. Recompile with ``-sCIsoDayOfWeek=true`` to opt in to the new behavior of `Monday = 1`")
  proc dateTime.weekday() : dayOfWeek where !cIsoDayOfWeek {
    return getDate().weekday();
  }

  /* Return the day of the week as an `isoDayOfWeek`.
     `Monday` == 1, `Sunday` == 7
   */
  @deprecated(notes="`dateTime.isoWeekday` is deprecated; use `dateTime.getDate().weekday()` instead")
  proc dateTime.isoWeekday() : isoDayOfWeek {
    return getDate().isoWeekday();
  }

  @chpldoc.nodoc
  @deprecated(notes="`dateTime.isoweekday` is deprecated, please use `dateTime.getDate().weekday()` instead")
  proc dateTime.isoweekday() : isoDayOfWeek {
    return isoWeekday();
  }

  /* Return the ISO date as a tuple containing the ISO year, ISO week number,
     and ISO day of the week
   */
  @deprecated(notes="`dateTime.isoCalendar` is deprecated; use `dateTime.getDate().isoWeekDate()` instead")
  proc dateTime.isoCalendar() : (int, int, int) {
    return getDate().isoCalendar();
  }

  @chpldoc.nodoc
  @deprecated(notes="`dateTime.isocalendar` is deprecated, please use `dateTime.getDate().isoWeekDate()` instead")
  proc dateTime.isocalendar() : (int, int, int) {
    return getDate().isoCalendar();
  }

  /* Get a `string` representation of this `dateTime` in ISO format
     ``YYYY-MM-DDThh:mm:ss.ssssss``, followed by ``±hh:mm`` if a timezone is
     specified
  */
  operator dateTime.:(x: dateTime, type t: string) {
    proc zeroPad(nDigits: int, i: int) {
      var numStr = i: string;
      for i in 1..nDigits-numStr.size {
        numStr = "0" + numStr;
      }
      return numStr;
    }
    var micro = if x.microsecond > 0 then "." + zeroPad(6, x.microsecond) else "";
    var offset: string;
    if x._timezone.borrow() != nil {
      var utcoff = x.utcOffset();
      var sign: string;
      if utcoff < new timeDelta(0) {
        sign = '-';
        utcoff = utcoff.abs();
      } else {
        sign = '+';
      }
      var hours = utcoff.seconds / (60*60);
      var minutes = (utcoff.seconds % (60*60)) / 60;
      offset = sign +
               (if hours < 10 then "0" + hours: string else hours: string) +
               ":" +
               (if minutes < 10 then "0" + minutes: string else minutes: string);
    }

    // on our Linux64 systems, the "%Y" format doesn't zero-pad to 4
    // characters on its own, so do it manually.
    var year = zeroPad(4, try! x.strftime("%Y"):int);
    return x.strftime(year + "-%m-%d" + "T" + "%H:%M:%S" + micro + offset);
  }

  /* Return the `dateTime` as a `string` in ISO format */
  @deprecated(notes="`dateTime.isoFormat` is deprecated; use cast to string instead")
  proc dateTime.isoFormat(sep="T") : string {
    proc zeroPad(nDigits: int, i: int) {
      var numStr = i: string;
      for i in 1..nDigits-numStr.size {
        numStr = "0" + numStr;
      }
      return numStr;
    }
    var micro = if microsecond > 0 then "." + zeroPad(6, microsecond) else "";
    var offset: string;
    if timezone.borrow() != nil {
      var utcoff = utcOffset();
      var sign: string;
      if utcoff < new timeDelta(0) {
        sign = '-';
        utcoff = utcoff.abs();
      } else {
        sign = '+';
      }
      var hours = utcoff.seconds / (60*60);
      var minutes = (utcoff.seconds % (60*60)) / 60;
      offset = sign +
               (if hours < 10 then "0" + hours: string else hours: string) +
               ":" +
               (if minutes < 10 then "0" + minutes: string else minutes: string);
    }

    // on our Linux64 systems, the "%Y" format doesn't zero-pad to 4
    // characters on its own, so do it manually.
    var year = zeroPad(4, try! strftime("%Y"):int);
    return strftime(year + "-%m-%d" + sep + "%H:%M:%S" + micro + offset);
  }

  /* Create a `dateTime` as described by the `date_string` and
     `format` string.  Note that this routine currently only supports
     the format strings of C's strptime().
  */
  @unstable("'dateTime.strptime' is unstable")
  proc type dateTime.strptime(date_string: string, format: string): dateTime(false) {
    extern proc strptime(buf: c_ptrConst(c_char), format: c_ptrConst(c_char), ref ts: tm);
    var timeStruct: tm;
    strptime(date_string.c_str(), format.c_str(), timeStruct);
    return new dateTime(timeStruct.tm_year + 1900,
                        timeStruct.tm_mon + 1,
                        timeStruct.tm_mday,
                        timeStruct.tm_hour,
                        timeStruct.tm_min,
                        timeStruct.tm_sec);
  }

  /* Create a `string` from a `dateTime` matching the `format` string */
  @unstable("'dateTime.strftime' is unstable")
  proc dateTime.strftime(fmt: string) : string {
    extern proc strftime(s: c_ptr(void), size: c_size_t, format: c_ptrConst(c_char), ref timeStruct: tm);
    const bufLen: c_size_t = 100;
    var buf: [1..bufLen] c_char;
    var timeStruct: tm;

    timeStruct.tm_hour = hour: int(32);
    timeStruct.tm_min = minute: int(32);
    timeStruct.tm_sec = second: int(32);

    if DateTimeStaticTZAwareness {
      if tzAware {
        timeStruct.tm_isdst = this._timezone.dst(this).seconds: int(32);
        timeStruct.tm_gmtoff = this._timezone.utcOffset(this).seconds: c_long;
        timeStruct.tm_zone = nil;
      } else {
        timeStruct.tm_isdst = -1: int(32);
        timeStruct.tm_gmtoff = 0;
        timeStruct.tm_zone = nil;
      }
    } else {
      if this._timezone.borrow() != nil {
        timeStruct.tm_isdst = this._timezone!.dst(this).seconds: int(32);
        timeStruct.tm_gmtoff = this._timezone!.utcOffset(this).seconds: c_long;
        timeStruct.tm_zone = nil;
      } else {
        timeStruct.tm_isdst = -1: int(32);
        timeStruct.tm_gmtoff = 0;
        timeStruct.tm_zone = nil;
      }
    }

    timeStruct.tm_year = (year-1900): int(32); // 1900 based
    timeStruct.tm_mon = (month-1): int(32);    // 0 based
    timeStruct.tm_mday = day: int(32);
    timeStruct.tm_wday = (this.getDate()._old_weekday(): int(32) + 1) % 7; // shift Sunday to 0
    timeStruct.tm_yday = (this.replace(tz=nil) - new dateTime(year, 1, 1)).days: int(32);

    // Iterate over format specifiers in strftime(), replacing %f with microseconds
    iter strftok(const ref s: string)
    {
      var per = "";
      for c in s {
        if per == "" {
          if c == '%' {
             per = "%";
          } else {
             yield c;
          }
        } else {
          per += c;

          // Modifiers - (no padding) 0 (0-padding) _ (space padding) E and O (POSIX extensions)
          if per != '%-' && per != '%0' && per != '%_' && per != '%E' && per != '%O' {
            if c == "f" {
              const fmt = if per == "%-f" then "%i" else if per == "%_f" then "%6i" else "%06i";
              try! {
                yield fmt.format(chpl_time.chpl_microsecond);
              }
            } else {
              yield per;
            }
            per = "";
          }
        }
      }
      if per != "" {
        yield per;
      }
    }

    strftime(c_ptrTo(buf), bufLen, "".join(strftok(fmt)).c_str(), timeStruct);

    var str: string;
    try! {
      str = string.createCopyingBuffer(c_ptrTo(buf));
    }

    return str;
  }

  /* Return a `string` from a `dateTime` in the form:
     Wed Dec  4 20:30:40 2002
  */
  @unstable("'dateTime.ctime' is unstable")
  proc dateTime.ctime() : string {
    return this.strftime("%a %b %e %T %Y");
  }

  /* Writes this `dateTime` formatted as ``YYYY-MM-DDThh:mm:ss.ssssss``,
     followed by ``±hh:mm`` if a timezone is specified
  */
  proc dateTime.writeThis(f) throws {
    f.write("%?".format(this));
  }

  /* Reads this `dateTime` with the same format used by
     :proc:`dateTime.writeThis`
   */
  proc ref dateTime.readThis(f) throws {
    import JSON.jsonDeserializer;

    const binary = f._binary(),
          arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY),
          isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary ||
            isSubtype(f.deserializerType, jsonDeserializer);

    if isjson then
      f._readLiteral('"');

    chpl_date._readCore(f);
    f._readLiteral("T");
    chpl_time._readCore(f);

    if isjson then
      f._readLiteral('"');
  }

  //
  // TODO: need to get this to work with the Json formatter
  //
  @chpldoc.nodoc
  proc dateTime.init(reader: fileReader, ref deserializer) throws {
    this.init();
    readThis(reader);
  }


  // TODO: Add a dateTime.timestamp() method

  /* Operators on dateTime values */

  @chpldoc.nodoc
  operator dateTime.+(td: timeDelta, dt: dateTime(?a)) : dateTime(a) {
    var newmicro = dt.microsecond + td.microseconds;
    var newsec = dt.second + td.seconds;
    var newmin = dt.minute;
    var newhour = dt.hour;

    // adjust to fit
    newsec += newmicro / 1000000;
    newmicro %= 1000000;

    newmin += newsec / 60;
    newsec %= 60;

    newhour += newmin / 60;
    newmin %= 60;

    var adddays = td.days + newhour / 24;
    newhour %= 24;

    return new dateTime(date.createFromOrdinal(dt.getDate().toOrdinal()+adddays),
                            new time(hour=newhour, minute=newmin,
                                     second=newsec, microsecond=newmicro,
                                     tz=dt._timezone, internal=true));

  }

  @chpldoc.nodoc
  operator dateTime.+(dt: dateTime(?a), td: timeDelta) : dateTime(a) {
    return td + dt;
  }

  @chpldoc.nodoc
  operator dateTime.-(dt: dateTime(?a), td: timeDelta) : dateTime(a) {
    var deltasec  = td.seconds % 60;
    var deltamin  = (td.seconds / 60) % 60;
    var deltahour = td.seconds / (60*60);

    var newmicro = dt.microsecond - td.microseconds;
    var newsec = dt.second - deltasec;
    var newmin = dt.minute - deltamin;
    var newhour = dt.hour - deltahour;

    var subDays = td.days;

    if newmicro < 0 {
      newsec -= 1;
      newmicro += 1000000;
    }
    if newsec < 0 {
      newmin -= 1;
      newsec += 60;
    }
    if newmin < 0 {
      newhour -= 1;
      newmin += 60;
    }
    if newhour < 0 {
      subDays += 1;
      newhour += 24;
    }
    return new dateTime(date.createFromOrdinal(dt.getDate().toOrdinal()-subDays),
                            new time(hour=newhour, minute=newmin,
                                     second=newsec, microsecond=newmicro,
                                     tz=dt._timezone, internal=true));
  }

  @chpldoc.nodoc
  operator dateTime.-(dt1: dateTime(?), dt2: dateTime(?)): timeDelta {
    if DateTimeStaticTZAwareness && (dt1.tzAware != dt2.tzAware)
      then compilerError("both 'dateTime' values must have the same timezone awareness for subtraction");

    if !DateTimeStaticTZAwareness && ((dt1._timezone.borrow() == nil) != (dt2._timezone.borrow() == nil))
      then halt("both 'dateTime' values must have the same timezone awareness for subtraction");

    if dt1._timezone == dt2._timezone {
      const newmicro = dt1.microsecond - dt2.microsecond,
            newsec = dt1.second - dt2.second,
            newmin = dt1.minute - dt2.minute,
            newhour = dt1.hour - dt2.hour,
            newday = dt1.getDate().toOrdinal() - dt2.getDate().toOrdinal();
      return new timeDelta(days=newday, hours=newhour, minutes=newmin,
                           seconds=newsec, microseconds=newmicro);
    } else {
      return dt1.replace(tz=nil) -
                                dt2.replace(tz=nil) +
                                dt2.utcOffset() - dt1.utcOffset();
    }
  }

  @chpldoc.nodoc
  operator dateTime.==(dt1: dateTime(?), dt2: dateTime(?)): bool {
    if DateTimeStaticTZAwareness && (dt1.tzAware != dt2.tzAware)
      then compilerError("both 'dateTime' values must have the same timezone awareness for comparison");

    if !DateTimeStaticTZAwareness && ((dt1._timezone.borrow() == nil) != (dt2._timezone.borrow() == nil)) {
      halt("both 'dateTime' values must have the same timezone awareness for comparison");
    } else if dt1._timezone == dt2._timezone {
      // just ignore timezone
      var d1: date = dt1.replace(tz=nil).getDate(),
          d2: date = dt2.replace(tz=nil).getDate();
      var t1: time = dt1.replace(tz=nil).getTime(),
          t2: time = dt2.replace(tz=nil).getTime();

      return d1.year == d2.year && d1.month == d2.month && d1.day == d2.day &&
                        t1.hour == t2.hour && t1.minute == t2.minute &&
                        t1.second == t2.second &&
                        t1.microsecond == t2.microsecond;
    } else {
      return (dt1.replace(tz=nil) - dt1.utcOffset()) ==
             (dt2.replace(tz=nil) - dt2.utcOffset());
    }
  }

  @chpldoc.nodoc
  operator dateTime.!=(dt1: dateTime(?), dt2: dateTime(?)) : bool {
    return !(dt1 == dt2);
  }

  @chpldoc.nodoc
  operator dateTime.<(dt1: dateTime(?), dt2: dateTime(?)): bool {
    if DateTimeStaticTZAwareness && (dt1.tzAware != dt2.tzAware)
      then compilerError("both 'dateTime' values must have the same timezone awareness for comparison");

    if !DateTimeStaticTZAwareness && ((dt1._timezone.borrow() == nil) != (dt2._timezone.borrow() == nil)) {
      halt("both 'dateTime' values must have the same timezone awareness for comparison");
    } else if dt1._timezone == dt2._timezone {
      const date1 = dt1.getDate(),
            date2 = dt2.getDate();
      if date1 < date2 then return true;
      else if date2 < date1 then return false;
      else return dt1.getTime() < dt2.getTime();
    } else {
      return (dt1 - dt1.utcOffset()) <
             (dt2 - dt2.utcOffset());
    }
  }

  @chpldoc.nodoc
  operator dateTime.<=(dt1: dateTime, dt2: dateTime): bool {
    if DateTimeStaticTZAwareness && (dt1.tzAware != dt2.tzAware)
      then compilerError("both 'dateTime' values must have the same timezone awareness for comparison");

    if !DateTimeStaticTZAwareness && ((dt1._timezone.borrow() == nil) != (dt2._timezone.borrow() == nil)) {
      halt("both 'dateTime' values must have the same timezone awareness for comparison");
    } else if dt1._timezone == dt2._timezone {
      const date1 = dt1.getDate(),
            date2 = dt2.getDate();
      if date1 < date2 then return true;
      else if date2 < date1 then return false;
      else return dt1.getTime() <= dt2.getTime();
    } else {
      return (dt1.replace(tz=nil) - dt1.utcOffset()) <=
             (dt2.replace(tz=nil) - dt2.utcOffset());
    }
  }

  @chpldoc.nodoc
  operator dateTime.>(dt1: dateTime, dt2: dateTime): bool {
    if DateTimeStaticTZAwareness && (dt1.tzAware != dt2.tzAware)
      then compilerError("both 'dateTime' values must have the same timezone awareness for comparison");

    if !DateTimeStaticTZAwareness && ((dt1._timezone.borrow() == nil) != (dt2._timezone.borrow() == nil)) {
      halt("both 'dateTime' values must have the same timezone awareness for comparison");
    } else if dt1._timezone == dt2._timezone {
      const date1 = dt1.getDate(),
            date2 = dt2.getDate();
      if date1 > date2 then return true;
      else if date2 > date1 then return false;
      else return dt1.getTime() > dt2.getTime();
    } else {
      return (dt1.replace(tz=nil) - dt1.utcOffset()) >
             (dt2.replace(tz=nil) - dt2.utcOffset());
    }
  }

  @chpldoc.nodoc
  operator dateTime.>=(dt1: dateTime, dt2: dateTime): bool {
    if DateTimeStaticTZAwareness && (dt1.tzAware != dt2.tzAware)
      then compilerError("both 'dateTime' values must have the same timezone awareness for comparison");

    if !DateTimeStaticTZAwareness && ((dt1._timezone.borrow() == nil) != (dt2._timezone.borrow() == nil)) {
      halt("both 'dateTime' values must have the same timezone awareness for comparison");
    } else if dt1._timezone == dt2._timezone {
      const date1 = dt1.getDate(),
            date2 = dt2.getDate();
      if date1 > date2 then return true;
      else if date2 > date1 then return false;
      else return dt1.getTime() >= dt2.getTime();
    } else {
      return (dt1.replace(tz=nil) - dt1.utcOffset()) >=
             (dt2.replace(tz=nil) - dt2.utcOffset());
    }
  }


  @deprecated(notes="'timedelta' is deprecated. Please use :record:`timeDelta` instead")
  type timedelta = timeDelta;
  /* A record representing an amount of time.  A `timeDelta` has fields
     representing days, seconds, and microseconds.  These fields are always
     kept within the following ranges:

     0 <= `microseconds` < 1000000

     0 <= `seconds` < 60*60*24

     -999999999 <= `days` <= 999999999

     It is an overflow error if `days` is outside the given range.
   */
  record timeDelta {
    var chpl_days: int;

    var chpl_seconds: int;

    var chpl_microseconds: int;

    /* The number of days this `timeDelta` represents */
    proc days : int {
      return chpl_days;
    }

    /* The number of seconds this `timeDelta` represents */
    proc seconds : int {
      return chpl_seconds;
    }

    /* The number of microseconds this `timeDelta` represents */
    proc microseconds : int {
      return chpl_microseconds;
    }

    /* Return the minimum representable `timeDelta` object. */
    proc type min : timeDelta {
      return new timeDelta(days=-999999999);
    }

    /* Return the maximum representable `timeDelta` object. */
    proc type max : timeDelta {
      return new timeDelta(days=999999999, hours=23, minutes=59,
                           seconds=59, microseconds=999999);
    }

    /* Return the smallest positive value representable by a `timeDelta`
       object.
     */
    proc type resolution : timeDelta {
      return new timeDelta(microseconds=1);
    }
  }

  /* initializers/factories for timeDelta values */

  /* Initialize a `timeDelta` object.  All arguments are optional and
     default to 0. Since only `days`, `seconds` and `microseconds` are
     stored, the other arguments are converted to days, seconds
     and microseconds. */
  proc timeDelta.init(days:int=0, seconds:int=0, microseconds:int=0,
                      milliseconds:int=0, minutes:int=0, hours:int=0, weeks:int=0) {
    param usps = 1000000,  // microseconds per second
          uspms = 1000,    // microseconds per millisecond
          spd = 24*60*60; // seconds per day

    //       us/sec   sec/min     min/hour
    //       |        |           |
    var us = usps * ((60 * (hours*60 + minutes)) + seconds) +
             uspms * milliseconds + microseconds;
    var sec = us / usps;
    us = us % usps;

    if us < 0 {
      sec -= 1;
      us = usps + us;
    }
    var day = weeks*7 + days + sec / spd;
    sec = sec % (spd);

    if sec < 0 {
      day -= 1;
      sec = spd + sec;
    }

    this.chpl_days = day;
    this.chpl_seconds = sec;
    this.chpl_microseconds = us;

    if this.days < -999999999 then
      HaltWrappers.initHalt("Overflow: days < -999999999");

    if this.days > 999999999 then
      HaltWrappers.initHalt("Overflow: days > 999999999");
  }

  /* Create a `timeDelta` from a given number of seconds */
  proc timeDelta.init(timestamp: real) {
    this.init(seconds = timestamp: int, microseconds=((timestamp - timestamp: int)*1000000): int);
  }


  /* Methods on timeDelta values */

  /* Return the total number of seconds represented by this object */
  proc timeDelta.totalSeconds(): real {
    return days*(24*60*60) + seconds + microseconds / 1000000.0;
  }


  /* Operators on timeDelta values */

  @chpldoc.nodoc
  operator timeDelta.*(i: int, t: timeDelta) : timeDelta {
    return new timeDelta(days=i*t.days, seconds=i*t.seconds, microseconds=i*t.microseconds);
  }

  @chpldoc.nodoc
  operator timeDelta.*(t: timeDelta, i: int) : timeDelta {
    return new timeDelta(days=i*t.days, seconds=i*t.seconds, microseconds=i*t.microseconds);
  }

  @chpldoc.nodoc
  operator timeDelta./(t: timeDelta, i: int) : timeDelta {
    var day = t.days / i;
    var second = t.seconds + (t.days % i)*24*60*60;
    var microsecond = t.microseconds + (second % i)*1000000;
    var us_remainder = microsecond % i;
    second /= i;
    microsecond /= i;

    if us_remainder*2 >= i then
      microsecond += 1; // round up

    return new timeDelta(days=day, seconds=second, microseconds=microsecond);
  }

  @chpldoc.nodoc
  operator timeDelta.+(t: timeDelta) : timeDelta {
    return t;
  }

  @chpldoc.nodoc
  operator timeDelta.-(t: timeDelta) : timeDelta {
    return new timeDelta(days=-t.days, seconds=-t.seconds, microseconds=-t.microseconds);
  }

  @chpldoc.nodoc
  operator timeDelta.+(lhs: timeDelta, rhs: timeDelta) : timeDelta {
    return new timeDelta(days=lhs.days+rhs.days,
                         seconds=lhs.seconds+rhs.seconds,
                         microseconds=lhs.microseconds+rhs.microseconds);
  }

  @chpldoc.nodoc
  operator timeDelta.-(lhs: timeDelta, rhs: timeDelta) : timeDelta {
    return new timeDelta(days=lhs.days-rhs.days,
                         seconds=lhs.seconds-rhs.seconds,
                         microseconds=lhs.microseconds-rhs.microseconds);
  }

  @chpldoc.nodoc
  operator timeDelta.>(lhs: timeDelta, rhs: timeDelta) : timeDelta {
    const ls = (lhs.days*(24*60*60) + lhs.seconds);
    const rs = (rhs.days*(24*60*60) + rhs.seconds);
    if ls > rs then return true;
    if rs > ls then return false;
    return lhs.microseconds > rhs.microseconds;
  }

  @chpldoc.nodoc
  operator timeDelta.>=(lhs: timeDelta, rhs: timeDelta) : bool {
    return lhs > rhs || lhs == rhs;
  }

  @chpldoc.nodoc
  operator timeDelta.<(lhs: timeDelta, rhs: timeDelta) : bool {
    const ls = (lhs.days*(24*60*60) + lhs.seconds);
    const rs = (rhs.days*(24*60*60) + rhs.seconds);
    if ls < rs then return true;
    if rs < ls then return false;
    return lhs.microseconds < rhs.microseconds;
  }

  @chpldoc.nodoc
  operator timeDelta.<=(lhs: timeDelta, rhs: timeDelta) : bool {
    return lhs < rhs || lhs == rhs;
  }

  /* Return the absolute value of this `timeDelta`. If is negative, then returns
     its negation, else returns it as-is.
   */
  proc timeDelta.abs() : timeDelta {
    if this.days < 0 then
      return -this;
    else
      return this;
  }

  /* Return the absolute value of `t`.  If `t` is negative, then returns `-t`,
     else returns `t`.
   */
  @deprecated(notes="`abs` as a free function is deprecated; use :proc:`timeDelta.abs` instead")
  proc abs(t: timeDelta) : timeDelta {
    return t.abs();
  }

  @chpldoc.nodoc
  operator :(t: timeDelta, type s:string) : string {
    var str: string;
    if t.days != 0 {
      str = t.days: string + " day";
      if t.days != 1 && t.days != -1 then
        str += "s";
      str += ", ";
    }
    const seconds = t.seconds % 60;
    const minutes = (t.seconds / 60) % 60;
    const hours = t.seconds / (60*60);
    const microseconds = t.microseconds;

    str += hours: string + ":";
    if minutes < 10 then
      str += "0";
    str += minutes + ":";
    if seconds < 10 then
      str += "0";
    str += seconds;
    if microseconds != 0 {
      str += ".";
      const usLog10 = log10(microseconds): int;
      for i in 1..(5-usLog10) {
        str += "0";
      }

      str += microseconds: string;
    }
    return str;
  }

  @chpldoc.nodoc
  @deprecated(notes="'TZInfo' is deprecated, please use 'Timezone' instead")
  class TZInfo: Timezone { }

  /* Abstract base class for time zones. This class should not be used
     directly, but concrete implementations of time zones should be
     derived from it. */
  @unstable("Timezone functionality is unstable and may change in the future")
  class Timezone {
    /* The offset from UTC this class represents */
    proc utcOffset(dt: dateTime): timeDelta {
      HaltWrappers.pureVirtualMethodHalt();
    }

    /* The `timeDelta` for daylight saving time */
    proc dst(dt: dateTime): timeDelta {
      HaltWrappers.pureVirtualMethodHalt();
    }

    /* The name of this time zone */
    @unstable("'tzname' is unstable")
    proc tzname(dt: dateTime): string {
      HaltWrappers.pureVirtualMethodHalt();
    }

    /* Convert a `time` in UTC to this time zone */
    proc fromUtc(dt: dateTime(?a)): dateTime(a) {
      HaltWrappers.pureVirtualMethodHalt();
    }

    // proc fromUtc(dt: dateTime): dateTime(false)
    //   where DateTimeStaticTZAwareness == false
    // {
    //   HaltWrappers.pureVirtualMethodHalt();
    // }

  }

  // TODO: Add a timezone class implementation


/*
   :arg  unit: The units for the returned value
   :type unit: :type:`TimeUnits`

   :returns: The elapsed time since midnight, local time, in the units specified
   :rtype:   `real(64)`
 */
@deprecated(notes="'getCurrentTime()' is deprecated please use 'timeSinceEpoch().totalSeconds()' instead")
proc getCurrentTime(unit: TimeUnits = TimeUnits.seconds) : real(64) do
  return _convert_microseconds(unit, chpl_now_time());

/*
   :returns:  (year, month, day) as a tuple of 3 ints

   The month is in the range 1 to 12.
   The day   is in the range 1 to 31
*/
@deprecated(notes="'getCurrentDate' is deprecated; access the individual fields of 'date.utcToday()' as needed instead, or use 'date.today()' for local wall time")
proc getCurrentDate() : (int, int, int) {
  var today = date.utcToday();
  return (today.year, today.month, today.day);
}

/*
   :returns: The current day of the week, calculated from UTC time.
   :rtype:   :type:`day`
 */
@deprecated("'getCurrentDayOfWeek' is deprecated; please use 'date.utcToday().weekday()' instead, or 'date.today().weekday()' for local wall time")
proc getCurrentDayOfWeek() : day {
  var now = chpl_now_timevalue();

  var seconds, minutes, hours, mday, month, year, wday, yday, isdst:int(32);

  chpl_timevalue_parts(now, seconds, minutes, hours, mday, month, year, wday, yday, isdst);

  return try! wday : day;
}

/*
   Delay a task for a duration in the units specified. This function
   will return without sleeping and emit a warning if the duration is
   negative.

   :arg  t: The duration for the time to sleep
   :type t: `real`

   :arg  unit: The units for the duration
   :type unit: :type:`TimeUnits`
*/
@deprecated(notes="'sleep' with a 'TimeUnits' argument is deprecated. Please use 'sleep' with a time in seconds")
inline proc sleep(t: real, unit: TimeUnits) : void {
  use CTypes;
  extern proc chpl_task_sleep(s:c_double) : void;

  if t < 0 {
    warning("sleep() called with negative time parameter: '", t, "'");
    return;
  }
  chpl_task_sleep(_convert_to_seconds(unit, t:real):c_double);
}

/*
   Delay a task for a duration specified in seconds. This function
   will return without sleeping and emit a warning if the duration is
   negative.

   :arg  t: The duration for the time to sleep
   :type t: `real`
*/
inline proc sleep(t: real) : void {
  use CTypes;
  extern proc chpl_task_sleep(s:c_double) : void;

  if t < 0 {
    warning("sleep() called with negative time parameter: '", t, "'");
    return;
  }
  chpl_task_sleep(t:c_double);
}

/*
   Implements basic stopwatch behavior with a potential resolution of
   microseconds if supported by the runtime platform.

   The :record:`!stopwatch` can be started, stopped, and cleared.
   A :record:`!stopwatch` is either running or stopped.
*/
record stopwatch {
  @chpldoc.nodoc
  var time:        _timevalue = chpl_null_timevalue();

  @chpldoc.nodoc
  var accumulated: real       = 0.0;

  @chpldoc.nodoc
  var running:     bool       = false;

  /*
     Clears the elapsed time. If the timer is running then it is restarted
     otherwise it remains in the stopped state.
  */
  proc ref clear() : void {
    accumulated = 0.0;

    if running {
      time = chpl_now_timevalue();
    }
  }

  /* Starts the timer. A warning is emitted if the timer is already running. */
  proc ref start() : void {
    if !running {
      running = true;
      time    = chpl_now_timevalue();
    } else {
      warning("start called on a timer that has not been stopped");
    }
  }

  /* Stops the timer. A warning is emitted if the timer is not running. */
  proc ref stop() : void {
    if running {
      var time2: _timevalue = chpl_now_timevalue();

      accumulated += _diff_time(time2, time);
      running      = false;
    } else {
      warning("stop called on a timer that has not been started");
    }
  }

  /* Clear the elapsed time and ensure the stopwatch is stopped */
  proc ref reset() : void {
    if running {
      stop();
    }
    clear();
  }

  /* Clear the elapsed time and ensure the stopwatch is running */
  proc ref restart() : void {
    clear();
    if !running {
      start();
    }
  }

  /*
     Returns the cumulative elapsed time, in the units specified, between
     all pairs of calls to :proc:`start` and :proc:`stop`
     since the timer was created or the last call to :proc:`clear`.
     If the timer is running, the elapsed time since the last call to
     :proc:`start` is added to the return value.

     :arg  unit: The units for the returned value
     :type unit: :type:`TimeUnits`

     :returns: The elapsed time in the units specified
     :rtype:   `real(64)`
  */
  @deprecated(notes="'stopwatch.elapsed' with a 'TimeUnits' argument is deprecated. Please call 'stopwatch.elapsed' without an argument and assume it returns a time in seconds.")
  proc elapsed(unit: TimeUnits) : real {
    if running {
      var time2: _timevalue = chpl_now_timevalue();

      return _convert_microseconds(unit, accumulated + _diff_time(time2, time));
    } else {
      return _convert_microseconds(unit, accumulated);
    }
  }

  /*
     Returns the cumulative elapsed time, in seconds, between
     all pairs of calls to :proc:`start` and :proc:`stop`
     since the timer was created or the last call to :proc:`clear`.
     If the timer is running, the elapsed time since the last call to
     :proc:`start` is added to the return value.

     :returns: The elapsed time in seconds
     :rtype:   `real(64)`
  */
  proc elapsed() : real {
    if running {
      var time2: _timevalue = chpl_now_timevalue();

      return (accumulated + _diff_time(time2, time)) / 1.0e+6;
    } else {
      return accumulated / 1.0e+6;
    }
  }
}

@deprecated(notes="'Timer' is deprecated, please use 'stopwatch' instead")
record Timer {
  @chpldoc.nodoc
  var time:        _timevalue = chpl_null_timevalue();

  @chpldoc.nodoc
  var accumulated: real       = 0.0;

  @chpldoc.nodoc
  var running:     bool       = false;

  /*
     Clears the elapsed time. If the timer is running then it is restarted
     otherwise it remains in the stopped state.
  */
  proc clear() : void {
    accumulated = 0.0;

    if running {
      time = chpl_now_timevalue();
    }
  }

  /* Starts the timer. A warning is emitted if the timer is already running. */
  proc ref start() : void {
    if !running {
      running = true;
      time    = chpl_now_timevalue();
    } else {
      warning("start called on a timer that has not been stopped");
    }
  }

  /* Stops the timer. A warning is emitted if the timer is not running. */
  proc ref stop() : void {
    if running {
      var time2: _timevalue = chpl_now_timevalue();

      accumulated += _diff_time(time2, time);
      running      = false;
    } else {
      warning("stop called on a timer that has not been started");
    }
  }

  /*
     Returns the cumulative elapsed time, in the units specified, between
     all pairs of calls to :proc:`start` and :proc:`stop`
     since the timer was created or the last call to :proc:`clear`.
     If the timer is running, the elapsed time since the last call to
     :proc:`start` is added to the return value.

     :arg  unit: The units for the returned value
     :type unit: :type:`TimeUnits`

     :returns: The elapsed time in the units specified
     :rtype:   `real(64)`
  */
  @deprecated(notes="'Timer.elapsed' with a 'TimeUnits' argument is deprecated. Please call 'stopwatch.elapsed' without an argument and assume it returns a time in seconds.")
  proc elapsed(unit: TimeUnits = TimeUnits.seconds) : real {
    if running {
      var time2: _timevalue = chpl_now_timevalue();

      return _convert_microseconds(unit, accumulated + _diff_time(time2, time));
    } else {
      return _convert_microseconds(unit, accumulated);
    }
  }
}

// returns diff of two time values in microseconds
private inline proc _diff_time(t1: _timevalue, t2: _timevalue) {
  extern proc chpl_timevalue_seconds(t:_timevalue):      int(64);
  extern proc chpl_timevalue_microseconds(t:_timevalue): int(64);

  var s1  = chpl_timevalue_seconds(t1);
  var us1 = chpl_timevalue_microseconds(t1);

  var s2  = chpl_timevalue_seconds(t2);
  var us2 = chpl_timevalue_microseconds(t2);

  return (s1 * 1.0e+6 + us1) - (s2 * 1.0e+6 + us2);
}

// converts a time specified by unit into seconds
@deprecated(notes="'_convert_to_seconds' is deprecated without replacement")
private proc _convert_to_seconds(unit: TimeUnits, us: real) {
  select unit {
    when TimeUnits.microseconds do return us *    1.0e-6;
    when TimeUnits.milliseconds do return us *    1.0e-3;
    when TimeUnits.seconds      do return us;
    when TimeUnits.minutes      do return us *   60.0;
    when TimeUnits.hours        do return us * 3600.0;
  }

  HaltWrappers.exhaustiveSelectHalt("unknown timeunits type");
}

// converts microseconds to another unit
@deprecated(notes="'_convert_microseconds' is deprecated without replacement")
private proc _convert_microseconds(unit: TimeUnits, us: real) {
  select unit {
    when TimeUnits.microseconds do return us;
    when TimeUnits.milliseconds do return us /    1.0e+3;
    when TimeUnits.seconds      do return us /    1.0e+6;
    when TimeUnits.minutes      do return us /   60.0e+6;
    when TimeUnits.hours        do return us / 3600.0e+6;
  }

  HaltWrappers.exhaustiveSelectHalt("unknown timeunits type");
}

}
