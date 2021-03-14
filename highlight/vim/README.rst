Chapel Mode for Vim
===================

Chapel support for Vim has been designed for use with Vim versions 5.08 or
newer.


File Descriptions:
==================

+ **ftdetect/chpl.vim**

  - Allows vim to auto-detect the syntax highlighting scheme for .chpl files.

+ **ftplugin/chpl.vim**

  - Useful editor settings supporting chpl, chpldoc

+ **syntax/chpl.vim**

  - Syntax highlighting scheme for Chapel

+ **indent/chpl.vim**

  - Indenting scheme for Chapel

+ **sample-vimrc**

  - Sample .vimrc configuration file


Installation Instructions:
==========================

vim-plug Installation:
----------------------

For users of the `vim-plug`_ plugin manager, the vim Chapel support can be
loaded as an unmanaged plugin. To do this, add the following line to the
``.vimrc``, between the ``call plug#begin()`` and ``call plug#end()`` lines:

.. code-block:: vim

    Plug '~/path/to/chapel/highlight/vim'

Where ``~/path/to/chapel`` is the path to the Chapel repository, starting from
the home directory. Chapel support should work once these changes are made
without any calls to ``:PlugInstall``, which is required for managed plugins.

Note that vim-plug uses the ``~`` character to identify that a plugin is a local
file, so the Chapel repository, or at least the ``highlight/vim/`` directory,
must be somewhere under the home directory.


.. _vim-plug: https://github.com/junegunn/vim-plug


Manual Installation:
--------------------

- Create a directory in the root of your home directory called ".vim"

  ``$ mkdir $HOME/.vim``

- Copy the contents of this directory to your ".vim" directory

  ``$ cp -r * $HOME/.vim``

- Modify your .vimrc configuration file to enable automatic filetype
  detection, syntax highlighting, and indenting (this step is optional but
  recommended).

  ``$ vim $HOME/.vimrc``

- **OR:** Use the sample .vimrc file provided (be careful you don't overwrite
  any existing .vimrc file!):

  ``$ cp -i sample-vimrc .vimrc``


Enabling Chapel Support in Vim:
===============================

If you have chosen to use the **sample-vimrc** file you can safely ignore this
section.  Vim commands are denoted ": command".  You can give these commands
manually whenever you start a vim session (with the ":") or they can also be
given automatically every time vim is started by putting them in your .vimrc
file (without the ":").


.. code-block:: vim

   " Enable automatic filetype detection
   :filetype on
   " Enable syntax highlighting scheme:
   :syntax on
   " Enable indenting scheme:
   :filetype indent on
