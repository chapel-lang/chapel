.. default-domain:: chpl

.. module:: ReplicatedVar
   :synopsis: Support for user-level replicated variables.

ReplicatedVar
=============
**Usage**

.. code-block:: chapel

   use ReplicatedVar;


Support for user-level replicated variables.

A "replicated" variable is a variable for which there is a copy on each locale.
Referencing a replicated variable
(in a stylized way, see :ref:`below <ReplicatedVar_basic-usage>`)
accesses its copy on the current locale.

Features:

* The variable's copies are not synchronized automatically among the locales.

Limitations:

* It is "user-level", i.e. the user is required to handle the variable
  in specific ways to achieve the desired result.

* Tree-shape communication (like for reductions) is not provided.

* Using a replicated variable of an array type is not straightforward.
  Workaround: declare that array itself as replicated, then access it normally,
  e.g.:

.. code-block:: chapel

   var replArray: [MyDomain dmapped Replicated()] real;

.. _ReplicatedVar_basic-usage:

-------------------------------
How to use replicated variables
-------------------------------

.. code-block:: chapel

    use ReplicatedVar;

    // declare a replicated variable of the type 'MyType'
    var myRepVar: [rcDomain] MyType;

    // access its copy on the current locale (read or write) (either option)
    ... myRepVar(1) ...  // must use 1
    ... rcLocal(myRepVar) ...

    // "replicate": assign 'valToRep' to copies on all locales
    rcReplicate(myRepVar, valToRep);

    // "collect": assign from each copy of 'myRepVar' to
    // corresponding element of an array 'collected'
    var collected: [LocaleSpace] MyType;
    rcCollect(myRepVar, collected);

    // access directly a remote copy on the locale 'remoteLoc' (read or write)
    ... rcRemote(myRepVar, remoteLoc) ...

.. _ReplicatedVar_subset-of-locales:

------------------------------------
Replicating over a subset of locales
------------------------------------

To replicate a variable over a subset of locales, say ``myLocales``,
modify the above variable declarations as follows:

.. code-block:: chapel

    var myRepVar: [rcDomainBase dmapped Replicated(myLocales,
                     "over which to replicate 'myRepVar'")] MyType;
    var collected: [myLocales.domain] MyType;


Tip: if the domain of the desired array of locales cannot be described
as a rectangular domain (which could be strided, multi-dimensional,
and/or sparse), make that array's domain associative over int.

----------------------------------------------
Declarations
----------------------------------------------

.. data:: const rcDomainBase = {rcDomainIx..rcDomainIx}

   Use this domain when replicating over a subset of locales,
   as shown :ref:`above <ReplicatedVar_subset-of-locales>`. 

.. data:: const rcDomain = rcDomainBase dmapped new dmap(rcDomainMap)

   Use this domain to declare a user-level replicated variable,
   as shown :ref:`above <ReplicatedVar_basic-usage>` . 

.. function:: proc rcReplicate(replicatedVar: [?D] ?MYTYPE, valToReplicate: MYTYPE): void

   Assign a value `valToReplicate` to copies of the replicated variable
   `replicatedVar` on all locales. 

.. function:: proc rcCollect(replicatedVar: [?D] ?MYTYPE, collected: [?CD] MYTYPE): void

   Copy the value of the replicated variable `replicatedVar` on each locale
   into the element of the array `collected` that corresponds to that locale.

.. function:: proc rcLocal(replicatedVar: [?D] ?MYTYPE) ref: MYTYPE

   
   Access the copy of `replicatedVar` on the current locale.
   
   This is equivalent to ``replicatedVar[1]``.

.. function:: proc rcRemote(replicatedVar: [?D] ?MYTYPE, remoteLoc: locale) ref: MYTYPE

   
   Access the copy of `replicatedVar` on the locale `remoteLoc`.

