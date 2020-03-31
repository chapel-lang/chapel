.. default-domain:: chpl

.. module:: DateTime
   :synopsis: Support for representing dates, times, combined dates and times and

DateTime
========
**Usage**

.. code-block:: chapel

   use DateTime;

Support for representing dates, times, combined dates and times and
timedeltas.  This module is modeled heavily off of the python module
'datetime'.

For more detailed descriptions of this functionality, please see the
python docs:

https://docs.python.org/2.7/library/datetime.html

Operators are supported for adding, subtracting, and comparing dates,
times, datetimes and timedeltas.

Operators are also supported for multiplying and dividing timedeltas.


.. data:: param MINYEAR = 1

   The minimum year allowed in `date` objects 

.. data:: param MAXYEAR = 9999

   The maximum year allowed in `date` objects 

.. data:: const unixEpoch = new datetime(1970, 1, 1)

   The Unix Epoch date and time 

.. enum:: enum DayOfWeek { Monday = 0, Tuesday = 1, Wednesday = 2, Thursday = 3, Friday = 4, Saturday = 5, Sunday = 6 }

   Days in the week, starting with `Monday` = 0 


.. enum:: enum ISODayOfWeek { Monday = 1, Tuesday = 2, Wednesday = 3, Thursday = 4, Friday = 5, Saturday = 6, Sunday = 7 }

   Days in the week, starting with `Monday` = 1 


.. function:: proc isLeapYear(year: int)

   Return true if `year` is a leap year 

.. function:: proc daysInMonth(year: int, month: int) throws

   Return the number of days in month `month` during the year `year`.
   The number for a month can change from year to year due to leap years.
   
   :throws IllegalArgumentError: Thrown if `month` is out of range.

.. record:: date

   A record representing a date 


   .. method:: proc year

      The year represented by this `date` value 

   .. method:: proc month

      The month represented by this `date` value 

   .. method:: proc day

      The day represented by this `date` value 

   .. method:: proc type min

      The minimum representable `date` 

   .. method:: proc type max

      The maximum representable `date` 

   .. method:: proc type resolution

      The minimum non-zero difference between two dates 

.. method:: proc date.init(year, month, day)

   Initialize a new `date` value from a `year`, `month`, and `day`. All
   three arguments are required and must be in valid ranges.  The
   valid ranges are:
   
   1 <= `year` <= 9999
   
   1 <= `month` <= 12
   
   1 <= `day` <= the number of days in the given month and year
   

.. method:: proc type date.today()

   A `date` object representing the current day 

.. method:: proc type date.fromtimestamp(timestamp)

   The date that is `timestamp` seconds from the epoch 

.. method:: proc type date.fromordinal(ord)

   The `date` that is `ord` days from 1-1-0001 

.. method:: proc date.replace(year = 0, month = 0, day = 0)

   Replace the `year`, `month` and/or `day` in a date to create a
   new `date` 

.. method:: proc date.timetuple()

   Return a filled record matching the C `struct tm` type for the given date 

.. method:: proc date.toordinal()

   Return the number of days since 1-1-0001 this `date` represents 

.. method:: proc date.weekday()

   Return the day of the week as a `DayOfWeek`.
   `Monday` == 0, `Sunday` == 6
   

.. method:: proc date.isoweekday()

   Return the day of the week as an `ISODayOfWeek`.
   `Monday` == 1, `Sunday` == 7 

.. method:: proc date.isocalendar()

   Return the ISO date as a tuple containing the ISO year, ISO week number,
   and ISO day of the week
   

.. method:: proc date.isoformat()

   Return the date as a `string` in ISO 8601 format: "YYYY-MM-DD" 

.. method:: proc date.ctime()

   Return a `string` representing the date 

.. method:: proc date.strftime(fmt: string)

   Return a formatted `string` matching the `format` argument and the date 

.. method:: proc date.readWriteThis(f) throws

   Read or write a date value from channel `f` 

