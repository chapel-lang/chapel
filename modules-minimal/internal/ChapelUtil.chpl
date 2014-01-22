// ChapelUtil.chpl
//
// Internal data structures module
//
pragma "no use ChapelStandard"
module ChapelUtil {

  pragma "no default functions"
  extern record chpl_main_argument {
  }

  //
  // These two are called from the emitted chpl_gen_main(), and
  // defined in the runtime.
  //
  extern proc chpl_rt_preUserCodeHook();
  extern proc chpl_rt_postUserCodeHook();
}
