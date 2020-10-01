.. _technical_notes:

Technical Notes
===============

Technical Notes describe in-progress language features that are not
yet stable enough to be in the language specification. Additionally, a
Techinical Note can describe implementation details that can be ignored
by most Chapel programmers.

Base Language Features
----------------------

.. toctree::
   :maxdepth: 1

   Associative Set Operations <sets>
   Avoiding Array Element Initialization with noinit <noinit>
   Error Handling Modes and Prototype Modules <errorHandling>
   First-class Functions in Chapel <firstClassFns>
   Including Sub-Modules from Separate Files <module_include>
   main() Functions <main>
   Module Search Paths <module_search>


Initializers and Generic Programming
------------------------------------

.. toctree::
   :maxdepth: 1

   Forwarding Methods Calls <forwarding>
   The ‘init=’ Method <initequals>
   Invoking Initializers with a Generic Instantiation <initTypeAlias>
   Partial Instantiations <partialInstantiations>

Parallel Language Features
--------------------------

.. toctree::
   :maxdepth: 1

   Domain Map Standard Interface <dsi>
   The ‘local’ keyword <local>
   Locale Models <localeModels>
   Querying a Local Subdomain <subquery>
   Reduce Intents <reduceIntents>
   Runtime Support for Atomics <atomics>

Interoperability
----------------

.. toctree::
   :maxdepth: 1

   C Interoperability <extern>
   Calling Chapel Code from Other Languages <libraries>
   Fortran Interoperability <fortranInterop>
   Using the Chapel Allocator from C <allocators>

Compiler Features
-----------------

.. toctree::
   :maxdepth: 1

   Checking for Nil Dereferences <nilChecking>
   Checking Overload Sets <overloadSets>
   Checking Variable Lifetimes <lifetimeChecking>
   LLVM Support <llvm>
   Variables to Detect Compilation Configuration <globalvars>

Tool Details
------------

.. toctree::
   :maxdepth: 1

   Protocol Buffers Support - Generated Chapel Code <protoGenCodeGuide>
