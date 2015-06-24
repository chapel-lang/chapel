/* GASNet-specific replacement for PLPA's configure-generated plpa.h */

#ifndef PLPA_H
  #define PLPA_H 1
  #undef PLPA_SYM_PREFIX
  #define PLPA_SYM_PREFIX gasneti_plpa_
  #include "plpa_bottom.h"
#endif /* PLPA_H */

