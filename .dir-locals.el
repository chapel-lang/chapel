;; This causes emacs to abide by formatting rules: No tab characters, except
;; in Makefiles; no whitespace at the end of lines; a newline at end of file.

(
 (nil . (
  (require-final-newline . t)
  (indent-tabs-mode . nil)
  (eval add-hook 'before-save-hook 'delete-trailing-whitespace)
  (eval add-hook 'before-save-hook (lambda () (untabify (point-min) (point-max))))
 ) )
 (makefile-mode . (
  (indent-tabs-mode . t)
  (eval remove-hook `before-save-hook (lambda () (untabify (point-min) (point-max))))
 ) )
)
