.. _emacs:

Emacs
-----

.. _emacs-syntax:

Syntax Highlighting
~~~~~~~~~~~~~~~~~~~

To get syntax highlighting for Chapel in Emacs, you can use the files in the
top-level ``highlight/emacs`` directory of the Chapel source tree.  These files
are licensed under the GPL, see ``highlight/emacs/LICENSE``.  To use them, take
the following steps:

1. Copy/move the ``highlight/emacs`` directory to a known directory.
   If you installed Chapel from a package manager, you may need to download the
   Chapel source code to get these files.

2. Add the following lines to your standard ``.emacs`` or ``.emacs.d/init.el`` file:

.. code-block:: emacs-lisp


    ; prepend the path to 'highlight/emacs' to your emacs load-path
    (if (string= chpl-home nil)
        nil
      (add-to-list 'load-path "/path/to/chapel/highlight/emacs")
      )

    ; make sure that when chpl-mode is entered, (our modified) cc-mode is
    ; loaded
    (autoload 'chpl-mode "chpl-mode" "Chpl enhanced cc-mode" t)

    ; make loading files with a .chpl extension put emacs into chpl-mode
    (add-to-list 'auto-mode-alist '("\\.chpl$" . chpl-mode))


.. _emacs-lsp:

Language Server Support
~~~~~~~~~~~~~~~~~~~~~~~

With Emacs 29.1, support has been added for language server protocols via `Eglot
<https://www.gnu.org/software/emacs/manual/html_mono/eglot.html>`_

To utilize the :ref:`Chapel language server <readme-chpl-language-server>` and/or :ref:`readme-chplcheck` with Eglot, add the following to your
``.emacs`` or ``.emacs.d/init.el`` file (note that this assumes you have already followed :ref:`the directions for syntax highlighting <emacs-syntax>`):

.. code-block:: lisp

    (with-eval-after-load 'eglot
      (add-to-list 'eglot-server-programs
                   '(chpl-mode . ("chpl-language-server" "--chplcheck"))))

This will enable using the language server with a particular ``.chpl`` file by
calling ``M-x eglot``.

To automatically use Eglot and the language server with every ``.chpl`` file,
additionally add the following to your ``.emacs`` file:

.. code-block:: lisp

    (add-hook 'chpl-mode-hook 'eglot-ensure)

.. note::

   The above uses the ``--chplcheck`` flag to enable additional diagnostics from
   ``chplcheck``. If you do not want to use ``chplcheck``, you can remove this.
