

==============================
Mason: Chapel's Package Manager
==============================

 Mason represents an effort by the developers on the chapel team at Cray to reach
 out to the chapel community and provide a systematic way of providing packages
 to the developers who want to build and explore in chapel. 




=========================
Installation Instructions
=========================

In the mason directory found in ``$CHPL_HOME/tools/mason`` run the following:

.. code-block:: sh
   
   make build


This will create the /.mason directory wherever your home directory
is located. It builds the mason binary so that the command line
interface can be used. The mason registry is also downloaded from
github so that a user can start to rely on mason to specifcy project
dependencies. 

.. code-block:: sh

   make install

This puts mason in the path of the user so that mason can be used
anywhere in the user's file system.

------------- 

To remove mason simply change directory to $CHPL_HOME/tools/mason and run:

.. code-block:: sh

   make clean
      
This will remove mason from the user path.




=======================
Setting up your Project
=======================
	
``mason new [project name] [--vcs=true/false]`` is the command that initializes
a new project. It also creates a git repository unless --vcs=false is included.

For example, after ``mason new MyPackage`` is run in an empty directory it will have the 
following hierarchy:

MyPackage/
  Mason.toml
  src/
    MyPackage.chpl

Mason will enforce that the main file be named after the package to enforce namespacing.
While it is common practice for package names to be PascalCase and chpl files to be lowercase,
it is an acceptable tradeoff for reliability, and MyPackage will be the first file listed in src/.





=================================
Building and Running your project
=================================

When invoked, ``mason build`` will do the following:

    - TODO: Navigate to the root of the project.
    - Run update to make sure any manual manifest edits are reflected in the dependency code.
    - Build ??.chpl in the /src directory, where ?? is the name of the project.
    - All packages are compiled into binaries and placed into target/

``mason run`` will, in turn:

    - Invoke build.
    - Run the resulting executable out of target/, if it exists.
        
For example, after ``mason run MyPackage``, the project directory appears as so:


MyPackage/
   Mason.toml
   Mason.lock
   src/
      MyPackage.chpl
   target/
      debug/
      release/
         benches/
         examples/
         MyPackage
         test/





===================
The Design of Mason
===================

    mason: Command line tool for building chapel programs to provide users with
            a consistent way of building applications and libraries. Mason uses
            a four state pipeline to go from start to finish in a project. The
	    four states are listed below.
    
    Four States:
        Project Code: ``yourProject/src/yourProject.chpl``
            This is the source code of the project the user creates using mason.
        Manifest File: Mason.toml
            Toml file containing metadata and dependencies
            Builds dependency directed acyclic graph (DAG) to be
	    serialized into lock file
        Lock File:  Mason.lock
            Contains necessary build information
            Serialized directed acyclic graph of the dependencies build options
	     from the manifest
        Dependancy Code:  ``$HOME/.mason/src``
	    Local dependencies downloaded by mason after the user lists them in
	    a project manifest.





=================
The Manifest File
=================
The Mason.toml manifest file is written in TOML(for more information see the TOML section below).
Each time a new project is created in Mason a standard TOML file in included in the top-level
directory of the project directory. 

For example, Mason.toml:

[brick]
name = "hello_world"
version = "0.1.0"
authors = ["Bradford Chamberlain <brad@chamberlain.com>"]

[dependencies]
curl = '1.0.0'




======
 TOML
======

TOML is the configuation language chosen by the chapel developers for
configuring programs written in chapel using mason. A TOML file contains
the nessescary information to build a chapel program using mason. 
`TOML Spec <https://github.com/toml-lang/toml>`_.




==============
Mason-Registry
==============

The initial mason registry is a GitHub repository containing a list of versioned manifest files.
This is not unlike that of the OS X Homebrew package manager registry.

The registry would follow a hierarchy as follows:

REGISTRY=https://github.com/chapel-lang/mason-registry

registry/
  Curl/
      1.0.0.toml
      2.0.0.toml
  RecordParser/
      1.0.0.toml
      1.1.0.toml
      1.2.0.toml
  VisualDebug/
      2.2.0.toml
      2.2.1.toml

