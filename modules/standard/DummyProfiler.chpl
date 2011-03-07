
use Profiling;

proc chpl_region_begin(param id : uint, param name : string, const file : string, const line : int) : int {
  writeln("profiling begin region #", id, " name=", name, " userfile=", file, " userline=", line);
  return 0;
}

proc chpl_region_end(param id : uint, param name : string, const file : string, const line : int) : int {
  writeln("profiling end region #", id, " name=", name, " userfile=", file, " userline=", line);
  return 0;
}

proc chpl_profiling_enable(const state : bool) : int {
  writeln("profiling enabled = ", state);
  return 0;
}

proc chpl_profiling_event(const name : string) : int {
  writeln("profiling trace event ", name);
  return 0;
}

proc chpl_profiling_value(const name : string, const val) : int {
  writeln("profiling trace event ", name, " with value ", val );
  return 0;
}

proc chpl_profiling_values(const name : string, const vals...?numvals) : int {
  writeln("profiling trace event ", name, " with values ", (...vals) );
  return 0;
}