.. record:: time

   A record representing a time 


   .. method:: proc hour

      The hour represented by this `time` value 

   .. method:: proc minute

      The minute represented by this `time` value 

   .. method:: proc second

      The second represented by this `time` value 

   .. method:: proc microsecond

      The microsecond represented by this `time` value 

   .. method:: proc tzinfo

      The timezone represented by this `time` value 

   .. method:: proc type min

      The minimum representable `time` 

   .. method:: proc type max

      The maximum representable `time` 

   .. method:: proc type resolution

      The minimum non-zero difference between two times 

.. method:: proc time.init(hour = 0, minute = 0, second = 0, microsecond = 0, in tzinfo: shared nilable TZInfo = )

   Initialize a new `time` value from the given `hour`, `minute`, `second`,
   `microsecond`, and `timezone`.  All arguments are optional
   

.. method:: proc time.replace(hour = -1, minute = -1, second = -1, microsecond = -1, in tzinfo = this.tzinfo)

   Replace the `hour`, `minute`, `second`, `microsecond` and `tzinfo` in a
   `time` to create a new `time`. All arguments are optional.
   

.. method:: proc time.isoformat()

   Return a `string` representing the `time` in ISO format 

.. method:: proc time.utcoffset()

   Return the offset from UTC 

.. method:: proc time.dst()

   Return the daylight saving time offset 

.. method:: proc time.tzname()

   Return the name of the timezone for this `time` value 

.. method:: proc time.strftime(fmt: string)

   Return a `string` matching the `format` argument for this `time` 

.. method:: proc time.readWriteThis(f) throws

   Read or write a time value from channel `f` 

.. record:: datetime

   A record representing a combined `date` and `time` 


   .. method:: proc type min

      The minimum representable `date` and `time` 

   .. method:: proc type max

      The maximum representable `date` and `time` 

   .. method:: proc type resolution

      The minimum non-zero difference between two datetimes 

   .. method:: proc year

      The year represented by this `datetime` value 

   .. method:: proc month

      The month represented by this `datetime` value 

   .. method:: proc day

      The day represented by this `datetime` value 

   .. method:: proc hour

      The hour represented by this `datetime` value 

   .. method:: proc minute

      The minute represented by this `datetime` value 

   .. method:: proc second

      The second represented by this `datetime` value 

   .. method:: proc microsecond

      The microsecond represented by this `datetime` value 

   .. method:: proc tzinfo

      The timezone represented by this `datetime` value 

.. method:: proc datetime.init(year, month, day, hour = 0, minute = 0, second = 0, microsecond = 0, in tzinfo: shared nilable TZInfo = )

   Initialize a new `datetime` value from the given `year`, `month`, `day`,
   `hour`, `minute`, `second`, `microsecond` and timezone.  The `year`,
   `month`, and `day` arguments are required, the rest are optional.
   

.. method:: proc type datetime.today()

   Return a `datetime` value representing the current time and date 

.. method:: proc type datetime.now(in tz: shared nilable TZInfo = )

   Return a `datetime` value representing the current time and date 

.. method:: proc type datetime.utcnow()

   Return a `datetime` value representing the current time and date in UTC 

.. method:: proc type datetime.fromtimestamp(timestamp: real, in tz: shared nilable TZInfo = )

   The `datetime` that is `timestamp` seconds from the epoch 

.. method:: proc type datetime.utcfromtimestamp(timestamp)

   The `datetime` that is `timestamp` seconds from the epoch in UTC 

.. method:: proc type datetime.fromordinal(ordinal)

   The `datetime` that is `ordinal` days from 1-1-0001 

.. method:: proc type datetime.combine(d: date, t: time)

   Form a `datetime` value from a given `date` and `time` 

.. method:: proc datetime.getdate()

   Get the `date` portion of the `datetime` value 

.. method:: proc datetime.gettime()

   Get the `time` portion of the `datetime` value, with `tzinfo` = nil 

.. method:: proc datetime.timetz()

   Get the `time` portion of the `datetime` value including the
   `tzinfo` field
   

