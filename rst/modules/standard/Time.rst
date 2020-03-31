.. default-domain:: chpl

.. module:: Time
   :synopsis: This module provides support for querying wall time in the local

Time
====
**Usage**

.. code-block:: chapel

   use Time;


This module provides support for querying wall time in the local
timezone and implements a record :record:`~Timer` that provides basic
stopwatch behavior.  The stopwatch has the potential for microsecond
resolution and is intended to be useful for performance testing.


.. enum:: enum TimeUnits { microseconds, milliseconds, seconds, minutes, hours }

   Specifies the units to be used when certain functions return a time 


.. enum:: enum Day { sunday = 0, monday, tuesday, wednesday, thursday, friday, saturday }

   Specifies the day of the week 


.. function:: proc getCurrentTime(unit: TimeUnits = TimeUnits.seconds): real(64)

   
   :arg  unit: The units for the returned value
   :type unit: :type:`TimeUnits`
   
   :returns: The elapsed time since midnight, local time, in the units specified
   :rtype:   `real(64)`
   

.. function:: proc getCurrentDate()

   
   :returns:  (year, month, day) as a tuple of 3 ints
   
   The month is in the range 1 to 12.
   The day   is in the range 1 to 31

.. function:: proc getCurrentDayOfWeek(): Day

   
   :returns: The current day of the week
   :rtype:   :type:`Day`
   

.. function:: proc sleep(t: real, unit: TimeUnits = TimeUnits.seconds): void

   
   Delay a task for a duration in the units specified. This function
   will return without sleeping and emit a warning if the duration is
   negative.
   
   :arg  t: The duration for the time to sleep
   :type t: `real`
   
   :arg  unit: The units for the duration
   :type unit: :type:`TimeUnits`

.. record:: Timer

   
   Implements basic stopwatch behavior with a potential resolution of
   microseconds if supported by the runtime platform.
   
   The :record:`!Timer` can be started, stopped, and cleared.
   A :record:`!Timer` is either running or stopped.


   .. method:: proc clear(): void

      
      Clears the elapsed time. If the timer is running then it is restarted
      otherwise it remains in the stopped state.
      

   .. method:: proc start(): void

      Starts the timer. A warning is emitted if the timer is already running. 

   .. method:: proc stop(): void

      Stops the timer. A warning is emitted if the timer is not running. 

   .. method:: proc elapsed(unit: TimeUnits = TimeUnits.seconds): real

      
      Returns the cumulative elapsed time, in the units specified, between
      all pairs of calls to :proc:`start` and :proc:`stop`
      since the timer was created or the last call to :proc:`clear`.
      If the timer is running, the elapsed time since the last call to
      :proc:`start` is added to the return value.
      
      :arg  unit: The units for the returned value
      :type unit: :type:`TimeUnits`
      
      :returns: The elapsed time in the units specified
      :rtype:   `real(64)`
      

