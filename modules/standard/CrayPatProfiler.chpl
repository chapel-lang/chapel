
use Profiling;

_extern def PAT_region_begin(id : int, lbl : string) : int;
_extern def PAT_region_end(id : int) : int;

_extern def PAT_record(state : bool) : int;

_extern def PAT_trace_user(name : string) : int;

_extern def PAT_trace_user_l(name : string, enable : bool, vals...?numvals) : int;

def chpl_region_begin(param id : uint, param name : string, const file : string, const line : int) : int{
  const buf : string = name + " (" + file + ":" + line + ")";
  return PAT_region_begin(id:int, buf);
}

/* // alternate version that uses only name parameter */
/* def chpl_region_begin(param id : uint, param name : string, const file : string, const line : int) : int { */
/*   return PAT_region_begin(id:int, name); */
/* } */

def chpl_region_end(param id : uint, param name : string, const file : string, const line : int) : int{
  return PAT_region_end(id:int);
}

def chpl_profiling_enable(const state : bool) : int {
  return PAT_record(state);
}

def chpl_profiling_event(const name : string) : int {
  return PAT_trace_user(name);
}

def chpl_profiling_value(const name : string, const val) : int {
  return PAT_trace_user_l(name, true, val);
}

def chpl_profiling_values(const name : string, const vals...?numvals) : int {
  PAT_trace_user_l(name, true, (...vals));
  return 0;
}
