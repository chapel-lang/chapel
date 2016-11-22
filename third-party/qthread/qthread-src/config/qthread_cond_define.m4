AC_DEFUN([QT_COND_DEFINE],
         [AS_IF([test "x$1" = "xyes"],
                [AC_DEFINE([$2], [1], [$3])])])
