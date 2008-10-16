/* This code is in the public domain.
** Version: 1.1  Author: Walt Karas
*/

#include "hmm_intrnl.h"

U(size_aau) U(true_size)(void *payload_ptr)
  {
    register  head_record *head_ptr = PTR_REC_TO_HEAD(payload_ptr);

    #ifdef HMM_AUDIT_FAIL
    AUDIT_BLOCK(head_ptr)
    #endif

    /* Convert block size from BAUs to AAUs.  Subtract head size, leaving
    ** payload size.
    */
    return(
      (BLOCK_BAUS(head_ptr) * ((U(size_aau)) HMM_BLOCK_ALIGN_UNIT)) -
      HEAD_AAUS);
  }
