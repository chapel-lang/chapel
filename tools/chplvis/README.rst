--------------------------------------------------
Chplvis -- A tool to visualize Chapel program runs
--------------------------------------------------

``chplvis`` is a tool to help the Chapel programmer visualize their
Chapel program's tasks and communication between locales.  Using the
standard module :mod:`VisualDebug`, the programmer controls what part
of their program generates information for ``chplvis``.  During the run of
a program using the :mod:`VisualDebug` module, data files are
created that are used as input for ``chplvis``.

This document is a list of possible improvements. (No particular order.
Some issues could be considered current bugs.)

-  Zoom currently leaves extra white space to the right and to the bottom
   when the zoom window is shrunk.   "reset zoom" doesn't properly reset
   this issue.

-  Add a way to have the background color user selectable rather than
   just white.

-  Change zooming to work with the scroll wheel and scrolling by drag
   and drop in the graph and grid views as well as using the current
   ways.

-  Concurrency view, add a middle button click on a task to load the
   source code file and go to the line that started that task.  Doable
   only for Local tasks, not for "On Call" tasks.  (Possibly find a
   way to do it for on call tasks also.)

-  Grid view, add labeling to show direction of data flow.

-  Add a help system that at minimum just opens the chplvis documentation
   on the web.

-  Improve the profile view as much as possible:

   + Find a way to use "concurrent" time, e.g. time in a function that
     contributes to final clock time of the program, not just a total
     time in the function.

     "simple" example of why:   "coforall l in Locales do on l sleep(5);"
     vs "sleep(5);".  Both should sleep around 5 seconds, but the
     coforall function would appear to have number of Locales more clock
     time.

   + Find a way to profile just a tag, not for the full run

   + Find a way (requires runtime changes and possibly compiler
     changes) a way to profile Chapel procs instead of internal
     functions, both for total time and concurrent time.  (CPU
     time might be interesting ... but may not be as helpful to
     a chapel programmer than a sequential C programmer.)

- In Grid and Graph view, middle click on a communication link could bring
  up all communication in a list and allow clicking to file location of the
  communication.

- Add a setting so user can select "default" view when program is run,
  currently default view is "Graph view".

- Clicking on a gray (zero comm) link in Graph view should not display a
  comm box in the information view.

- Add keyword highlighting to any view that shows source code.

- Add line numbers to any view that shows source code.

- Add a way to define CHPL_HOME and compile directory on Settings page
  so data generated on one machine could be viewed on a different machine
  with different CHPL_HOME and compile directory paths.  (Currently this
  can be done by editing the definitions of CHPL_HOME and compile directory
  in the locale 0 data file.)  Save the new definitions in a file in the
  data directory? e.g. PATHS file


Wild suggestions ...

- Make the source code views allow editing -- which is more like saying,
  build a Chapel IDE that has chplvis as a tool for viewing runs.


