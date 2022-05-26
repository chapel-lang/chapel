use Sys;

config const doit = false;

var x: fd_set;
if doit then
  sys_fd_clr(1, x);

