;;; chpl-mode.el --- Major mode for editing Chapel code

;; Author:     2007 Steven T Balensiefer
;; Maintainer: Chapel group <chapel_info@cray.com>
;; Created:    December 2002
;; Version:    0.7
;; Keywords:   Chapel languages oop

;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2 of the License, or
;; (at your option) any later version.
;; 
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;; 
;; You should have received a copy of the GNU General Public License
;; along with this program; see the file COPYING.  If not, write to
;; the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.

;;; Commentary:

;; This is a mode for the Chapel programming language under
;; development by Hewlett Packard Enterprise (formerly by Cray Inc.).
;; The mode itself was originally developed by Steven T Balensiefer at
;; UW as part of his course project for CSE590o, based on the
;; derived-mode-ex.el code supported with the standard cc-mode.
;;
;; Note: The interface used in this file requires CC Mode 5.30 or
;; later.

;;; Code:

(require 'cc-mode)

;; Work around "Symbol's function definition is void: set-difference"
;; in emacs 24, at least through 24.5.  Per
;; http://lists.gnu.org/archive/html/bug-gnu-emacs/2014-10/msg01175.html
(eval-when-compile
  (if (and (= emacs-major-version 24) (<= emacs-minor-version 5))
      (require 'cl)))

;; Need to exclude xemacs from some behavior
(defvar running-xemacs (string-match "XEmacs\\|Lucid" emacs-version))

;; These are only required at compile time to get the sources for the
;; language constants.  (The cc-fonts require and the font-lock
;; related constants could additionally be put inside an
;; (eval-after-load "font-lock" ...) but then some trickery is
;; necessary to get them compiled.)
(eval-when-compile
  (require 'cc-langs)
  (require 'cc-fonts))

(eval-and-compile
  ;; Make our mode known to the language constant system.  Use Java
  ;; mode as the fallback for the constants we don't change here.
  ;; This needs to be done also at compile time since the language
  ;; constants are evaluated then.
  (c-add-language 'chpl-mode 'c-mode))

;; Define chpl primitive types

(c-lang-defconst c-primitive-type-kwds
  chpl '("bool"
         "bytes"
         "complex"
         "domain"
         "imag" "index" "int"
         "locale"
         "nothing"
         "opaque"
         "range" "real"
         "string" "subdomain"
         "uint"
         "void"))

;; Define chpl type modifiers
(c-lang-defconst c-type-modifier-kwds
  "Type modifier keywords.  These can occur almost anywhere in types
but they don't build a type of themselves.  Unlike the keywords on
`c-primitive-type-kwds', they are fontified with the keyword face and
not the type face."
  chpl '("as"
         "const" "config"
         "except" "export" "extern"
         "inline" "iter"
         "module"
         "only" "override"
         "param" "private" "proc" "public"
         "require"
         "type"
         "use"
         "var"))

;; Class-style declarations
(c-lang-defconst c-class-decl-kwds
  "Keywords introducing declarations where the following block (if any)
contains another declaration level that should be considered a class.

If any of these also are on `c-type-list-kwds', `c-ref-list-kwds',
`c-colon-type-list-kwds', `c-paren-nontype-kwds', `c-paren-type-kwds',
`c-<>-type-kwds', or `c-<>-arglist-kwds' then the associated clauses
will be handled.

Note that presence on this list does not automatically treat the
following identifier as a type; the keyword must also be present on
`c-type-prefix-kwds' or `c-type-list-kwds' to accomplish that."
  chpl '("class" "record" "union"))

(c-lang-defconst c-type-start-kwds
  "Keywords that can start a type."
  chpl '("class" "enum" "record" "type" "union"))

;; Type aliases
(c-lang-defconst c-typedef-decl-kwds
  "Keywords introducing declarations where the identifier(s) being
declared are types.

If any of these also are on `c-type-list-kwds', `c-ref-list-kwds',
`c-colon-type-list-kwds', `c-paren-nontype-kwds', `c-paren-type-kwds',
`c-<>-type-kwds', or `c-<>-arglist-kwds' then the associated clauses
will be handled."
  ;; Default to `c-class-decl-kwds' and `c-brace-list-decl-kwds'
  ;; (since e.g. "Foo" is a type that's being defined in "class Foo
  ;; {...}").
  chpl '("type"))


(c-lang-defconst c-typeless-decl-kwds
  "Keywords introducing declarations where the \(first) identifier
\(declarator) follows directly after the keyword, without any type.

If any of these also are on `c-type-list-kwds', `c-ref-list-kwds',
`c-colon-type-list-kwds', `c-paren-nontype-kwds', `c-paren-type-kwds',
`c-<>-type-kwds', or `c-<>-arglist-kwds' then the associated clauses
will be handled."
  ;; Default to `c-class-decl-kwds' and `c-brace-list-decl-kwds'
  ;; (since e.g. "Foo" is the identifier being defined in "class Foo
  ;; {...}").
  chpl '("const" "iter" "module" "param" "proc" "type" "var"))

(c-lang-defconst c-ref-list-kwds
  "Keywords that may be followed by a comma separated list of
reference (i.e. namespace/scope/module) identifiers, where each
optionally can be prefixed by keywords.  (Can also be used for the
special case when the list can contain only one element.)  Assumed to
be mutually exclusive with `c-type-list-kwds'.

Note: Use `c-typeless-decl-kwds' for keywords followed by a function
or variable identifier (that's being defined)."
  chpl '("as" "except" "only" "use"))

(c-lang-defconst c-block-stmt-1-kwds
  "Statement keywords followed directly by a substatement."
  chpl '("do" "else" "then"))

(c-lang-defconst c-block-stmt-2-kwds
  "Statement keywords followed by a paren sexp and then by a substatement."
  chpl '("select"))
  ; putting "if" "for" "forall" here would cause
  ; weird indentation since parens not required

(c-lang-defconst c-simple-stmt-kwds
  "Statement keywords followed by an expression or nothing."
  chpl '("break" "continue" "label" "return" "yield"))

(c-lang-defconst c-label-kwds
  "Keywords introducing colon terminated labels in blocks."
  chpl '("otherwise" "when"))

(c-lang-defconst c-constant-kwds
  "Keywords for constants."
  chpl    '("false" "nil" "none" "true"))

(c-lang-defconst c-primary-expr-kwds
  "Keywords besides constants and operators that start primary expressions."
  chpl '("new" "delete")) ;; Not really a keyword, but practically works as one.


(c-lang-defconst c-other-kwds
  "Keywords not accounted for by any other `*-kwds' language constant."
  chpl '("align" "atomic" "begin" "borrowed" "by" "catch" "cobegin" "coforall" "deinit" "dmapped" "for" "forall" "if" "in" "init" "inout" "lifetime" "local" "noinit" "on" "out" "owned" "prototype" "reduce" "ref" "scan" "serial" "shared" "single" "sparse" "sync" "throw" "throws" "try" "unmanaged" "where" "while" "with" "zip"))

;;; Chpl.

(defun c-font-lock-chpl-new (limit)
  ;; Assuming point is after a "new" word, check that it isn't inside
  ;; a string or comment, and if so try to fontify the type in the
  ;; allocation expression.  Nil is always returned.
  ;;
  ;; As usual, Chpl takes the prize in coming up with a hard to parse
  ;; syntax. :P

  (unless (c-skip-comments-and-strings limit)
    (save-excursion
      (catch 'false-alarm
	;; A "new" keyword is followed by one to three expressions, where
	;; the type is the middle one, and the only required part.
	(let (expr1-pos expr2-pos
	      ;; Enable recording of identifier ranges in `c-forward-type'
	      ;; etc for later fontification.  Not using
	      ;; `c-fontify-types-and-refs' here since the ranges should
	      ;; be fontified selectively only when an allocation
	      ;; expression is successfully recognized.
	      (c-record-type-identifiers t)
	      c-record-ref-identifiers
	      ;; The font-lock package in Emacs is known to clobber
	      ;; `parse-sexp-lookup-properties' (when it exists).
	      (parse-sexp-lookup-properties
	       (cc-eval-when-compile
		 (boundp 'parse-sexp-lookup-properties))))
	  (c-forward-syntactic-ws)

	  ;; The first placement arglist is always parenthesized, if it
	  ;; exists.
	  (when (eq (char-after) ?\()
	    (setq expr1-pos (1+ (point)))
	    (condition-case nil
		(c-forward-sexp)
	      (scan-error (throw 'false-alarm t)))
	    (c-forward-syntactic-ws))

	  ;; The second expression is either a type followed by some "*" or
	  ;; "[...]" or similar, or a parenthesized type followed by a full
	  ;; identifierless declarator.
	  (setq expr2-pos (1+ (point)))
	  (cond ((eq (char-after) ?\())
		((let ((c-promote-possible-types t))
		   (c-forward-type)))
		(t (setq expr2-pos nil)))

	  (when expr1-pos
	    (cond
	     ((not expr2-pos)
	      ;; No second expression, so the first has to be a
	      ;; parenthesized type.
	      (goto-char expr1-pos)
	      (let ((c-promote-possible-types t))
		(c-forward-type)))

	     ((eq (char-before expr2-pos) ?\()
	      ;; Got two parenthesized expressions, so we have to look
	      ;; closer at them to decide which is the type.  No need to
	      ;; handle `c-record-ref-identifiers' since all references
	      ;; has already been handled by other fontification rules.
	      (let (expr1-res expr2-res)

		(goto-char expr1-pos)
		(when (setq expr1-res (c-forward-type))
		  (unless (looking-at
			   (cc-eval-when-compile
			     (concat (c-lang-const c-symbol-start chpl)
				     "\\|[*:\)\[]")))
		    ;; There's something after the would-be type that
		    ;; can't be there, so this is a placement arglist.
		    (setq expr1-res nil)))

		(goto-char expr2-pos)
		(when (setq expr2-res (c-forward-type))
		  (unless (looking-at
			   (cc-eval-when-compile
			     (concat (c-lang-const c-symbol-start chpl)
				     "\\|[*:\)\[]")))
		    ;; There's something after the would-be type that can't
		    ;; be there, so this is an initialization expression.
		    (setq expr2-res nil))
		  (when (and (c-go-up-list-forward)
			     (progn (c-forward-syntactic-ws)
				    (eq (char-after) ?\()))
		    ;; If there's a third initialization expression
		    ;; then the second one is the type, so demote the
		    ;; first match.
		    (setq expr1-res nil)))

		;; We fontify the most likely type, with a preference for
		;; the first argument since a placement arglist is more
		;; unusual than an initializer.
		(cond ((memq expr1-res '(t known prefix)))
		      ((memq expr2-res '(t known prefix)))
		      ((eq expr1-res 'found)
		       (let ((c-promote-possible-types t))
			 (goto-char expr1-pos)
			 (c-forward-type)))
		      ((eq expr2-res 'found)
		       (let ((c-promote-possible-types t))
			 (goto-char expr2-pos)
			 (c-forward-type)))
		      ((and (eq expr1-res 'maybe) (not expr2-res))
		       (let ((c-promote-possible-types t))
			 (goto-char expr1-pos)
			 (c-forward-type)))
		      ((and (not expr1-res) (eq expr2-res 'maybe))
		       (let ((c-promote-possible-types t))
			 (goto-char expr2-pos)
			 (c-forward-type)))
		      ;; If both type matches are 'maybe then we're
		      ;; too uncertain to promote either of them.
		      )))))

	  ;; Fontify the type that now is recorded in
	  ;; `c-record-type-identifiers', if any.
	  (c-fontify-recorded-types-and-refs)))))
  nil)

(c-override-default-keywords 'chpl-font-lock-keywords)

(defconst chpl-font-lock-keywords-1 (c-lang-const c-matchers-1 chpl)
  "Minimal font locking for Chpl mode.
Fontifies only preprocessor directives (in addition to the syntactic
fontification of strings and comments).")

(defconst chpl-font-lock-keywords-2 (c-lang-const c-matchers-2 chpl)
  "Fast normal font locking for Chpl mode.
In addition to `chpl-font-lock-keywords-1', this adds fontification of
keywords, simple types, declarations that are easy to recognize, the
user defined types on `chpl-font-lock-extra-types', and the doc comment
styles specified by `c-doc-comment-style'.")

(defconst chpl-font-lock-keywords-3 (c-lang-const c-matchers-3 chpl)
  "Accurate normal font locking for Chpl mode.
Like `chpl-font-lock-keywords-2' but detects declarations in a more
accurate way that works in most cases for arbitrary types without the
need for `chpl-font-lock-extra-types'.")

(defvar chpl-font-lock-keywords chpl-font-lock-keywords-3
  "Default expressions to highlight in Chpl mode.")

(defun chpl-font-lock-keywords-2 ()
  (c-compose-keywords-list chpl-font-lock-keywords-2))
(defun chpl-font-lock-keywords-3 ()
  (c-compose-keywords-list chpl-font-lock-keywords-3))
(defun chpl-font-lock-keywords ()
  (c-compose-keywords-list chpl-font-lock-keywords))

(defvar cc-imenu-chpl-generic-expression
  '((nil "^[ \t]*\\def\\|(function\\)[ \t\n]+\\([a-zA-Z0-9_.:]+\\)" 2))
  "Imenu expression for Chapel-mode.  See `imenu-generic-expression'.")


;; Support for Chapel mode

;;;###autoload
(defvar chpl-mode-syntax-table nil
  "Syntax table used in chpl-mode buffers.")
(or chpl-mode-syntax-table
    (setq chpl-mode-syntax-table
	  (funcall (c-lang-const c-make-mode-syntax-table chpl))))

;; Nested block comments -- add "n" to the syntax table entry for "*"
;; https://www.gnu.org/software/emacs/manual/html_node/elisp/Syntax-Flags.html#Syntax-Flags
(if (not running-xemacs)
    (modify-syntax-entry ?* ". 23n" chpl-mode-syntax-table))

(defvar chpl-mode-abbrev-table nil
  "Abbreviation table used in chpl-mode buffers.")
(c-define-abbrev-table 'chpl-mode-abbrev-table
  '(("else" "else" c-electric-continued-statement 0)
    ("while" "while" c-electric-continued-statement 0)
    ("catch" "catch" c-electric-continued-statement 0)))

(defvar chpl-mode-map ()
  "Keymap used in chpl-mode buffers.")
(if chpl-mode-map
    nil
  (setq chpl-mode-map (c-make-inherited-keymap))
  ;; add bindings which are only useful for chpl
  (define-key chpl-mode-map "\C-c\C-e" 'c-macro-expand)
  (define-key chpl-mode-map "\C-c:"    'c-scope-operator)
  (define-key chpl-mode-map "<"        'c-electric-lt-gt)
  (define-key chpl-mode-map ">"        'c-electric-lt-gt))

(easy-menu-define c-chpl-menu chpl-mode-map "chpl Mode Commands"
		  (cons "chpl" (c-lang-const c-mode-menu chpl)))

;;;###autoload
(add-to-list 'auto-mode-alist '("\\.chpl\\'" . chpl-mode))

;;;###autoload
(defun chpl-mode ()
  "Major mode for editing Chapel code.

To see what version of CC Mode you are running, enter `\\[c-version]'.

The hook `c-mode-common-hook' is run with no args at mode
initialization, then `chpl-mode-hook'.

Key bindings:
\\{chpl-mode-map}"
  (interactive)
  (kill-all-local-variables)
  (c-initialize-cc-mode t)
  (set-syntax-table chpl-mode-syntax-table)
  (setq major-mode 'chpl-mode
	mode-name "Chapel"
	local-abbrev-table chpl-mode-abbrev-table
	abbrev-mode t)
  (use-local-map chpl-mode-map)
  (c-init-language-vars chpl-mode)
  (c-common-init 'chpl-mode)
  (easy-menu-add c-chpl-menu)
  (cc-imenu-init cc-imenu-chpl-generic-expression)
  (run-hooks 'c-mode-common-hook)
  (run-hooks 'chpl-mode-hook)
  (c-update-modeline))

(provide 'chpl-mode)

;;; chpl-mode.el ends here