Each versioned manifest file would be identical to the manifest file in the top-level directory
of the package repository, with one additional field that is not required in the repository manifest,
a URL pointing to the repository and revision in which the version is located.

Continuing the example from before, the 'registry' Mason.toml would include the additional source field:

[brick]
name = "hello_world"
version = "0.1.0"
authors = ["Brad Chamberlain <brad@chamberlain.com>"]
source = "https://github.com/bradcray/hello_world"

[dependencies]
curl = '1.0.0'




=========================================
To submit a package to the Mason-Registry 
=========================================

The mason registry(a repository in the chapel-lang repository) will hold the
manifest files for packages submitted by developers. To contribute a package,
all a developer has to do is host their package in a git repository, write a
manifest file(in TOML) with a source field containing the URL to the package
repository, and open a PR in the mason-registry repository! As soon as trusted
chapel developers look at your package and approve it, other users will be able
to use your package through mason simply by adding the name and version number
of your package to their project's dependencies! 

   Steps: 
      1) Write a library or binary project in chapel using Mason
      2) Host that project in a git repository. (e.g. GitHub)
      3) Add a source field to the Mason.toml file in your project's repository.
      4) Open a PR that contains just your Mason.toml file from your project in
         in the Mason-Registry repository.
      5) Wait for trusted chapel developers to approve the PR.
      6) Maintain your project and notify chapel developers if taken down. 



===========
Namespacing
===========

All packages will exist in a single common namespace with a first-come, first-served policy.
It is easier to go to separate namespaces than to roll them back, so this position affords
flexibility.



===================
Semantic Versioning
===================

To assist version resolution, the registry will enforce the following conventions:

    The format for all versions will be a.b.c.
        Major versions are denoted by a.
        Minor versions are denoted by b.
        Bug fixes are denoted by c.
    If the major version is 0, no further conventions will be enforced.
    The major version must be advanced if and only if the update causes breaking API changes,
    	such as updated data structures or removed methods and procedures. The minor and bug fix
     	versions will be zeroed out. (ex. 1.13.1 -> 2.0.0)
    The minor version must be advanced if and only if the update adds functionality to the API
    	while maintaining backward compatibility with the current major version. The bug fix 
	version will be zeroed out. (ex. 1.13.1 -> 1.14.0)
    The bug fix must be advanced for any update correcting functionality within a minor revision.
    	(ex. 1.13.1 -> 1.13.2)




Incompatible Version Resolution Strategy
-------------------------------------------
The current resolution strategy for Mason 0.1.0 is the IVRS as described below:
    1. If multiple bug fixes of a package are present in the project,
       mason will use the latest bug fix. (ex. 1.1.0, 1.1.1 --> 1.1.1)
    2. If multiple minor versions of a package are present in the project,
       mason will use the latest minor version within the common major version.
       (ex. 1.4.3, 1.7.0 --> 1.7)
    3. If multiple major versions are present, mason will print an error.
       (ex. 1.13.0, 2.1.0 --> incompatible)




=============
The Lock File
=============

The lock file (Mason.lock) is generated after running a mason update command. The user should
never manually edit the lock file as it is intended to "lock" in the settings of a certain 
project build iteration. Mason.lock is added by default to the .gitignore when a new project 
is created. If you intention is to create a binary application package that does not need to
be re-compiled by mason then simply take the Mason.lock out of your .gitignore. An example of
a lock file is written below as if generated from the earlier example of a Mason.toml:


[curl]
name = 'curl'
version = '0.1.0'
source = 'https://github.com/username/curl'


[root]
name = "hello_world"
version = "0.1.0"
authors = ["Brad Chamberlain <brad@chamberlain.com>"]
source = "https://github.com/bradcray/hello_world"
dependencies = [curl '1.0.0' 'https://github.com/username/curl']




===============
Dependency Code
===============

The src code for every package downloaded will be in $MASON_HOME which by default is placed
under the $HOME directory of the user. The path to the versioned packages downloaded by the
user would then be under ``$HOME/.mason/src/``. In the directory adjacent to the source code
directory is the user's checkout of the mason-registry. 