.. method:: proc datetime.replace(year = -1, month = -1, day = -1, hour = -1, minute = -1, second = -1, microsecond = -1, in tzinfo = this.tzinfo)

   Replace the `year`, `month`, `day`, `hour`, `minute`, `second`,
   `microsecond`, or `tzinfo` to form a new `datetime` object. All
   arguments are optional.
   

.. method:: proc datetime.astimezone(in tz: shared TZInfo)

   Return the date and time converted into the timezone in the argument 

.. method:: proc datetime.utcoffset()

   Return the offset from UTC 

.. method:: proc datetime.dst()

   Return the daylight saving time offset 

.. method:: proc datetime.tzname()

   Return the name of the timezone for this `datetime` value 

.. method:: proc datetime.timetuple()

   Return a filled record matching the C `struct tm` type for the given
   `datetime` 

.. method:: proc datetime.utctimetuple()

   Return a filled record matching the C `struct tm` type for the given
   `datetime` in UTC
   

.. method:: proc datetime.toordinal()

   Return the number of days since 1-1-0001 this `datetime` represents 

.. method:: proc datetime.weekday()

   Return the day of the week as a `DayOfWeek`.
   `Monday` == 0, `Sunday` == 6
   

.. method:: proc datetime.isoweekday()

   Return the day of the week as an `ISODayOfWeek`.
   `Monday` == 1, `Sunday` == 7
   

.. method:: proc datetime.isocalendar()

   Return the ISO date as a tuple containing the ISO year, ISO week number,
   and ISO day of the week
   

.. method:: proc datetime.isoformat(sep = "T")

   Return the `datetime` as a `string` in ISO format 

.. method:: proc type datetime.strptime(date_string: string, format: string)

   Create a `datetime` as described by the `date_string` and `format`
   string 

.. method:: proc datetime.strftime(fmt: string)

   Create a `string` from a `datetime` matching the `format` string 

.. method:: proc datetime.ctime()

   Return a `string` from a `datetime` in the form:
   Wed Dec  4 20:30:40 2002
   

.. method:: proc datetime.readWriteThis(f) throws

   Read or write a datetime value from channel `f` 

.. record:: timedelta

   A record representing an amount of time.  A `timedelta` has fields
   representing days, seconds, and microseconds.  These fields are always
   kept within the following ranges:
   
   0 <= `microseconds` < 1000000
   
   0 <= `seconds` < 60*60*24
   
   -999999999 <= `days` <= 999999999
   
   It is an overflow error if `days` is outside the given range.
   


   .. method:: proc days

      The number of days this `timedelta` represents 

   .. method:: proc seconds

      The number of seconds this `timedelta` represents 

   .. method:: proc microseconds

      The number of microseconds this `timedelta` represents 

   .. method:: proc type min

      Return the minimum representable `timedelta` object. 

   .. method:: proc type max

      Return the maximum representable `timedelta` object. 

   .. method:: proc type resolution

      Return the smallest positive value representable by a `timedelta`
      object.
      

.. method:: proc timedelta.init(days = 0, seconds = 0, microseconds = 0, milliseconds = 0, minutes = 0, hours = 0, weeks = 0)

   Initialize a `timedelta` object.  All arguments are optional and
   default to 0. Since only `days`, `seconds` and `microseconds` are
   stored, the other arguments are converted to days, seconds
   and microseconds. 

.. method:: proc timedelta.init(timestamp: real)

   Create a `timedelta` from a given number of seconds 

.. method:: proc timedelta.total_seconds(): real

   Return the total number of seconds represented by this object 

.. function:: proc abs(t: timedelta)

   Return the absolute value of `t`.  If `t` is negative, then returns `-t`,
   else returns `t`.
   

.. class:: TZInfo

   Abstract base class for time zones. This class should not be used
   directly, but concrete implementations of time zones should be
   derived from it. 


   .. method:: proc utcoffset(dt: datetime): timedelta

      The offset from UTC this class represents 

   .. method:: proc dst(dt: datetime): timedelta

      The `timedelta` for daylight saving time 

   .. method:: proc tzname(dt: datetime): string

      The name of this time zone 

   .. method:: proc fromutc(dt: datetime): datetime

      Convert a `time` in UTC to this time zone 

