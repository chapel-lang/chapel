
record Profiler {
  param name : string = "";
    
  proc enable(const state : bool = true) : int {
    return chpl_profiling_enable(state);
  }
  
  proc event(const str : string) : int {
    return chpl_profiling_event(name+str);
  }
  
  proc value(const str : string, const val) : int {
    return chpl_profiling_value(name+str, val);
  }

  proc values(const str : string, const vals...?numvals) : int {
    return chpl_profiling_values(name+str, (...vals));
  }
}

const Profilers : [LocaleSpace] Profiler;


type RegionID = __primitive("_next_uint32");

record Region : Profiler{
  param id : uint;
  param name : string = "";

  proc initialize() {
    const file : string = __primitive("_get_user_file");
    const line : int = __primitive("_get_user_line");
    chpl_region_begin(id, name, file, line);
  }

  proc ~Region {
    const file : string = __primitive("_get_user_file");
    const line : int = __primitive("_get_user_line");
    chpl_region_end(id, name, file, line); 
  }

  proc event(const str : string) : int {
    return chpl_profiling_event(name+str);
  }
  
  proc value(const str : string, const val) : int{
    return chpl_profiling_value(name+str, val);
  }

  proc values(const str : string, const vals...?numvals) : int {
    return chpl_profiling_values(name+str, (...vals));
  }
}

type AnonymousRegion = Region(RegionID);

