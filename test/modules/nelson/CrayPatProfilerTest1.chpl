
use CrayPatProfiler; 
//use DefaultProfiler; 

proc main() {
  const p : Profiler;

  writeln("1");
  const r1 : Region(RegionID, "r1");
  const r1a : Region(RegionID, "r1a");
  const r1b : Region(RegionID, "r1b");
  writeln("2");
  const r2 : AnonymousRegion; 
  const r2a : AnonymousRegion; 
  const r2b : AnonymousRegion; 

  writeln("main");
  p.event("main");

  for i in 1..3 {
    writeln("r12");
    const r12 : Region(RegionID, "r12");
    const r12x : Region(RegionID, "r12x");
    const r12y : Region(RegionID, "r12y");
    writeln("r13");
    const r13 : AnonymousRegion; 
    const r13x : AnonymousRegion; 
    const r13y : AnonymousRegion; 
    
    writeln("loop");
    r12.values("loop", i, i+1, i+2, i+3, i+4);
  }

  p.enable(false);
  writeln("no profiling here");
  p.enable(true);
  writeln("done");
}

