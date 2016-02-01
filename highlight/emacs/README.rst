=================
Chapel emacs mode
=================

This directory subtree contains emacs e-lisp files that perform simple
formatting of Chapel code.  Support for Chapel formatting with these
files is far from perfect, but far better than having no formatting at
all (we find).  Versions 22-24 are the main versions that we are
currently supporting. Thanks to Steve Balensiefer for providing the
initial version of the Chapel mode for these versions.

The sources in this subdirectory are licensed under the GPL.  See the
COPYING file in this directory for more information.

To use these modes, it is suggested that you install these directories
so that they appear under your Chapel release as follows::

   $CHPL_HOME/    -- the location of your normal Chapel installation
     etc/
       emacs/     -- this directory

In which case, you should be able to use these modes by adding the
following lines to your standard ``.emacs`` or ``.emacs.d/init.el`` file:


.. code-block:: emacs-lisp

    ; query the environment variable CHPL_HOME
    (setq chpl-home (getenv "CHPL_HOME"))

    ; prepend $CHPL_HOME/etc/emacs to your emacs load-path
    (if (string= chpl-home nil)
        nil
      (add-to-list 'load-path (concat chpl-home
                                      "/etc/emacs/"
                                      )
                   )
      )

    ; make sure that when chpl-mode is entered, (our modified) cc-mode is
    ; loaded
    (autoload 'chpl-mode "chpl-mode" "Chpl enhanced cc-mode" t)

    ; make loading files with a .chpl extension put emacs into chpl-mode
    (add-to-list 'auto-mode-alist '("\\.chpl$" . chpl-mode))
