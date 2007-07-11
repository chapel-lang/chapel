;;; pascal.el --- major mode for editing pascal source in Emacs

;; Copyright (C) 1993, 94, 95, 96, 97, 98, 1999, 2000 Free Software Foundation, Inc.

;; Modified: by Bradford Chamberlain and Steven Deitz in 2007 to
;;           support a Chapel mode and coloring.  This file was
;;           created by copying the emacs pascal.el file and modifying
;;           it to incorporate Chapel concepts.  These hacks are being
;;           made available to the Chapel user community for their
;;           convenience and are not intended to be supported by the
;;           original authors named below, nor to be folded back into
;;           the emacs source tree without sufficient additional work
;;           and popularization of Chapel.
;;
;; Comments: Please send comments, feedback, and bugs regarding the
;;           Chapel mode to: chapel_info@cray.com.

;; Author: Espen Skoglund <esk@gnu.org>
;; Keywords: languages

;; This file is part of GNU Emacs.

;; GNU Emacs is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.

;; GNU Emacs is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs; see the file COPYING.  If not, write to
;; the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.

;;; Commentary:

;; USAGE
;; =====

;; Emacs should enter Pascal mode when you find a Pascal source file.
;; When you have entered Pascal mode, you may get more info by pressing
;; C-h m. You may also get online help describing various functions by:
;; C-h f <Name of function you want described>

;; If you want to customize Pascal mode to fit you better, you may add
;; these lines (the values of the variables presented here are the defaults):
;;
;; ;; User customization for Pascal mode
;; (setq pascal-indent-level       3
;;       pascal-case-indent        2
;;       pascal-auto-newline       nil
;;       pascal-tab-always-indent  t
;;       pascal-auto-endcomments   t
;;       pascal-auto-lineup        '(all)
;;       pascal-toggle-completions nil
;;       pascal-type-keywords      '("array" "file" "packed" "char"
;; 				     "int" "real" "string" "record")
;;       pascal-start-keywords     '("begin" "end" "function" "procedure"
;; 				     "repeat" "until" "while" "read" "readln"
;; 				     "reset" "rewrite" "write" "writeln")
;;       pascal-separator-keywords '("downto" "else" "mod" "div" "then"))

;; KNOWN BUGS / BUGREPORTS
;; =======================
;; As far as I know, there are no bugs in the current version of this
;; package.  This may not be true however, since I never use this mode
;; myself and therefore would never notice them anyway.   If you do
;; find any bugs, you may submit them to: esk@gnu.org as well as to
;; bug-gnu-emacs@gnu.org.

;;; Code:

