.. default-domain:: chpl

.. _Chapter-Organization:

Organization
============

This specification is organized as follows:

-  Chapter :ref:`Chapter-Scope` describes the scope of this
   specification.

-  Chapter :ref:`Chapter-Notation` introduces the
   notation that is used throughout this specification.

-  This Chapter :ref:`Chapter-Organization` describes
   the contents of each of the chapters within this specification.

-  Chapter :ref:`Chapter-Acknowledgments`
   offers a note of thanks to people and projects.

-  Chapter :ref:`Chapter-Language_Overview` describes Chapel at a high level.

-  Chapter :ref:`Chapter-Lexical_Structure` describes
   the lexical components of Chapel.

-  Chapter :ref:`Chapter-Types` describes the types in Chapel
   and defines the primitive and enumerated types.

-  Chapter :ref:`Chapter-Variables` describes
   variables and constants in Chapel.

-  Chapter :ref:`Chapter-Conversions` describes
   the legal implicit and explicit conversions allowed between values of
   different types.

-  Chapter :ref:`Chapter-Expressions` describes
   the non-parallel expressions in Chapel.

-  Chapter :ref:`Chapter-Statements` describes the
   non-parallel statements in Chapel.

-  Chapter :ref:`Chapter-Modules` describes modules in
   Chapel, which allow for namespace management.

-  Chapter :ref:`Chapter-Procedures` describes
   functions and function resolution in Chapel.
   It focuses on procedure functions.

-  Chapter :ref:`Chapter-Tuples` describes tuples in Chapel.

-  Chapter :ref:`Chapter-Classes` describes classes in Chapel,
   which offer reference semantics.

-  Chapter :ref:`Chapter-Records` describes records in Chapel,
   which offer value semantics.

-  Chapter :ref:`Chapter-Unions` describes unions in Chapel.

-  Chapter :ref:`Chapter-Ranges` describes ranges in Chapel.

-  Chapter :ref:`Chapter-Domains` describes domains in
   Chapel. Chapel domains are first-class index sets that support the
   description of iteration spaces, array sizes and shapes, and sets of
   indices.

-  Chapter :ref:`Chapter-Arrays` describes arrays in Chapel.
   Chapel arrays are more general than in most languages including
   support for multidimensional, sparse, associative, and unstructured
   arrays.

-  Chapter :ref:`Chapter-Iterators` describes iterator
   functions.

-  Chapter :ref:`Chapter-Generics` describes Chapel’s
   support for generic functions and types.

-  Chapter :ref:`Chapter-Input_and_Output` describes support for
   input and output in Chapel, including file input and output.

-  Chapter :ref:`Chapter-Task_Parallelism_and_Synchronization` describes
   task-parallel expressions and statements in Chapel as well as
   synchronization constructs, atomic variables, and the atomic statement.

-  Chapter :ref:`Chapter-Data_Parallelism` describes data-parallel expressions
   and statements in
   Chapel including reductions and scans, whole array assignment, and
   promotion.

-  Chapter :ref:`Chapter-Locales_Chapter` describes constructs for managing
   locality and executing Chapel
   programs on distributed-memory systems.

-  Chapter :ref:`Chapter-Domain_Maps` describes
   Chapel’s *domain map* construct for defining the layout of domains
   and arrays within a single locale and/or the distribution of domains
   and arrays across multiple locales.

-  Chapter :ref:`Chapter-User_Defined_Reductions_and_Scans` describes
   how Chapel programmers
   can define their own reduction and scan operators.

-  Chapter :ref:`Chapter-Memory_Consistency_Model` describes Chapel’s rules
   for ordering the
   reads and writes performed by a program’s tasks.

-  Chapter :ref:`Chapter-Interoperability` describes
   Chapel’s interoperability features for combining Chapel programs with
   code written in different languages.
