:title: Users Guide: *Locales*

*Locales*: representing architectural locality
==============================================

In Chapel, a *locale* is a subset of the target architecture that can
be used to control and reason about affinity for the sake of
performance and scalability.  Things that are co-located within a
single locale are close to each other in the system and can therefore
interact with one another relatively cheaply.  Things that are in
distinct locales can still interact with each other in the same ways,
simply at greater expense.

As an example, a task running on a given locale can refer to variables
via standard lexical scoping rules regardless of which locale the
variables are stored on.  However, a variable stored on the same
locale as the task can be accessed more cheaply than if it were stored
on some other (*remote*) locale.

For most target architectures, a locale is equivalent to a compute
node.  For example, a program that is specified to execute on 16
locales will typically be executed using 16 compute nodes.

All locales have the ability to run tasks and to store variables.
This means that a locale will contain processors and memory (or have
access to processors and/or memory that it can use by proxy).

Locales may themselves contain locales (*sub-locales*), but this
capability is not yet widely used, so it is not yet covered in this
guide.
