.. default-domain:: chpl

.. module:: VisualDebug
   :synopsis: Support for the visualization tool :ref:`chplvis`.

VisualDebug
===========
**Usage**

.. code-block:: chapel

   use VisualDebug;



Support for the visualization tool :ref:`chplvis`.

This module provides access to and enables hooks to dump out
task and communication information for post-run visualization
of the tasks and communication.


.. data:: config param DefaultVisualDebugOn = true

   
   This is the default setting for :var:`VisualDebugOn`.  It defaults
   to `true`, but can be changed at compile time.
   

.. data:: config const VisualDebugOn = DefaultVisualDebugOn

   
   If this is `true`, data collection to support :ref:`chplvis` reporting is
   turned on.
   It defaults to the value of :param:`DefaultVisualDebugOn`, but can
   be changed at execution time.
   

.. function:: proc startVdebug(rootname: string)

   
   Start logging events for VisualDebug.  Open a new set of data
   files, one for each locale, for :ref:`chplvis`.  This routine should be
   called only once for each program.  It creates a directory with the
   rootname and creates the files in that directory.  The files are
   named with the rootname and "-n" is added where n is the locale
   number.
   
   :arg rootname:  Directory name and rootname for files.

.. function:: proc tagVdebug(tagname: string)

   
   Add a tag to the data for :ref:`chplvis` to allow "view points" in the data.
   
   :arg tagname: name of the tag

.. function:: proc stopVdebug()

   
   Stop collecting VisualDebug data and close the data files.

.. function:: proc pauseVdebug()

   
   Suspend collection of VisualDebug data.

