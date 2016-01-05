:title: Users Guide: Overview

Overview
========

This Users Guide is written with the goal of providing an intuitive
and example-oriented introduction to the Chapel programming language
without feeling the need to be overly pedantic or to worry about
defining every corner case in the language.  

Organization
------------

The Chapel Users Guide is divided into four main sections:

* `Base Language <index.html#base-language>`_: This is the core of
  Chapel and what remains when all features in support of parallelism
  and locality are removed.

* `Task Parallelism <index.html#task-parallelism>`_: These are
  Chapel's lower-level features for creating parallel tasks explicitly
  and synchronizing between them.

* `Data Parallelism <index.html#data-parallelism>`_: These are
  Chapel's higher-level features for creating parallelism more
  abstractly using a rich set of data structures.

* `Locality <index.html#locality>`_: These are Chapel's features for
  describing how data and tasks should be mapped to the target
  architecture for the purposes of performance and scalability.


Pardon Our Dust
---------------

This users guide is a work-in-progress that was kicked off in Autumn
2015.  We are taking the approach of writing it in a breadth-first
manner in order to cover features that are most unique to the language
sooner without completely neglecting the base language (which is
expected to comprise the largest number of the sections of the guide).
We welcome feedback and requests on the guide as we go.