(defgroup chapel nil
  "Major mode for editing Chapel source in Emacs"
  :group 'languages)

(defvar chapel-mode-abbrev-table nil
  "Abbrev table in use in Chapel-mode buffers.")
(define-abbrev-table 'chapel-mode-abbrev-table ())

(defvar chapel-mode-map ()
  "Keymap used in Chapel mode.")
(if chapel-mode-map
    ()
  (setq chapel-mode-map (make-sparse-keymap))
  (define-key chapel-mode-map ";"        'electric-chapel-semi-or-dot)
  (define-key chapel-mode-map "."        'electric-chapel-semi-or-dot)
  (define-key chapel-mode-map ":"        'electric-chapel-colon)
  (define-key chapel-mode-map "="        'electric-chapel-equal)
  (define-key chapel-mode-map "#"        'electric-chapel-hash)
  (define-key chapel-mode-map "\r"       'electric-chapel-terminate-line)
  (define-key chapel-mode-map "\t"       'electric-chapel-tab)
  (define-key chapel-mode-map "\M-\t"    'chapel-complete-word)
  (define-key chapel-mode-map "\M-?"     'chapel-show-completions)
  (define-key chapel-mode-map "\177"     'backward-delete-char-untabify)
  (define-key chapel-mode-map "\M-\C-h"  'chapel-mark-defun)
  (define-key chapel-mode-map "\C-c\C-b" 'chapel-insert-block)
  (define-key chapel-mode-map "\M-*"     'chapel-star-comment)
  (define-key chapel-mode-map "\C-c\C-c" 'chapel-comment-area)
  (define-key chapel-mode-map "\C-c\C-u" 'chapel-uncomment-area)
  (define-key chapel-mode-map "\M-\C-a"  'chapel-beg-of-defun)
  (define-key chapel-mode-map "\M-\C-e"  'chapel-end-of-defun)
  (define-key chapel-mode-map "\C-c\C-d" 'chapel-goto-defun)
  (define-key chapel-mode-map "\C-c\C-o" 'chapel-outline)
;;; A command to change the whole buffer won't be used terribly
;;; often, so no need for a key binding.
;  (define-key chapel-mode-map "\C-cd"    'chapel-downcase-keywords)
;  (define-key chapel-mode-map "\C-cu"    'chapel-upcase-keywords)
;  (define-key chapel-mode-map "\C-cc"    'chapel-capitalize-keywords)
  )

(defvar chapel-imenu-generic-expression
  '((nil "^[ \t]*\\def\\|(function\\)[ \t\n]+\\([a-zA-Z0-9_.:]+\\)" 2))
  "Imenu expression for Chapel-mode.  See `imenu-generic-expression'.")

(defvar chapel-keywords
  '("and" "array" "case" "const" "def" "div" "do" "downto" "else" "end"
    "file" "for" "function" "goto" "if" "in" "label" "module" "mod" "nil" "not" "of"
    "or" "packed" "param" "record" "repeat" "set" "then" "to"
    "type" "until" "var" "while" "with"
    ;; The following are not standard in chapel, but widely used.
    "get" "put" "input" "output" "read" "readln" "reset" "rewrite" "write"
    "writeln"))

;;;
;;; Regular expressions used to calculate indent, etc.
;;;
(defconst chapel-symbol-re      "\\<[a-zA-Z_][a-zA-Z_0-9.]*\\>")
(defconst chapel-beg-block-re   "\\<\\(case\\|record\\|repeat\\|{\\)\\>")
(defconst chapel-end-block-re   "\\<\\(end\\|until\\|}\\)\\>")
(defconst chapel-declaration-re "\\<\\(const\\|label\\|type\\|var\\|param\\)\\>")
(defconst chapel-progbeg-re     "\\<\\module\\>")
(defconst chapel-defun-re       "\\<\\(function\\|def\\|module\\)\\>")
(defconst chapel-sub-block-re   "\\<\\(if\\|else\\|for\\|while\\|with\\)\\>")
(defconst chapel-noindent-re    "\\<\\(end\\|until\\|else\\|{\\)\\>")
(defconst chapel-nosemi-re      "\\<\\(repeat\\|then\\|do\\|else\\|{\\)\\>")
(defconst chapel-autoindent-lines-re
  "\\<\\(label\\|var\\|type\\|const\\|until\\|end\\|repeat\\|else\\|param\\|{\\)\\>")

;;; Strings used to mark beginning and end of excluded text
(defconst chapel-exclude-str-start "{-----\\/----- EXCLUDED -----\\/-----")
(defconst chapel-exclude-str-end " -----/\\----- EXCLUDED -----/\\-----}")

(defvar chapel-mode-syntax-table nil
  "Syntax table in use in Chapel-mode buffers.")

(if chapel-mode-syntax-table
    ()
  (setq chapel-mode-syntax-table (make-syntax-table))
  (modify-syntax-entry ?\\ "."   chapel-mode-syntax-table)
  (modify-syntax-entry ?/ ". 124b"  chapel-mode-syntax-table)
  (modify-syntax-entry ?* ". 23" chapel-mode-syntax-table)
;  (modify-syntax-entry ?{ "<"    chapel-mode-syntax-table)
;  (modify-syntax-entry ?} ">"    chapel-mode-syntax-table)
  (modify-syntax-entry ?+ "."    chapel-mode-syntax-table)
  (modify-syntax-entry ?- "."    chapel-mode-syntax-table)
  (modify-syntax-entry ?= "."    chapel-mode-syntax-table)
  (modify-syntax-entry ?% "."    chapel-mode-syntax-table)
  (modify-syntax-entry ?< "."    chapel-mode-syntax-table)
  (modify-syntax-entry ?> "."    chapel-mode-syntax-table)
  (modify-syntax-entry ?& "."    chapel-mode-syntax-table)
  (modify-syntax-entry ?| "."    chapel-mode-syntax-table)
  (modify-syntax-entry ?_ "_"    chapel-mode-syntax-table)
  (modify-syntax-entry ?\' "\""  chapel-mode-syntax-table))

(defconst chapel-font-lock-keywords (purecopy
  (list
   '("^[ \t]*\\(def\\|function\\)\\>[ \t]*\\([a-z]\\)"
     1 font-lock-keyword-face)
   '("^[ \t]*\\(def\\|function\\|pro\\(cedure\\|gram\\)\\)\\>[ \t]*\\([a-z][a-z0-9_]*\\)"
     3 font-lock-function-name-face t)
;   ("type" "const" "real" "int" "char" "boolean" "var"
;    "record" "array" "file")
   (cons (concat "\\<\\(array\\|boolean\\|c\\(har\\|onst\\)\\|file\\|"
		 "int\\|re\\(al\\|cord\\)\\|type\\|var\\|param)\\>")
	 'font-lock-type-face)
   '("\\<\\(label\\|external\\|forward\\)\\>" . font-lock-constant-face)
   '("\\<\\([0-9]+\\)[ \t]*:" 1 font-lock-function-name-face)
;   ("of" "to" "for" "if" "then" "else" "case" "while"
;    "do" "until" "and" "or" "not" "in" "with" "repeat" "end")
   (concat "\\<\\("
	   "and\\|case\\|do\\|e\\(lse\\|nd\\)\\|for\\|i[fn]\\|"
	   "not\\|o[fr]\\|repeat\\|t\\(hen\\|o\\)\\|until\\|w\\(hile\\|ith\\)"
	   "\\)\\>")
   '("\\<\\(goto\\)\\>[ \t]*\\([0-9]+\\)?"
     1 font-lock-keyword-face)
   '("\\<\\(goto\\)\\>[ \t]*\\([0-9]+\\)?"
     2 font-lock-keyword-face t)))
  "Additional expressions to highlight in Chapel mode.")
(put 'chapel-mode 'font-lock-defaults '(chapel-font-lock-keywords nil t))

(defcustom chapel-indent-level 3
  "*Indentation of Chapel statements with respect to containing block."
  :type 'integer
  :group 'chapel)

(defcustom chapel-case-indent 2
  "*Indentation for case statements."
  :type 'integer
  :group 'chapel)

(defcustom chapel-auto-newline nil
  "*Non-nil means automatically insert newlines in certain cases.
These include after semicolons and after the punctuation mark after an `end'."
  :type 'boolean
  :group 'chapel)

(defcustom chapel-indent-nested-functions t
  "*Non-nil means nested functions are indented."
  :type 'boolean
  :group 'chapel)

(defcustom chapel-tab-always-indent t
  "*Non-nil means TAB in Chapel mode should always reindent the current line.
If this is nil, TAB inserts a tab if it is at the end of the line
and follows non-whitespace text."
  :type 'boolean
  :group 'chapel)

(defcustom chapel-auto-endcomments t
  "*Non-nil means automatically insert comments after certain `end's.
Specifically, this is done after the ends of cases statements and functions.
The name of the function or case is included between the braces."
  :type 'boolean
  :group 'chapel)

(defcustom chapel-auto-lineup '(all)
  "*List of contexts where auto lineup of :'s or ='s should be done.
Elements can be of type: 'paramlist', 'declaration' or 'case', which will
do auto lineup in parameterlist, declarations or case-statements
respectively. The word 'all' will do all lineups. '(case paramlist) for
instance will do lineup in case-statements and parameterlist, while '(all)
will do all lineups."
  :type '(set :extra-offset 8
	      (const :tag "Everything" all)
	      (const :tag "Parameter lists" paramlist)
	      (const :tag "Decalrations" declaration)
	      (const :tag "Case statements" case))
  :group 'chapel)

(defcustom chapel-toggle-completions nil
  "*Non-nil means \\<chapel-mode-map>\\[chapel-complete-word] should try all possible completions one by one.
Repeated use of \\[chapel-complete-word] will show you all of them.
Normally, when there is more than one possible completion,
it displays a list of all possible completions."
  :type 'boolean
  :group 'chapel)

(defcustom chapel-type-keywords
  '("bool" "class" "complex" "imag" "int" "locale" "real" "record" "string" "uint" "union")
  "*Keywords for types used when completing a word in a declaration or parmlist.
These include integer, real, char, etc.
The types defined within the Chapel program
are handled in another way, and should not be added to this list."
  :type '(repeat (string :tag "Keyword"))
  :group 'chapel)

(defcustom chapel-start-keywords
  '("def" "end" "function" "repeat" "until" "while"
    "read" "readln" "reset" "rewrite" "write" "writeln")
  "*Keywords to complete when standing at the first word of a statement.
These are keywords such as begin, repeat, until, readln.
The procedures and variables defined within the Chapel program
are handled in another way, and should not be added to this list."
  :type '(repeat (string :tag "Keyword"))
  :group 'chapel)

(defcustom chapel-separator-keywords
  '("downto" "else" "mod" "div" "then")
  "*Keywords to complete when NOT standing at the first word of a statement.
These are keywords such as downto, else, mod, then.
Variables and function names defined within the Chapel program
are handled in another way, and should not be added to this list."
  :type '(repeat (string :tag "Keyword"))
  :group 'chapel)


;;;
;;;  Macros
;;;

(defsubst chapel-get-beg-of-line (&optional arg)
  (save-excursion
    (beginning-of-line arg)
    (point)))

(defsubst chapel-get-end-of-line (&optional arg)
  (save-excursion
    (end-of-line arg)
    (point)))

(defun chapel-declaration-end ()
  (let ((nest 1))
    (while (and (> nest 0)
		(re-search-forward
		 "[:=]\\|\\(\\<record\\>\\)\\|\\(\\<end\\>\\)"
		 (save-excursion (end-of-line 2) (point)) t))
      (cond ((match-beginning 1) (setq nest (1+ nest)))
	    ((match-beginning 2) (setq nest (1- nest)))
	    ((looking-at "[^(\n]+)") (setq nest 0))))))


(defun chapel-declaration-beg ()
  (let ((nest 1))
    (while (and (> nest 0)
		(re-search-backward "[:=]\\|\\<\\(type\\|var\\|label\\|const\\|param\\)\\>\\|\\(\\<record\\>\\)\\|\\(\\<end\\>\\)" (chapel-get-beg-of-line 0) t))
      (cond ((match-beginning 1) (setq nest 0))
	    ((match-beginning 2) (setq nest (1- nest)))
	    ((match-beginning 3) (setq nest (1+ nest)))))
    (= nest 0)))


(defsubst chapel-within-string ()
  (save-excursion
    (nth 3 (parse-partial-sexp (chapel-get-beg-of-line) (point)))))


;;;###autoload
(defun chapel-mode ()
  "Major mode for editing Chapel code. \\<chapel-mode-map>
TAB indents for Chapel code.  Delete converts tabs to spaces as it moves back.

\\[chapel-complete-word] completes the word around current point with respect \
to position in code
\\[chapel-show-completions] shows all possible completions at this point.

Other useful functions are:

\\[chapel-mark-defun]\t- Mark function.
\\[chapel-insert-block]\t- insert begin ... end;
\\[chapel-star-comment]\t- insert (* ... *)
\\[chapel-comment-area]\t- Put marked area in a comment, fixing nested comments.
\\[chapel-uncomment-area]\t- Uncomment an area commented with \
\\[chapel-comment-area].
\\[chapel-beg-of-defun]\t- Move to beginning of current function.
\\[chapel-end-of-defun]\t- Move to end of current function.
\\[chapel-goto-defun]\t- Goto function prompted for in the minibuffer.
\\[chapel-outline]\t- Enter chapel-outline-mode (see also chapel-outline).

Variables controlling indentation/edit style:

 chapel-indent-level (default 3)
    Indentation of Chapel statements with respect to containing block.
 chapel-case-indent (default 2)
    Indentation for case statements.
 chapel-auto-newline (default nil)
    Non-nil means automatically newline after semicolons and the punctuation
    mark after an end.
 chapel-indent-nested-functions (default t)
    Non-nil means nested functions are indented.
 chapel-tab-always-indent (default t)
    Non-nil means TAB in Chapel mode should always reindent the current line,
    regardless of where in the line point is when the TAB command is used.
 chapel-auto-endcomments (default t)
    Non-nil means a comment { ... } is set after the ends which ends cases and
    functions. The name of the function or case will be set between the braces.
 chapel-auto-lineup (default t)
    List of contexts where auto lineup of :'s or ='s should be done.

See also the user variables chapel-type-keywords, chapel-start-keywords and
chapel-separator-keywords.

Turning on Chapel mode calls the value of the variable chapel-mode-hook with
no args, if that value is non-nil."
  (interactive)
  (kill-all-local-variables)
  (use-local-map chapel-mode-map)
  (setq major-mode 'chapel-mode)
  (setq mode-name "Chapel")
  (setq local-abbrev-table chapel-mode-abbrev-table)
  (set-syntax-table chapel-mode-syntax-table)
  (make-local-variable 'indent-line-function)
  (setq indent-line-function 'chapel-indent-line)
  (make-local-variable 'comment-indent-function)
  (setq comment-indent-function 'chapel-indent-comment)
  (make-local-variable 'parse-sexp-ignore-comments)
  (setq parse-sexp-ignore-comments nil)
  (make-local-variable 'blink-matching-paren-dont-ignore-comments)
  (setq blink-matching-paren-dont-ignore-comments t)
  (make-local-variable 'case-fold-search)
  (setq case-fold-search t)
  (make-local-variable 'comment-start)
  (setq comment-start "/\\*")
  (make-local-variable 'comment-start-skip)
  (setq comment-start-skip "/\\*+ *\\")
  (make-local-variable 'comment-end)
  (setq comment-end "\\*/")
  ;; Font lock support
  (make-local-variable 'font-lock-defaults)
  (setq font-lock-defaults '(chapel-font-lock-keywords nil t))
  ;; Imenu support
  (make-local-variable 'imenu-generic-expression)
  (setq imenu-generic-expression chapel-imenu-generic-expression)
  (setq imenu-case-fold-search t)
  (run-hooks 'chapel-mode-hook))



;;;
;;;  Electric functions
;;;
(defun electric-chapel-terminate-line ()
  "Terminate line and indent next line."
  (interactive)
  ;; First, check if current line should be indented
  (save-excursion
    (beginning-of-line)
    (skip-chars-forward " \t")
    (if (looking-at chapel-autoindent-lines-re)
	(chapel-indent-line)))
  (delete-horizontal-space) ; Removes trailing whitespaces
  (newline)
  ;; Indent next line
  (chapel-indent-line)
  ;; Maybe we should set some endcomments
  (if chapel-auto-endcomments
      (chapel-set-auto-comments))
  ;; Check if we shall indent inside comment
  (let ((setstar nil))
    (save-excursion
      (forward-line -1)
      (skip-chars-forward " \t")
      (cond ((looking-at "\\*[ \t]+/")
	     ;; Delete region between `*' and `)' if there is only whitespaces.
	     (forward-char 1)
	     (delete-horizontal-space))
	    ((and (looking-at "/\\*\\|\\*[^/]")
		  (not (save-excursion
			 (search-forward "*/" (chapel-get-end-of-line) t))))
	     (setq setstar t))))
    ;; If last line was a star comment line then this one shall be too.
    (if (null setstar)
	(chapel-indent-line)
      (insert "*  "))))


(defun electric-chapel-semi-or-dot ()
  "Insert `;' or `.' character and reindent the line."
  (interactive)
  (insert last-command-char)
  (save-excursion
    (beginning-of-line)
    (chapel-indent-line))
  (if chapel-auto-newline
      (electric-chapel-terminate-line)))

(defun electric-chapel-colon ()
  "Insert `:' and do all indentions except line indent on this line."
  (interactive)
  (insert last-command-char)
  ;; Do nothing if within string.
  (if (chapel-within-string)
      ()
    (save-excursion
      (beginning-of-line)
      (chapel-indent-line))
    (let ((chapel-tab-always-indent nil))
      (chapel-indent-command))))

(defun electric-chapel-equal ()
  "Insert `=', and do indention if within type declaration."
  (interactive)
  (insert last-command-char)
  (if (eq (car (chapel-calculate-indent)) 'declaration)
      (let ((chapel-tab-always-indent nil))
	(chapel-indent-command))))

(defun electric-chapel-hash ()
  "Insert `#', and indent to column 0 if this is a CPP directive."
  (interactive)
  (insert last-command-char)
  (if (save-excursion (beginning-of-line) (looking-at "^[ \t]*#"))
      (save-excursion (beginning-of-line)
		      (delete-horizontal-space))))

(defun electric-chapel-tab ()
  "Function called when TAB is pressed in Chapel mode."
  (interactive)
  ;; Do nothing if within a string or in a CPP directive.
  (if (or (chapel-within-string)
	  (and (not (bolp))
	       (save-excursion (beginning-of-line) (eq (following-char) ?#))))
      (insert "\t")
    ;; If chapel-tab-always-indent, indent the beginning of the line.
    (if chapel-tab-always-indent
	(save-excursion
	  (beginning-of-line)
	  (chapel-indent-line))
      (if (save-excursion
	    (skip-chars-backward " \t")
	    (bolp))
	  (chapel-indent-line)
	(insert "\t")))
    (chapel-indent-command)))



;;;
;;; Interactive functions
;;;
(defun chapel-insert-block ()
  "Insert Chapel begin ... end; block in the code with right indentation."
  (interactive)
  (insert "{")
  (electric-chapel-terminate-line)
  (save-excursion
    (newline)
    (insert "}")
    (beginning-of-line)
    (chapel-indent-line)))

(defun chapel-star-comment ()
  "Insert Chapel star comment at point."
  (interactive)
  (chapel-indent-line)
  (insert "/*")
  (electric-chapel-terminate-line)
  (save-excursion
    (electric-chapel-terminate-line)
    (delete-horizontal-space)
    (insert "/"))
  (insert "  "))

(defun chapel-mark-defun ()
  "Mark the current chapel function (or procedure).
This puts the mark at the end, and point at the beginning."
  (interactive)
  (push-mark (point))
  (chapel-end-of-defun)
  (push-mark (point))
  (chapel-beg-of-defun)
  (if (fboundp 'zmacs-activate-region)
      (zmacs-activate-region)))

(defun chapel-comment-area (start end)
  "Put the region into a Chapel comment.
The comments that are in this area are \"deformed\":
`*)' becomes `!(*' and `}' becomes `!{'.
These deformed comments are returned to normal if you use
\\[chapel-uncomment-area] to undo the commenting.

The commented area starts with `chapel-exclude-str-start', and ends with
`chapel-include-str-end'.  But if you change these variables,
\\[chapel-uncomment-area] won't recognize the comments."
  (interactive "r")
  (save-excursion
    ;; Insert start and endcomments
    (goto-char end)
    (if (and (save-excursion (skip-chars-forward " \t") (eolp))
	     (not (save-excursion (skip-chars-backward " \t") (bolp))))
	(forward-line 1)
      (beginning-of-line))
    (insert chapel-exclude-str-end)
    (setq end (point))
    (newline)
    (goto-char start)
    (beginning-of-line)
    (insert chapel-exclude-str-start)
    (newline)
    ;; Replace end-comments within commented area
    (goto-char end)
    (save-excursion
      (while (re-search-backward "\\*/" start t)
	(replace-match "!/*" t t)))))

(defun chapel-uncomment-area ()
  "Uncomment a commented area; change deformed comments back to normal.
This command does nothing if the pointer is not in a commented
area.  See also `chapel-comment-area'."
  (interactive)
  (save-excursion
    (let ((start (point))
	  (end (point)))
      ;; Find the boundaries of the comment
      (save-excursion
	(setq start (progn (search-backward chapel-exclude-str-start nil t)
			   (point)))
	(setq end (progn (search-forward chapel-exclude-str-end nil t)
			 (point))))
      ;; Check if we're really inside a comment
      (if (or (equal start (point)) (<= end (point)))
	  (message "Not standing within commented area.")
	(progn
	  ;; Remove endcomment
	  (goto-char end)
	  (beginning-of-line)
	  (let ((pos (point)))
	    (end-of-line)
	    (delete-region pos (1+ (point))))
	  ;; Change comments back to normal
	  (save-excursion
	    (while (re-search-backward "!/\\*" start t)
	      (replace-match "*/" t t)))
	  ;; Remove startcomment
	  (goto-char start)
	  (beginning-of-line)
	  (let ((pos (point)))
	    (end-of-line)
	    (delete-region pos (1+ (point)))))))))

(defun chapel-beg-of-defun ()
  "Move backward to the beginning of the current function or procedure."
  (interactive)
  (catch 'found
    (if (not (looking-at (concat "\\s \\|\\s)\\|" chapel-defun-re)))
	(forward-sexp 1))
    (let ((nest 0) (max -1) (func 0)
	  (reg (concat chapel-beg-block-re "\\|"
		       chapel-end-block-re "\\|"
		       chapel-defun-re)))
      (while (re-search-backward reg nil 'move)
	(cond ((let ((state (save-excursion
			      (parse-partial-sexp (point-min) (point)))))
		 (or (nth 3 state) (nth 4 state))) ; Inside string or comment
	       ())
	      ((match-end 1)                       ; begin|case|record|repeat
	       (if (and (looking-at "\\<record\\>") (>= max 0))
		   (setq func (1- func)))
	       (setq nest (1+ nest)
		     max (max nest max)))
	      ((match-end 2)                       ; end|until
	       (if (and (= nest max) (>= max 0))
		   (setq func (1+ func)))
	       (setq nest (1- nest)))
	      ((match-end 3)                       ; function|procedure
	       (if (= 0 func)
		   (throw 'found t)
		 (setq func (1- func)))))))
    nil))

(defun chapel-end-of-defun ()
  "Move forward to the end of the current function or procedure."
  (interactive)
  (if (looking-at "\\s ")
      (forward-sexp 1))
  (if (not (looking-at chapel-defun-re))
      (chapel-beg-of-defun))
  (forward-char 1)
  (let ((nest 0) (func 1)
	(reg (concat chapel-beg-block-re "\\|"
		     chapel-end-block-re "\\|"
		     chapel-defun-re)))
    (while (and (/= func 0)
		(re-search-forward reg nil 'move))
      (cond ((let ((state (save-excursion
			      (parse-partial-sexp (point-min) (point)))))
		 (or (nth 3 state) (nth 4 state))) ; Inside string or comment
	       ())
	    ((match-end 1)
	     (setq nest (1+ nest))
	     (if (save-excursion
		   (goto-char (match-beginning 0))
		   (looking-at "\\<record\\>"))
		 (setq func (1+ func))))
	    ((match-end 2)
	     (setq nest (1- nest))
	     (if (= nest 0)
		 (setq func (1- func))))
	    ((match-end 3)
	     (setq func (1+ func))))))
  (forward-line 1))

(defun chapel-end-of-statement ()
  "Move forward to end of current statement."
  (interactive)
  (let ((parse-sexp-ignore-comments t)
	(nest 0) pos
	(regexp (concat "\\(" chapel-beg-block-re "\\)\\|\\("
			chapel-end-block-re "\\)")))
    (if (not (looking-at "[ \t\n]")) (forward-sexp -1))
    (or (looking-at chapel-beg-block-re)
	;; Skip to end of statement
	(setq pos (catch 'found
		    (while t
		      (forward-sexp 1)
		      (cond ((looking-at "[ \t]*;")
			     (skip-chars-forward "^;")
			     (forward-char 1)
			     (throw 'found (point)))
			    ((save-excursion
			       (forward-sexp -1)
			       (looking-at chapel-beg-block-re))
			     (goto-char (match-beginning 0))
			     (throw 'found nil))
			    ((eobp)
			     (throw 'found (point))))))))
    (if (not pos)
	;; Skip a whole block
	(catch 'found
	  (while t
	    (re-search-forward regexp nil 'move)
	    (setq nest (if (match-end 1)
			   (1+ nest)
			 (1- nest)))
	    (cond ((eobp)
		   (throw 'found (point)))
		  ((= 0 nest)
		   (throw 'found (chapel-end-of-statement))))))
      pos)))

(defun chapel-downcase-keywords ()
  "Downcase all Chapel keywords in the buffer."
  (interactive)
  (chapel-change-keywords 'downcase-word))

(defun chapel-upcase-keywords ()
  "Upcase all Chapel keywords in the buffer."
  (interactive)
  (chapel-change-keywords 'upcase-word))

(defun chapel-capitalize-keywords ()
  "Capitalize all Chapel keywords in the buffer."
  (interactive)
  (chapel-change-keywords 'capitalize-word))

;; Change the keywords according to argument.
(defun chapel-change-keywords (change-word)
  (save-excursion
    (let ((keyword-re (concat "\\<\\("
			      (mapconcat 'identity chapel-keywords "\\|")
			      "\\)\\>")))
      (goto-char (point-min))
      (while (re-search-forward keyword-re nil t)
	(funcall change-word -1)))))



;;;
;;; Other functions
;;;
(defun chapel-set-auto-comments ()
  "Insert `{ case }' or `{ NAME }' on this line if appropriate.
Insert `{ case }' if there is an `end' on the line which
ends a case block.  Insert `{ NAME }' if there is an `end'
on the line which ends a function or procedure named NAME."
  (save-excursion
    (forward-line -1)
    (skip-chars-forward " \t")
    (if (and (looking-at "\\<end;")
	     (not (save-excursion
		    (end-of-line)
		    (search-backward "/*" (chapel-get-beg-of-line) t))))
	(let ((type (car (chapel-calculate-indent))))
	  (if (eq type 'declaration)
	      ()
	    (if (eq type 'case)
		;; This is a case block
		(progn
		  (end-of-line)
		  (delete-horizontal-space)
		  (insert " /* case */"))
	      (let ((nest 1))
		;; Check if this is the end of a function
		(save-excursion
		  (while (not (or (looking-at chapel-defun-re) (bobp)))
		    (backward-sexp 1)
		    (cond ((looking-at chapel-beg-block-re)
			   (setq nest (1- nest)))
			  ((looking-at chapel-end-block-re)
			   (setq nest (1+ nest)))))
		  (if (bobp)
		      (setq nest 1)))
		(if (zerop nest)
		    (progn
		      (end-of-line)
		      (delete-horizontal-space)
		      (insert " /* ")
		      (let (b e)
			(save-excursion
			  (setq b (progn (chapel-beg-of-defun)
					 (skip-chars-forward "^ \t")
					 (skip-chars-forward " \t")
					 (point))
				e (progn (skip-chars-forward "a-zA-Z0-9_")
					 (point))))
			(insert-buffer-substring (current-buffer) b e))
		      (insert " */"))))))))))



;;;
;;; Indentation
;;;
(defconst chapel-indent-alist
  '((block . (+ ind chapel-indent-level))
    (case . (+ ind chapel-case-indent))
    (caseblock . ind) (cpp . 0)
    (declaration . (+ ind chapel-indent-level))
    (paramlist . (chapel-indent-paramlist t))
    (comment . (chapel-indent-comment))
    (defun . ind) (contexp . ind)
    (unknown . ind) (string . 0) (progbeg . 0)))

(defun chapel-indent-command ()
  "Indent for special part of code."
  (let* ((indent-str (chapel-calculate-indent))
	 (type (car indent-str))
	 (ind (car (cdr indent-str))))
    (cond ((and (eq type 'paramlist)
		(or (memq 'all chapel-auto-lineup)
		    (memq 'paramlist chapel-auto-lineup)))
	   (chapel-indent-paramlist)
	   (chapel-indent-paramlist))
	  ((and (eq type 'declaration)
		(or (memq 'all chapel-auto-lineup)
		    (memq 'declaration  chapel-auto-lineup)))
	   (chapel-indent-declaration))
	  ((and (eq type 'case) (not (looking-at "^[ \t]*$"))
		(or (memq 'all chapel-auto-lineup)
		    (memq 'case chapel-auto-lineup)))
	   (chapel-indent-case)))
    (if (looking-at "[ \t]+$")
	(skip-chars-forward " \t"))))

(defun chapel-indent-line ()
  "Indent current line as a Chapel statement."
  (let* ((indent-str (chapel-calculate-indent))
	 (type (car indent-str))
	 (ind (car (cdr indent-str))))
    ;; Labels should not be indented.
    (if (and (looking-at "^[0-9a-zA-Z]+[ \t]*:[^=]")
	     (not (eq type 'declaration)))
	(search-forward ":" nil t))
    (delete-horizontal-space)
    (cond (; Some things should not be indented
	   (or (and (eq type 'declaration) (looking-at chapel-declaration-re))
	       (eq type 'cpp))
	   ())
	  (; Other things should have no extra indent
	   (looking-at chapel-noindent-re)
	   (indent-to ind))
	  (; Nested functions should be indented
	   (looking-at chapel-defun-re)
	   (if (and chapel-indent-nested-functions
		    (eq type 'defun))
	       (indent-to (+ ind chapel-indent-level))
	     (indent-to ind)))
	  (; But most lines are treated this way
	   (indent-to (eval (cdr (assoc type chapel-indent-alist))))
	   ))))

(defun chapel-calculate-indent ()
  "Calculate the indent of the current Chapel line.
Return a list of two elements: (INDENT-TYPE INDENT-LEVEL)."
  (save-excursion
    (let* ((parse-sexp-ignore-comments t)
	   (oldpos (point))
	   (state (save-excursion (parse-partial-sexp (point-min) (point))))
	   (nest 0) (par 0) (complete (looking-at "[ \t]*end\\>"))
	   (elsed (looking-at "[ \t]*else\\>")) (funccnt 0)
	   (did-func (looking-at "[ \t]*\\(def\\|function\\)\\>"))
	   (type (catch 'nesting
		   ;; Check if inside a string, comment or parenthesis
		   (cond ((nth 3 state) (throw 'nesting 'string))
			 ((nth 4 state) (throw 'nesting 'comment))
			 ((> (car state) 0)
			  (goto-char (scan-lists (point) -1 (car state)))
			  (setq par (1+ (current-column))))
			 ((save-excursion (beginning-of-line)
					  (eq (following-char) ?#))
			  (throw 'nesting 'cpp)))
		   ;; Loop until correct indent is found
		   (while t
		     (backward-sexp 1)
		     (cond (;--Escape from case statements
			    (and (looking-at "[A-Za-z0-9]+[ \t]*:[^=]")
				 (not complete)
				 (save-excursion (skip-chars-backward " \t")
						 (bolp))
				 (= (save-excursion
				      (end-of-line) (backward-sexp) (point))
				    (point))
				 (> (save-excursion (goto-char oldpos)
						    (beginning-of-line)
						    (point))
				    (point)))
			    (throw 'nesting 'caseblock))
			   (;--Beginning of program
			    (looking-at chapel-progbeg-re)
			    (throw 'nesting 'progbeg))
			   (;--No known statements
			    (bobp)
			    (throw 'nesting 'progbeg))
			   (;--Nest block outwards
			    (looking-at chapel-beg-block-re)
			    (if (= nest 0)
				(cond ((looking-at "case\\>")
				       (throw 'nesting 'case))
				      ((looking-at "record\\>")
				       (throw 'nesting 'declaration))
				      (t (throw 'nesting 'block)))
			      (if (and (looking-at "record\\>") (= nest 1))
				  (setq funccnt (1- funccnt)))
			      (setq nest (1- nest))))
			   (;--Nest block inwards
			    (looking-at chapel-end-block-re)
			    (if (and (looking-at "end\\s ")
				     elsed (not complete))
				(throw 'nesting 'block))
			    (if (= nest 0)
				(setq funccnt (1+ funccnt)))
			    (setq complete t
				  nest (1+ nest)))
			   (;--Defun (or parameter list)
			    (and (looking-at chapel-defun-re)
				 (progn (setq funccnt (1- funccnt)
					      did-func t)
					(or (bolp) (< funccnt 0))))
			    ;; Prevent searching whole buffer
			    (if (and (bolp) (>= funccnt 0))
				(throw 'nesting 'progbeg))
			    (if (= 0 par)
				(throw 'nesting 'defun)
			      (setq par 0)
			      (let ((n 0))
				(while (re-search-forward
					"\\(\\<record\\>\\)\\|\\<end\\>"
					oldpos t)
				  (if (match-end 1)
				      (setq n (1+ n)) (setq n (1- n))))
				(if (> n 0)
				    (throw 'nesting 'declaration)
				  (throw 'nesting 'paramlist)))))
			   (;--Declaration part
			    (and (looking-at chapel-declaration-re)
				 (not did-func)
				 (= funccnt 0))
			    (if (save-excursion
				  (goto-char oldpos)
				  (forward-line -1)
				  (looking-at "^[ \t]*$"))
				(throw 'nesting 'unknown)
			      (throw 'nesting 'declaration)))
			   (;--If, else or while statement
			    (and (not complete)
				 (looking-at chapel-sub-block-re))
			    (throw 'nesting 'block))
			   (;--Found complete statement
			    (save-excursion (forward-sexp 1)
					    (= (following-char) ?\;))
			    (setq complete t))
			   )))))

      ;; Return type of block and indent level.
      (if (> par 0)                               ; Unclosed Parenthesis
	  (list 'contexp par)
	(list type (chapel-indent-level))))))

(defun chapel-indent-level ()
  "Return the indent-level the current statement has.
Do not count labels, case-statements or records."
  (save-excursion
    (beginning-of-line)
    (if (looking-at "[ \t]*[0-9a-zA-Z]+[ \t]*:[^=]")
	(search-forward ":" nil t)
      (if (looking-at ".*=[ \t]*record\\>")
	  (search-forward "=" nil t)))
    (skip-chars-forward " \t")
    (current-column)))

(defun chapel-indent-comment ()
  "Return indent for current comment."
  (save-excursion
    (re-search-backward "\\(/\\*\\)\\" nil t)
    (if (match-beginning 1)
	(1+ (current-column))
      (current-column))))

(defun chapel-indent-case ()
  "Indent within case statements."
  (let ((savepos (point-marker))
	(end (prog2
		 (end-of-line)
		 (point-marker)
	       (re-search-backward "\\<case\\>" nil t)))
	(beg (point)) oldpos
	(ind 0))
    ;; Get right indent
    (while (< (point) end)
      (if (re-search-forward
	   "^[ \t]*[^ \t,:]+[ \t]*\\(,[ \t]*[^ \t,:]+[ \t]*\\)*:"
	   (marker-position end) 'move)
	  (forward-char -1))
      (if (< (point) end)
	  (progn
	    (delete-horizontal-space)
	    (if (> (current-column) ind)
		(setq ind (current-column)))
	    (chapel-end-of-statement))))
    (goto-char beg)
    (setq oldpos (marker-position end))
    ;; Indent all case statements
    (while (< (point) end)
      (if (re-search-forward
	   "^[ \t]*[^][ \t,\\.:]+[ \t]*\\(,[ \t]*[^ \t,:]+[ \t]*\\)*:"
	   (marker-position end) 'move)
	  (forward-char -1))
      (indent-to (1+ ind))
      (if (/= (following-char) ?:)
	  ()
	(forward-char 1)
	(delete-horizontal-space)
	(insert " "))
      (setq oldpos (point))
      (chapel-end-of-statement))
    (goto-char savepos)))

(defun chapel-indent-paramlist (&optional arg)
  "Indent current line in parameterlist.
If optional arg is non-nil, just return the
indent of the current line in parameterlist."
  (save-excursion
    (let* ((oldpos (point))
	   (stpos (progn (goto-char (scan-lists (point) -1 1)) (point)))
	   (stcol (1+ (current-column)))
	   (edpos (progn (chapel-declaration-end)
			 (search-backward ")" (chapel-get-beg-of-line) t)
			 (point)))
	   (usevar (re-search-backward "\\<var\\>" stpos t)))
      (if arg (progn
		;; If arg, just return indent
		(goto-char oldpos)
		(beginning-of-line)
		(if (or (not usevar) (looking-at "[ \t]*var\\>"))
		    stcol (+ 4 stcol)))
	(goto-char stpos)
	(forward-char 1)
	(delete-horizontal-space)
	(if (and usevar (not (looking-at "var\\>")))
	    (indent-to (+ 4 stcol)))
	(chapel-indent-declaration nil stpos edpos)))))

(defun chapel-indent-declaration (&optional arg start end)
  "Indent current lines as declaration, lining up the `:'s or `='s."
  (let ((pos (point-marker)))
    (if (and (not (or arg start)) (not (chapel-declaration-beg)))
	()
      (let ((lineup (if (or (looking-at "\\<var\\>\\|\\<record\\>") arg start)
			":" "="))
	    (stpos (if start start
		       (forward-word 2) (backward-word 1) (point)))
	    (edpos (set-marker (make-marker)
			       (if end end
				 (max (progn (chapel-declaration-end)
					     (point))
				      pos))))
	    ind)

	(goto-char stpos)
	;; Indent lines in record block
	(if arg
	    (while (<= (point) edpos)
	      (beginning-of-line)
	      (delete-horizontal-space)
	      (if (looking-at "end\\>")
		  (indent-to arg)
		(indent-to (+ arg chapel-indent-level)))
	      (forward-line 1)))

	;; Do lineup
	(setq ind (chapel-get-lineup-indent stpos edpos lineup))
	(goto-char stpos)
	(while (and (<= (point) edpos) (not (eobp)))
	  (if (search-forward lineup (chapel-get-end-of-line) 'move)
	      (forward-char -1))
	  (delete-horizontal-space)
	  (indent-to ind)
	  (if (not (looking-at lineup))
	      (forward-line 1) ; No more indent if there is no : or =
	    (forward-char 1)
	    (delete-horizontal-space)
	    (insert " ")
	    ;; Indent record block
	    (if (looking-at "record\\>")
		(chapel-indent-declaration (current-column)))
	    (forward-line 1)))))

    ;; If arg - move point
    (if arg (forward-line -1)
      (goto-char pos))))

;  "Return the indent level that will line up several lines within the region
;from b to e nicely. The lineup string is str."
(defun chapel-get-lineup-indent (b e str)
  (save-excursion
    (let ((ind 0)
	  (reg (concat str "\\|\\(\\<record\\>\\)\\|" chapel-defun-re)))
      (goto-char b)
      ;; Get rightmost position
      (while (< (point) e)
	(and (re-search-forward reg (min e (chapel-get-end-of-line 2)) 'move)
	     (cond ((match-beginning 1)
		    ;; Skip record blocks
		    (chapel-declaration-end))
		   ((match-beginning 2)
		    ;; We have entered a new procedure.  Exit.
		    (goto-char e))
		   (t
		    (goto-char (match-beginning 0))
		    (skip-chars-backward " \t")
		    (if (> (current-column) ind)
			(setq ind (current-column)))
		    (goto-char (match-end 0))
		    (end-of-line)
		    ))))
      ;; In case no lineup was found
      (if (> ind 0)
	  (1+ ind)
	;; No lineup-string found
	(goto-char b)
	(end-of-line)
	(skip-chars-backward " \t")
	(1+ (current-column))))))



;;;
;;; Completion
;;;
(defvar chapel-str nil)
(defvar chapel-all nil)
(defvar chapel-pred nil)
(defvar chapel-buffer-to-use nil)
(defvar chapel-flag nil)

(defun chapel-string-diff (str1 str2)
  "Return index of first letter where STR1 and STR2 differs."
  (catch 'done
    (let ((diff 0))
      (while t
	(if (or (> (1+ diff) (length str1))
		(> (1+ diff) (length str2)))
	    (throw 'done diff))
	(or (equal (aref str1 diff) (aref str2 diff))
	    (throw 'done diff))
	(setq diff (1+ diff))))))

;; Calculate all possible completions for functions if argument is `function',
;; completions for procedures if argument is `procedure' or both functions and
;; procedures otherwise.

(defun chapel-func-completion (type)
  ;; Build regular expression for function/procedure names
  (if (string= chapel-str "")
      (setq chapel-str "[a-zA-Z_]"))
  (let ((chapel-str (concat (cond
			     ((eq type 'def) "\\<\\(def\\)\\s +")
			     ((eq type 'function) "\\<\\(function\\)\\s +")
			     (t "\\<\\(function\\|def\\)\\s +"))
			    "\\<\\(" chapel-str "[a-zA-Z0-9_.]*\\)\\>"))
	match)

    (if (not (looking-at "\\<\\(function\\|def\\)\\>"))
	(re-search-backward "\\<\\(function\\|def\\)\\>" nil t))
    (forward-char 1)

    ;; Search through all reachable functions
    (while (chapel-beg-of-defun)
      (if (re-search-forward chapel-str (chapel-get-end-of-line) t)
	  (progn (setq match (buffer-substring (match-beginning 2)
					       (match-end 2)))
		 (if (or (null chapel-pred)
			 (funcall chapel-pred match))
		     (setq chapel-all (cons match chapel-all)))))
      (goto-char (match-beginning 0)))))

(defun chapel-get-completion-decl ()
  ;; Macro for searching through current declaration (var, type or const)
  ;; for matches of `str' and adding the occurrence to `all'
  (let ((end (save-excursion (chapel-declaration-end)
			     (point)))
	match)
    ;; Traverse lines
    (while (< (point) end)
      (if (re-search-forward "[:=]" (chapel-get-end-of-line) t)
	  ;; Traverse current line
	  (while (and (re-search-backward
		       (concat "\\((\\|\\<\\(var\\|type\\|const\\)\\>\\)\\|"
			       chapel-symbol-re)
		       (chapel-get-beg-of-line) t)
		      (not (match-end 1)))
	    (setq match (buffer-substring (match-beginning 0) (match-end 0)))
	    (if (string-match (concat "\\<" chapel-str) match)
		(if (or (null chapel-pred)
			(funcall chapel-pred match))
		    (setq chapel-all (cons match chapel-all))))))
      (if (re-search-forward "\\<record\\>" (chapel-get-end-of-line) t)
	  (chapel-declaration-end)
	(forward-line 1)))))

(defun chapel-type-completion ()
  "Calculate all possible completions for types."
  (let ((start (point))
	goon)
    ;; Search for all reachable type declarations
    (while (or (chapel-beg-of-defun)
	       (setq goon (not goon)))
      (save-excursion
	(if (and (< start (prog1 (save-excursion (chapel-end-of-defun)
						 (point))
			    (forward-char 1)))
		 (re-search-forward
		  "\\<type\\>\\|\\<\\(function\\|def\\|{\\)\\>"
		  start t)
		 (not (match-end 1)))
	    ;; Check current type declaration
	    (chapel-get-completion-decl))))))

(defun chapel-var-completion ()
  "Calculate all possible completions for variables (or constants)."
  (let ((start (point))
	goon twice)
    ;; Search for all reachable var declarations
    (while (or (chapel-beg-of-defun)
	       (setq goon (not goon)))
      (save-excursion
	(if (> start (prog1 (save-excursion (chapel-end-of-defun)
					    (point))))
	    () ; Declarations not reachable
	  (if (search-forward "(" (chapel-get-end-of-line) t)
	      ;; Check parameterlist
		(chapel-get-completion-decl))
	  (setq twice 2)
	  (while (>= (setq twice (1- twice)) 0)
	    (cond ((and (re-search-forward
			 (concat "\\<\\(var\\|const\\)\\>\\|"
				 "\\<\\(function\\|def\\|{\\)\\>")
			 start t)
			(not (match-end 2)))
		   ;; Check var/const declarations
		   (chapel-get-completion-decl))
		  ((match-end 2)
		   (setq twice 0)))))))))


(defun chapel-keyword-completion (keyword-list)
  "Give list of all possible completions of keywords in KEYWORD-LIST."
  (mapcar '(lambda (s)
	     (if (string-match (concat "\\<" chapel-str) s)
		 (if (or (null chapel-pred)
			 (funcall chapel-pred s))
		     (setq chapel-all (cons s chapel-all)))))
	  keyword-list))

;; Function passed to completing-read, try-completion or
;; all-completions to get completion on STR. If predicate is non-nil,
;; it must be a function to be called for every match to check if this
;; should really be a match. If flag is t, the function returns a list
;; of all possible completions. If it is nil it returns a string, the
;; longest possible completion, or t if STR is an exact match. If flag
;; is 'lambda, the function returns t if STR is an exact match, nil
;; otherwise.

(defun chapel-completion (chapel-str chapel-pred chapel-flag)
  (save-excursion
    (let ((chapel-all nil))
      ;; Set buffer to use for searching labels. This should be set
      ;; within functions which use chapel-completions
      (set-buffer chapel-buffer-to-use)

      ;; Determine what should be completed
      (let ((state (car (chapel-calculate-indent))))
	(cond (;--Within a declaration or parameterlist
	       (or (eq state 'declaration) (eq state 'paramlist)
		   (and (eq state 'defun)
			(save-excursion
			  (re-search-backward ")[ \t]*:"
					      (chapel-get-beg-of-line) t))))
	       (if (or (eq state 'paramlist) (eq state 'defun))
		   (chapel-beg-of-defun))
	       (chapel-type-completion)
	       (chapel-keyword-completion chapel-type-keywords))
	      (;--Starting a new statement
	       (and (not (eq state 'contexp))
		    (save-excursion
		      (skip-chars-backward "a-zA-Z0-9_.")
		      (backward-sexp 1)
		      (or (looking-at chapel-nosemi-re)
			  (progn
			    (forward-sexp 1)
			    (looking-at "\\s *\\(;\\|:[^=]\\)")))))
	       (save-excursion (chapel-var-completion))
	       (chapel-func-completion 'def)
	       (chapel-keyword-completion chapel-start-keywords))
	      (t;--Anywhere else
	       (save-excursion (chapel-var-completion))
	       (chapel-func-completion 'function)
	       (chapel-keyword-completion chapel-separator-keywords))))

      ;; Now we have built a list of all matches. Give response to caller
      (chapel-completion-response))))

(defun chapel-completion-response ()
  (cond ((or (equal chapel-flag 'lambda) (null chapel-flag))
	 ;; This was not called by all-completions
	 (if (null chapel-all)
	     ;; Return nil if there was no matching label
	     nil
	   ;; Get longest string common in the labels
	   (let* ((elm (cdr chapel-all))
		  (match (car chapel-all))
		  (min (length match))
		  tmp)
	     (if (string= match chapel-str)
		 ;; Return t if first match was an exact match
		 (setq match t)
	       (while (not (null elm))
		 ;; Find longest common string
		 (if (< (setq tmp (chapel-string-diff match (car elm))) min)
		     (progn
		       (setq min tmp)
		       (setq match (substring match 0 min))))
		 ;; Terminate with match=t if this is an exact match
		 (if (string= (car elm) chapel-str)
		     (progn
		       (setq match t)
		       (setq elm nil))
		   (setq elm (cdr elm)))))
	     ;; If this is a test just for exact match, return nil ot t
	     (if (and (equal chapel-flag 'lambda) (not (equal match 't)))
		 nil
	       match))))
	;; If flag is t, this was called by all-completions. Return
	;; list of all possible completions
	(chapel-flag
	 chapel-all)))

(defvar chapel-last-word-numb 0)
(defvar chapel-last-word-shown nil)
(defvar chapel-last-completions nil)

(defun chapel-complete-word ()
  "Complete word at current point.
\(See also `chapel-toggle-completions', `chapel-type-keywords',
`chapel-start-keywords' and `chapel-separator-keywords'.)"
  (interactive)
  (let* ((b (save-excursion (skip-chars-backward "a-zA-Z0-9_") (point)))
	 (e (save-excursion (skip-chars-forward "a-zA-Z0-9_") (point)))
	 (chapel-str (buffer-substring b e))
	 ;; The following variable is used in chapel-completion
	 (chapel-buffer-to-use (current-buffer))
	 (allcomp (if (and chapel-toggle-completions
			   (string= chapel-last-word-shown chapel-str))
		      chapel-last-completions
		    (all-completions chapel-str 'chapel-completion)))
	 (match (if chapel-toggle-completions
		    "" (try-completion
			chapel-str (mapcar '(lambda (elm)
					      (cons elm 0)) allcomp)))))
    ;; Delete old string
    (delete-region b e)

    ;; Toggle-completions inserts whole labels
    (if chapel-toggle-completions
	(progn
	  ;; Update entry number in list
	  (setq chapel-last-completions allcomp
		chapel-last-word-numb
		(if (>= chapel-last-word-numb (1- (length allcomp)))
		    0
		  (1+ chapel-last-word-numb)))
	  (setq chapel-last-word-shown (elt allcomp chapel-last-word-numb))
	  ;; Display next match or same string if no match was found
	  (if (not (null allcomp))
	      (insert "" chapel-last-word-shown)
	    (insert "" chapel-str)
	    (message "(No match)")))
      ;; The other form of completion does not necessarily do that.

      ;; Insert match if found, or the original string if no match
      (if (or (null match) (equal match 't))
	  (progn (insert "" chapel-str)
		 (message "(No match)"))
	(insert "" match))
      ;; Give message about current status of completion
      (cond ((equal match 't)
	     (if (not (null (cdr allcomp)))
		 (message "(Complete but not unique)")
	       (message "(Sole completion)")))
	    ;; Display buffer if the current completion didn't help
	    ;; on completing the label.
	    ((and (not (null (cdr allcomp))) (= (length chapel-str)
						(length match)))
	     (with-output-to-temp-buffer "*Completions*"
	       (display-completion-list allcomp))
	     ;; Wait for a keypress. Then delete *Completion*  window
	     (momentary-string-display "" (point))
	     (delete-window (get-buffer-window (get-buffer "*Completions*")))
	     )))))

(defun chapel-show-completions ()
  "Show all possible completions at current point."
  (interactive)
  (let* ((b (save-excursion (skip-chars-backward "a-zA-Z0-9_") (point)))
	 (e (save-excursion (skip-chars-forward "a-zA-Z0-9_") (point)))
	 (chapel-str (buffer-substring b e))
	 ;; The following variable is used in chapel-completion
	 (chapel-buffer-to-use (current-buffer))
	 (allcomp (if (and chapel-toggle-completions
			   (string= chapel-last-word-shown chapel-str))
		      chapel-last-completions
		    (all-completions chapel-str 'chapel-completion))))
    ;; Show possible completions in a temporary buffer.
    (with-output-to-temp-buffer "*Completions*"
      (display-completion-list allcomp))
    ;; Wait for a keypress. Then delete *Completion*  window
    (momentary-string-display "" (point))
    (delete-window (get-buffer-window (get-buffer "*Completions*")))))


(defun chapel-get-default-symbol ()
  "Return symbol around current point as a string."
  (save-excursion
    (buffer-substring (progn
			(skip-chars-backward " \t")
			(skip-chars-backward "a-zA-Z0-9_")
			(point))
		      (progn
			(skip-chars-forward "a-zA-Z0-9_")
			(point)))))

(defun chapel-build-defun-re (str &optional arg)
  "Return function/procedure starting with STR as regular expression.
With optional second arg non-nil, STR is the complete name of the instruction."
  (if arg
      (concat "^\\(function\\|def\\)[ \t]+\\(" str "\\)\\>")
    (concat "^\\(function\\|def\\)[ \t]+\\(" str "[a-zA-Z0-9_]*\\)\\>")))

;; Function passed to completing-read, try-completion or
;; all-completions to get completion on any function name. If
;; predicate is non-nil, it must be a function to be called for every
;; match to check if this should really be a match. If flag is t, the
;; function returns a list of all possible completions. If it is nil
;; it returns a string, the longest possible completion, or t if STR
;; is an exact match. If flag is 'lambda, the function returns t if
;; STR is an exact match, nil otherwise.

(defun chapel-comp-defun (chapel-str chapel-pred chapel-flag)
  (save-excursion
    (let ((chapel-all nil)
	  match)

      ;; Set buffer to use for searching labels. This should be set
      ;; within functions which use chapel-completions
      (set-buffer chapel-buffer-to-use)

      (let ((chapel-str chapel-str))
	;; Build regular expression for functions
	(if (string= chapel-str "")
	    (setq chapel-str (chapel-build-defun-re "[a-zA-Z_]"))
	  (setq chapel-str (chapel-build-defun-re chapel-str)))
	(goto-char (point-min))

	;; Build a list of all possible completions
	(while (re-search-forward chapel-str nil t)
	  (setq match (buffer-substring (match-beginning 2) (match-end 2)))
	  (if (or (null chapel-pred)
		  (funcall chapel-pred match))
	      (setq chapel-all (cons match chapel-all)))))

      ;; Now we have built a list of all matches. Give response to caller
      (chapel-completion-response))))

(defun chapel-goto-defun ()
  "Move to specified Chapel function/procedure.
The default is a name found in the buffer around point."
  (interactive)
  (let* ((default (chapel-get-default-symbol))
	 ;; The following variable is used in chapel-comp-function
	 (chapel-buffer-to-use (current-buffer))
	 (default (if (chapel-comp-defun default nil 'lambda)
		      default ""))
	 (label (if (not (string= default ""))
		    ;; Do completion with default
		    (completing-read (concat "Label: (default " default ") ")
				     'chapel-comp-defun nil t "")
		  ;; There is no default value. Complete without it
		  (completing-read "Label: "
				   'chapel-comp-defun nil t ""))))
    ;; If there was no response on prompt, use default value
    (if (string= label "")
	(setq label default))
    ;; Goto right place in buffer if label is not an empty string
    (or (string= label "")
	(progn
	  (goto-char (point-min))
	  (re-search-forward (chapel-build-defun-re label t))
	  (beginning-of-line)))))



;;;
;;; Chapel-outline-mode
;;;
(defvar chapel-outline-map nil "Keymap used in Chapel Outline mode.")

(if chapel-outline-map
    nil
  (if (boundp 'set-keymap-name)
      (set-keymap-name chapel-outline-map 'chapel-outline-map))
  (if (not (boundp 'set-keymap-parent))
      (setq chapel-outline-map (copy-keymap chapel-mode-map))
    (setq chapel-outline-map (make-sparse-keymap))
    (set-keymap-parent chapel-outline-map chapel-mode-map))
  (define-key chapel-outline-map "\M-\C-a"  'chapel-outline-prev-defun)
  (define-key chapel-outline-map "\M-\C-e"  'chapel-outline-next-defun)
  (define-key chapel-outline-map "\C-c\C-d" 'chapel-outline-goto-defun)
  (define-key chapel-outline-map "\C-c\C-s" 'chapel-show-all)
  (define-key chapel-outline-map "\C-c\C-h" 'chapel-hide-other-defuns))

(defvar chapel-outline-mode nil "Non-nil while using Chapel Outline mode.")
(make-variable-buffer-local 'chapel-outline-mode)
(set-default 'chapel-outline-mode nil)
(if (not (assoc 'chapel-outline-mode minor-mode-alist))
    (setq minor-mode-alist (append minor-mode-alist
				   (list '(chapel-outline-mode " Outl")))))

(defun chapel-outline (&optional arg)
  "Outline-line minor mode for Chapel mode.
When in Chapel Outline mode, portions
of the text being edited may be made invisible. \\<chapel-outline-map>

Chapel Outline mode provides some additional commands.

\\[chapel-outline-prev-defun]\
\t- Move to previous function/procedure, hiding everything else.
\\[chapel-outline-next-defun]\
\t- Move to next function/procedure, hiding everything else.
\\[chapel-outline-goto-defun]\
\t- Goto function/procedure prompted for in minibuffer,
\t  hide all other functions.
\\[chapel-show-all]\t- Show the whole buffer.
\\[chapel-hide-other-defuns]\
\t- Hide everything but the current function (function under the cursor).
\\[chapel-outline]\t- Leave chapel-outline-mode."
  (interactive "P")
  (setq chapel-outline-mode
	(if (null arg) (not chapel-outline-mode) t))
  (if (boundp 'redraw-mode-line)
      (redraw-mode-line))
  (if chapel-outline-mode
      (progn
	(setq selective-display t)
	(use-local-map chapel-outline-map))
    (progn
      (setq selective-display nil)
      (chapel-show-all)
      (use-local-map chapel-mode-map))))

(defun chapel-outline-change (b e chapel-flag)
  (let ((modp (buffer-modified-p)))
    (unwind-protect
	(subst-char-in-region b e (if (= chapel-flag ?\n)
				      ?\^M ?\n) chapel-flag)
      (set-buffer-modified-p modp))))

(defun chapel-show-all ()
  "Show all of the text in the buffer."
  (interactive)
  (chapel-outline-change (point-min) (point-max) ?\n))

(defun chapel-hide-other-defuns ()
  "Show only the current defun."
  (interactive)
  (save-excursion
    (let ((beg (progn (if (not (looking-at "\\(function\\|def\\)\\>"))
			  (chapel-beg-of-defun))
		      (point)))
	  (end (progn (chapel-end-of-defun)
		      (backward-sexp 1)
		      (search-forward "\n\\|\^M" nil t)
		      (point)))
	  (opoint (point-min)))
      (goto-char (point-min))

      ;; Hide all functions before current function
      (while (re-search-forward "^\\(function\\|def\\)\\>" beg 'move)
	(chapel-outline-change opoint (1- (match-beginning 0)) ?\^M)
	(setq opoint (point))
	;; Functions may be nested
	(if (> (progn (chapel-end-of-defun) (point)) beg)
	    (goto-char opoint)))
      (if (> beg opoint)
	  (chapel-outline-change opoint (1- beg) ?\^M))

      ;; Show current function
      (chapel-outline-change beg end ?\n)
      ;; Hide nested functions
      (forward-char 1)
      (while (re-search-forward "^\\(function\\|def\\)\\>" end 'move)
	(setq opoint (point))
	(chapel-end-of-defun)
	(chapel-outline-change opoint (point) ?\^M))

      (goto-char end)
      (setq opoint end)

      ;; Hide all function after current function
      (while (re-search-forward "^\\(function\\|def\\)\\>" nil 'move)
	(chapel-outline-change opoint (1- (match-beginning 0)) ?\^M)
	(setq opoint (point))
	(chapel-end-of-defun))
      (chapel-outline-change opoint (point-max) ?\^M)

      ;; Hide main program
      (if (< (progn (forward-line -1) (point)) end)
	  (progn
	    (goto-char beg)
	    (chapel-end-of-defun)
	    (backward-sexp 1)
	    (chapel-outline-change (point) (point-max) ?\^M))))))

(defun chapel-outline-next-defun ()
  "Move to next function/procedure, hiding all others."
  (interactive)
  (chapel-end-of-defun)
  (chapel-hide-other-defuns))

(defun chapel-outline-prev-defun ()
  "Move to previous function/procedure, hiding all others."
  (interactive)
  (chapel-beg-of-defun)
  (chapel-hide-other-defuns))

(defun chapel-outline-goto-defun ()
  "Move to specified function/procedure, hiding all others."
  (interactive)
  (chapel-goto-defun)
  (chapel-hide-other-defuns))

(provide 'chapel)

;;; arch-tag: 04535136-fd93-40b4-a505-c9bebdc051f5
;;; chapel.el ends here
