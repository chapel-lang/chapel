
use Profiling;

extern proc PAT_region_begin(id : int, lbl : string) : int;
extern proc PAT_region_end(id : int) : int;

extern proc PAT_record(state : bool) : int;

extern proc PAT_trace_user(name : string) : int;

extern proc PAT_trace_user_l(name : string, enable : bool, vals...?numvals) : int;

proc chpl_region_begin(param id : uint, param name : string, const file : string, const line : int) : int{
  const buf : string = name + " (" + file + ":" + line + ")";
  return PAT_region_begin(id:int, buf);
}

/* // alternate version that uses only name parameter */
/* proc chpl_region_begin(param id : uint, param name : string, const file : string, const line : int) : int { */
/*   return PAT_region_begin(id:int, name); */
/* } */

proc chpl_region_end(param id : uint, param name : string, const file : string, const line : int) : int{
  return PAT_region_end(id:int);
}

proc chpl_profiling_enable(const state : bool) : int {
  return PAT_record(state);
}

proc chpl_profiling_event(const name : string) : int {
  return PAT_trace_user(name);
}

proc chpl_profiling_value(const name : string, const val) : int {
  return PAT_trace_user_l(name, true, val);
}

proc chpl_profiling_values(const name : string, const vals...?numvals) : int {
  PAT_trace_user_l(name, true, (...vals));
  return 0;
}
