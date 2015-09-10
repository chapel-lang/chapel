// various utilites for testing

extern proc chpl_cache_print();
pragma "insert line file info"
config const verbose=false;
extern proc printf(fmt: c_string, vals...?numvals): int;


/// label the output with "phases" //////////////////////////////////////////

var pfx = "# ";
var phase = 0;
var phase2 = 0;

//var phase_ptr = c_ptrTo(phase);
//var pfx_ptr = c_ptrTo(pfx);
//printf("phase_ptr %p pfx_ptr %p\n", phase_ptr, pfx_ptr);

on Locales[1] {
  printf("Running test on Locales[%i]\n", 1:int(32));
  assert(phase == 0);
  phase += 1;
  assert(phase == 1);
  var local_format = "%{####}".format(1);
  assert(local_format == "%{####}".format(1));
  pfx = local_format;
  //chpl_cache_print();
  var local_pfx = pfx;
  assert(local_pfx == local_format);
  phase += 1;
  pfx = "%{####}".format(phase);
  assert(pfx == "%{####}".format(phase));
  phase += 1;
  pfx = "%{####}".format(phase);
  assert(pfx == "%{####}".format(phase));
  phase += 1;
  pfx = "%{####}".format(phase);
  assert(pfx == "%{####}".format(phase));
}
