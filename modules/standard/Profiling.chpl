
record Profiler {
  param name : string = "";
    
  def enable(const state : bool = true) : int {
    return chpl_profiling_enable(state);
  }
  
  def event(const str : string) : int {
    return chpl_profiling_event(name+str);
  }
  
  def value(const str : string, const val) : int {
    return chpl_profiling_value(name+str, val);
  }

  def values(const str : string, const vals...?numvals) : int {
    return chpl_profiling_values(name+str, (...vals));
  }
}

const Profilers : [LocaleSpace] Profiler;


type RegionID = __primitive("_next_uint32");

record Region : Profiler{
  param id : uint;
  param name : string = "";

  def initialize() {
    const file : string = __primitive("_get_user_file");
    const line : int = __primitive("_get_user_line");
    chpl_region_begin(id, name, file, line);
  }

  def ~Region {
    const file : string = __primitive("_get_user_file");
    const line : int = __primitive("_get_user_line");
    chpl_region_end(id, name, file, line); 
  }

  def event(const str : string) : int {
    return chpl_profiling_event(name+str);
  }
  
  def value(const str : string, const val) : int{
    return chpl_profiling_value(name+str, val);
  }

  def values(const str : string, const vals...?numvals) : int {
    return chpl_profiling_values(name+str, (...vals));
  }
}

type AnonymousRegion = Region(RegionID);

