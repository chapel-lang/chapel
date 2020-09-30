.. default-domain:: chpl

.. _Chapter-Organization:

Organization
============

This specification is organized as follows:

-  Chapter \ `[Scope] <#Scope>`__, Scope, describes the scope of this
   specification.

-  Chapter \ `[Notation] <#Notation>`__, Notation, introduces the
   notation that is used throughout this specification.

-  Chapter \ `[Organization] <#Organization>`__, Organization, describes
   the contents of each of the chapters within this specification.

-  Chapter \ `[Acknowledgments] <#Acknowledgments>`__, Acknowledgements,
   offers a note of thanks to people and projects.

-  Chapter \ `[Language_Overview] <#Language_Overview>`__, Language
   Overview, describes Chapel at a high level.

-  Chapter \ `[Lexical_Structure] <#Lexical_Structure>`__, Lexical
   Structure, describes the lexical components of Chapel.

-  Chapter \ `[Types] <#Types>`__, Types, describes the types in Chapel
   and defines the primitive and enumerated types.

-  Chapter \ `[Variables] <#Variables>`__, Variables, describes
   variables and constants in Chapel.

-  Chapter \ `[Conversions] <#Conversions>`__, Conversions, describes
   the legal implicit and explicit conversions allowed between values of
   different types. Chapel does not allow for user-defined conversions.

-  Chapter \ `[Expressions] <#Expressions>`__, Expressions, describes
   the non-parallel expressions in Chapel.

-  Chapter \ `[Statements] <#Statements>`__, Statements, describes the
   non-parallel statements in Chapel.

-  Chapter \ `[Modules] <#Modules>`__, Modules, describes modules in
   Chapel., Chapel modules allow for namespace management.

-  Chapter \ `[Functions] <#Functions>`__, Functions, describes
   functions and function resolution in Chapel.

-  Chapter \ `[Tuples] <#Tuples>`__, Tuples, describes tuples in Chapel.

-  Chapter \ `[Classes] <#Classes>`__, Classes, describes reference
   classes in Chapel.

-  Chapter \ `[Records] <#Records>`__, Records, describes records or
   value classes in Chapel.

-  Chapter \ `[Unions] <#Unions>`__, Unions, describes unions in Chapel.

-  Chapter \ `[Ranges] <#Ranges>`__, Ranges, describes ranges in Chapel.

-  Chapter \ `[Domains] <#Domains>`__, Domains, describes domains in
   Chapel. Chapel domains are first-class index sets that support the
   description of iteration spaces, array sizes and shapes, and sets of
   indices.

-  Chapter \ `[Arrays] <#Arrays>`__, Arrays, describes arrays in Chapel.
   Chapel arrays are more general than in most languages including
   support for multidimensional, sparse, associative, and unstructured
   arrays.

-  Chapter \ `[Iterators] <#Iterators>`__, Iterators, describes iterator
   functions.

-  Chapter \ `[Generics] <#Generics>`__, Generics, describes Chapel’s
   support for generic functions and types.

-  Chapter \ `[Input_and_Output] <#Input_and_Output>`__, Input and
   Output, describes support for input and output in Chapel, including
   file input and output..

-  Chapter \ `[Task_Parallelism_and_Synchronization] <#Task_Parallelism_and_Synchronization>`__,
   Task Parallelism and Synchronization, describes task-parallel
   expressions and statements in Chapel as well as synchronization
   constructs, atomic variables, and the atomic statement.

-  Chapter \ `[Data_Parallelism] <#Data_Parallelism>`__, Data
   Parallelism, describes data-parallel expressions and statements in
   Chapel including reductions and scans, whole array assignment, and
   promotion.

-  Chapter \ `[Locales_Chapter] <#Locales_Chapter>`__, Locales,
   describes constructs for managing locality and executing Chapel
   programs on distributed-memory systems.

-  Chapter \ `[Domain_Maps] <#Domain_Maps>`__, Domain Maps, describes
   Chapel’s *domain map* construct for defining the layout of domains
   and arrays within a single locale and/or the distribution of domains
   and arrays across multiple locales.

-  Chapter \ `[User_Defined_Reductions_and_Scans] <#User_Defined_Reductions_and_Scans>`__,
   User-Defined Reductions and Scans, describes how Chapel programmers
   can define their own reduction and scan operators.

-  Chapter \ `[Memory_Consistency_Model] <#Memory_Consistency_Model>`__,
   Memory Consistency Model, describes Chapel’s rules for ordering the
   reads and writes performed by a program’s tasks.

-  Chapter \ `[Interoperability] <#Interoperability>`__ describes
   Chapel’s interoperability features for combining Chapel programs with
   code written in different languages.

-  Appendix \ `[Syntax] <#Syntax>`__, Collected Lexical and Syntax
   Productions, contains the syntax productions listed throughout this
   specification in both alphabetical and depth-first order.
