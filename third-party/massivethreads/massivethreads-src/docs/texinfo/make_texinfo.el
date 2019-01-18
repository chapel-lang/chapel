
(defun make-texinfo (src dst)
  (find-file src)
  (mark-whole-buffer)
  (texinfo-insert-node-lines (region-beginning) (region-end) t)
  (texinfo-every-node-update)
  (texinfo-all-menus-update)
  (texinfo-master-menu t)
  (write-file dst))

(make-texinfo "massivethreads_src.tex" "massivethreads.tex")
