#ifndef _chpl_locale_model_h_
#define _chpl_locale_model_h_

//
// The flat locale model doesn't have sublocales.  Force the tasking
// layer to say there are no sublocales even if it knows otherwise
// (NUMA, e.g.).
//
#define CHPL_LOCALE_MODEL_NUM_SUBLOCALES 0

#endif // _chpl_locale_model_h_
