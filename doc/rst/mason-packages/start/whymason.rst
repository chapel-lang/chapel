:title: Mason Guide: *Why mason?*
        
Why would I want to use mason?
==============================

So, you want to write some Chapel code and find yourself asking if it would
make sense to use mason or if it would just end up adding needless overhead to your
project in the long run. Here's an answer for you: Yes! You should use mason!

Using mason brings with it a number of valuable features that are not available
to users of the old-fashioned ``chpl`` command. Some of these are:

* access to libraries contributed by the Chapel community (see :ref:`using-a-package`)
  
* portable, simple build commands (see :ref:`building-and-running`)
  
* a built-in testing framework to help test your code (see :ref:`testing-with-mason`)
  
* make your code easily shareable on GitHub (see :ref:`mason-git-dependencies`)
  
* contribute your own libraries and help the Chapel community grow (see :ref:`submit-a-package`)
  

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
already in place. This is what drives your application and where the execution will start.
This ``main()`` function is unique to the mason application package type and makes it the only
type of mason package that can be ran via ``mason run``.

When compiling Chapel code, there can only be one ``main()`` function (or you will need to use the
``--main-module`` flag to specify which one to run). This means that mason application package
types are not meant to be used as dependencies for other mason packages and cannot be added as a
dependency with the ``mason add <library-name@version>`` command. Rather, mason applications published
to the mason registry can be downloaded, built, and run using the mason install <application-name>
command (which is currently a work in progress). If you would like to create a library with functions
to be exposed to other users, the mason library is what you are looking for (see :ref:`mason-libraries`).

Some examples of when you would want to use a mason application as opposed to one of the other
types are a benchmark that you would like others to be able to compile and execute in a reproducible
way, an application that simulates the weather, or just about anywhere you have a cool program that
you would like to share.

.. _mason-libraries:

Mason Libraries
~~~~~~~~~~~~~~~
A mason library is meant to be a module that contains functions that are to be made accessible
to other users. Mason libraries do not have a ``main()`` function and cannot be run with the
``mason run`` command, but Mason library code can be executed through examples
(see :ref:`mason-examples`).

To start writing your own mason library, use the ``mason new <package-name> --lib`` command and
mason will create an empty default module. Inside the empty module you can define the
functions your library will provide to its users.

Some examples of when you would want to create a mason library as opposed to an application are:

* You want to provide a linear algebra library for other Chapel programs

* You have multiple Chapel projects that depend on a common set of functionality you wish to centralize
  
* You found some functionality missing from Chapel, and think others could benefit from your solution


Mason Lightweight Projects
~~~~~~~~~~~~~~~~~~~~~~~~~~
A mason lightweight project is simply a mason manifest file that tracks which mason packages your
Chapel code depends on. This gives all Chapel users the ability to use mason libraries
as dependencies, without forcing your project to use the folder structure defined for mason packages.

Lightweight packages cannot use most mason commands, such as ``mason build`` or ``mason run``, but a special
``mason modules`` command allows you to get the ``chpl`` compiler flags necessary to include the list of
dependencies in your Chapel project.

When you run the ``mason new --light`` command to create a lightweight package, mason creates a basic
``Mason.toml`` file in the current directory. You can add dependencies to that file by using the
``mason add <package-name@ver>`` command or specifying a git dependency (see :ref:`mason-git-dependencies`),
the same as you would for other mason package types.

After adding your project's dependencies to the ``Mason.toml`` file, use the ``mason modules`` command to
print out the flags that should be added to your build system's chpl compilation step.

The main reason you would use a lightweight mason project as opposed to one of the other
types of mason project types would be in the case that you already have a project that is
built out and has its own well-defined build process that uses something like a ``Makefile``
to handle building, so you just want to use some mason packages without having to corral
your directory structure to fit the requirements of a mason application.

