;;; dparserpy.el --- support for working with DParser for Python
;;;
;;; Author: Yves Forkl <Yves.Forkl@gmx.net>
;;; Keywords: parser, grammar, syntax
;;; Version: 0.1 (2004.04.27)

;; Copyright (C) 2004 Yves Forkl

;; This file is not currently part of GNU Emacs.

;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License as
;; published by the Free Software Foundation; either version 2, or (at
;; your option) any later version.

;; This program is distributed in the hope that it will be useful, but
;; WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;; General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with this program ; see the file COPYING.  If not, write to
;; the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.

;;; Commentary:

;; This package tries to provide some support for developing and
;; running parsers using DParser for Python [1] by Brian Sabbey, a
;; Python interface to DParser [2] by John Plevyak.

;; Code in this package directly relates to DParser for Python, while
;; general Emacs Lisp code for DParser should be found in dparser.el.

;; [1] see http://staff.washington.edu/sabbey/py_dparser/
;; [2] see http://dparser.sourceforge.net/

;; Send suggestions, bug reports etc. to the package maintainer:
;; <Yves.Forkl@gmx.net>

;;; Change Log:

;; V.  0.1      initial release

;;; Code:

(defun dparserpy-goto-trouble-spot ()
  "Visit place in input file that caused a DParser for Python syntax error.

Call this function from within the custom syntax error message looking
like this:

-------------------------------------------------------------------------------
File /path/to/inputfile:

[...] text preceding the error ____________
                              /syntax error at line X (buffer pos. Y)
                              ERROR SPOT and more text [...]
-------------------------------------------------------------------------------

This will show the input file in an another window and move Point to
the trouble spot."
  (interactive)
  (let ((startpos (point))
        (startwindow (get-buffer-window (current-buffer))))
    (search-backward (make-string 79 ?-))
    (search-forward "\nFile ")
    (let ((filename (buffer-substring (point) (1- (point-at-eol))))
          (pos
           (string-to-int
            (buffer-substring
             (re-search-forward
              "/syntax error at line [0-9]+ (buffer pos. ")
             (1- (point-at-eol))))))
      (goto-char startpos)
      (find-file-other-window filename)
      (goto-char (1+ pos))
      (recenter startwindow)
      (recenter))))

(require 'thingatpt)

(defun dparserpy-insert-action-template ()
  "Within DParser Python buffer, insert template for action name at Point."
  (interactive)
  (let ((bounds (bounds-of-thing-at-point 'word)))
    (if bounds
        (let* ((start (car bounds))
               (end (cdr bounds))
               (identifier (buffer-substring start end)))
          (kill-region start end)
          (insert "def d_" identifier "(t):\n"
                  "\"\"\" " identifier " :\n"
                  "\"\"\"\n")
          (beginning-of-line -1)
          (indent-according-to-mode)
          (beginning-of-line 2)
          (indent-according-to-mode)
          (beginning-of-line)
          (search-forward "\"\"\"")
          (backward-char 3)
          (insert " ")
          (backward-char 1)))))
