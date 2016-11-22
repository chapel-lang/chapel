# Chapel Documentation README

```
This directory contains the following documentation:

  README                 : this file
  bugs.rst               : how to report bugs or suggestions to the Chapel team
  building.rst           : information about building the Chapel compiler
  chplenv.rst            : setting up your environment to use Chapel
  compiling.rst          : how to use the Chapel compiler to compile code
  executing.rst          : execution options for Chapel programs
  launcher.rst           : using a launcher to get Chapel execution started
  multilocale.rst        : how to use Chapel with multiple locales
  prereqs.rst            : prerequisites for using Chapel
  tasks.rst              : explains how Chapel tasks are implemented using threads
  debugging.rst          : debugging capabilities for Chapel programs

  chapelLanguageSpec.pdf : the current draft of the Chapel language
                           specification

  quickReference.pdf     : a one-sheet, tri-fold overview of Chapel syntax
                           for quick reference

  platforms/             : platform-specific notes:
    cray.rst             :  Cray XC(TM), Cray XE(TM), Cray XK(TM), and
                            Cray CS(TM) series
    cygwin.rst           :  Cygwin
    ibm.rst              :  IBM
    knl.rst              :  Intel Xeon Phi (Knights Landing)
    macosx.rst           :  Mac
    marenostrum.rst      :  MareNostrum (at BSC)
    sgi.rst              :  SGI Altix

  technotes/             : technical notes about our implementation:
    allocators.rst       :  using Chapel allocators from C
    atomics.rst          :  information about using 'atomic' variables
    auxIO.rst            :  information about Auxiliary I/O with Chapel
    chpl-ipe.rst         :  information about Interactive Chapel
    reduceIntents.rst    :  'reduce' forall intents
    dsi.rst              :  requirements/guidelines on writing a domain map
    extern.rst           :  calling external C routines
    firstClassFns.rst    :  first-class functions
    libraries.rst        :  information about creating libraries in Chapel
    llvm.rst             :  information about using the LLVM back-end
    local.rst            :  the 'local' statement
    localeModels.rst     :  information about Chapel locale models
    main.rst             :  defining the 'main()' routine  TODO
    module_search.rst    :  how modules are searched for to fulfill 'use's
    sets.rst             :  set operations on associative arrays and domains
    subquery.rst         :  querying the index set owned by a locale
```


Online Documentation
--------------------

You can navigate to the root of the documentation for this release using
the URL:

[chapel.cray.com/docs/1.14](http://chapel.cray.com/docs/1.14/)


For More Information
--------------------

For additional information about Chapel, please refer to our website:

[chapel.cray.com](http://chapel.cray.com/)
