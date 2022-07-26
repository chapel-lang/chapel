:title: Mason Guide: *Why mason?*
        
Why would I want to use mason?
==============================

So, you want to write some Chapel code and find yourself asking if it would
make sense to use or if it would just end up adding needless overhead to your
project in the long run. Here's an answer for you: Yes! You should use mason!

Using mason brings with it a number of valuable features that are not available
to users of the old-fashioned ``chpl`` command. Some of these are:

* access to libraries contributed by the Chapel community (see :ref:`using-a-package`)
  
* portable, easy build commands (see :ref:`building-and-running`)
  
* an existing testing framework to help test your code (see :ref:`testing-with-mason`)
  
* make your code easily shareable on GitHub (see :ref:`mason-git-dependencies`)
  
* contribute your own libraries to the existing mason registry (see :ref:`submit-a-package`)
  

So, you see, a better question would be "Why not use mason?" instead of "Why use mason?"

The Three Modes of Mason
========================

Now, having answered the "why" of mason, let's get into the "how".

There are three distinct types of mason packages:

#. mason applications
#. mason libraries
#. mason lightweight projects

Each of these types of mason packages serve different purposes and, to use mason effectively,
it helps to know what each type has been designed for.


Mason Applications
~~~~~~~~~~~~~~~~~~
A mason application, which is the default type of mason package, is meant to be a self-contained,
executable, full application. What this means is that a mason application will have a main
module and expect to be built and run as a standalone application, as opposed to being only a
library that contains functions that are valuable to users, but you wouldn't want to run by
themselves.

When you create a mason application with ``mason new <package-name>`` or
``mason new <package-name> --app``, the default module created will have a ``main()`` function
already in place. This is what drives your application and where the execution will happen from.
This ``main()`` function is unique to the mason application mason applications are the only type
of mason packages that can be ran via ``mason run``.

When compiling Chapel code, there can only be one ``main()`` function (or you will need to use the
``--main-module`` flag to specify which one to run). What this means for mason applications is that
that they are not designed to be imported into other code and have functions included in the
application be run by other applications. Because of this, mason applications published to the mason
registry can be downloaded, built, and run by running the ``mason install <application-name>`` command
(which is currently a work in progress), but they cannot be added as a dependency with the
``mason add <library-name@version>`` command. If you would like to create a library with functions
to be exposed to other users, the mason library is what you are looking for.

Some examples of when you would want to use a mason application as opposed to one of the other
types are a benchmark that you would like others to be able to compile and execute in a reproducible
way, an application that simulates the weather, or just about anywhere you have a cool program that
you would like to share.


Mason Libraries
~~~~~~~~~~~~~~~
A mason library is meant to be a module that contains functions that are to be made accessible
to other users. Mason libraries do not have a ``main()`` function and cannot be run with the
``mason run`` command, but can be run through examples.

When you create a mason library with ``mason new <package-name> --lib``, the default module
created will not have a ``main()`` function and will just be an empty module. In this module
functions can be defined that other users can use.

Some examples of when you would want to use a mason library as opposed to one of the other
types include adding a linear algebra library that could be useful to other users or for just
about any set of functions that other users of Chapel may find useful.


Mason Lightweight Projects
~~~~~~~~~~~~~~~~~~~~~~~~~~
A mason lightweight project is nothing than a mason manifest file that can track dependencies
of mason packages that your project is using without being forced into the confines of a
mason project structure, like you would be with a mason library or mason application. This
means that you cannot use most of the mason commands, such as ``mason build`` or ``mason run``,
but you can use mason libraries and get the compilation flags for using those libraries
through the ``mason modules`` command.

When you create a lightweight mason project with ``mason new --light``, all that is created
is a ``Mason.toml`` file in the current directory. Dependencies can then be added to that
file as you would with any other mason project, using the ``mason add <package-name@ver>``
command or specifying a git dependency.

Once dependencies are added to the ``Mason.toml`` that you would like to use, running a
``mason modules`` command will print out the flags that should be added to your ``chpl``
compilation step to use those libraries in your project.

The main reason you would use a lightweight mason project as opposed to one of the other
types of mason project types would be in the case that you already have a project that is
built out and has its own well-defined build process that uses something like a ``Makefile``
to handle building, so you just want to use some mason packages without having to corral
your directory structure to fit the requirements of a mason application.

