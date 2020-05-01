pragma "function terminates program"
inline proc exit(status: int=0) {
  __primitive("chpl_exit_any", status);
}
