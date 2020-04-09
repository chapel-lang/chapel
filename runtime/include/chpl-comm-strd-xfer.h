/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Common strided bulk put/get, adapted from comm-none.c
//

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-comm-callbacks.h"
#include "chpl-comm-callbacks-internal.h"
#include "chpl-mem.h"
#include "chpl-mem-desc.h"
#include "chpl-tasks.h"

// Don't get warning macros for chpl_comm_get etc
#include "chpl-comm-no-warning-macros.h"


//
//
// Common versions of the strided bulk transfer functions, for comm
// layer implementations that do not have native strided transfers.
//
// We use non-blocking transactions for the multiple GETs/PUTs of a
// strided transfer.  We can only track so many of these in flight at
// once, obviously.  Our caller will tell us the appropriate number
// for the specific comm layer implementation being used.
//
// Prefer initiating transactions to retiring them, in order to get
// them started earlier.  Once we have as many transactions in flight
// as we can track and are thus forced to retire some in order to
// initiate more, only retire ones that have already completed.  Don't
// wait for any more to finish.  Then at the end, wait for everything
// to complete.
//
static inline
void strd_nb_helper(chpl_comm_nb_handle_t (*xferFn)(void*, int32_t, void*,
                                                    size_t,
                                                    int32_t,
                                                    int, int32_t),
                    void* localAddr, int32_t remoteLocale, void* remoteAddr,
                    size_t cnt,
                    chpl_comm_nb_handle_t* handles, size_t* pCurrHandles,
                    size_t maxOutstandingXfers,
                    void (yieldFn)(void),
                    int32_t commID, int ln, int32_t fn)
{
  size_t currHandles = *pCurrHandles;

  if (currHandles >= maxOutstandingXfers) {
    // reached max in flight -- retire some to make room
    while (!chpl_comm_try_nb_some(handles, currHandles)) {
      (yieldFn)();
    }

    // compress retired transactions out of the list
    {
      size_t iOut, iIn;

      for (iOut = iIn = 0; iIn < currHandles; ) {
        if (handles[iIn] == NULL)
          iIn++;
        else
          handles[iOut++] = handles[iIn++];
      }

      currHandles = iOut;
    }
  }

  handles[currHandles] = (*xferFn)(localAddr, remoteLocale, remoteAddr, cnt,
                                   commID, ln, fn);
  if (handles[currHandles] != NULL)
    currHandles++;

  *pCurrHandles = currHandles;
}


static inline
void put_strd_common(void* dstaddr_arg, size_t* dststrides, int32_t dstlocale,
                     void* srcaddr_arg, size_t* srcstrides,
                     size_t* count, int32_t stridelevels, size_t elemSize,
                     size_t maxOutstandingXfers, void (yieldFn)(void),
                     int32_t commID, int ln, int32_t fn) {
  const size_t strlvls=(size_t)stridelevels;
  size_t i,j,k,t,total,off,x,carry;

  int8_t* dstaddr,*dstaddr1;
  int8_t* srcaddr,*srcaddr1;

  int *srcdisp, *dstdisp;

  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  chpl_comm_nb_handle_t handles[maxOutstandingXfers];
  size_t currHandles = 0;

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put_strd)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_put_strd, chpl_nodeID, dstlocale,
         .iu.comm_strd={srcaddr_arg, srcstrides, dstaddr_arg, dststrides, count,
                        stridelevels, elemSize, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  //Only count[0] and strides are measured in number of bytes.
  cnt[0]= count[0] * elemSize;
  if (strlvls>0) {
    srcstr[0] = srcstrides[0] * elemSize;
    dststr[0] = dststrides[0] * elemSize;
    for (i=1;i<strlvls;i++)
      {
        srcstr[i] = srcstrides[i] * elemSize;
        dststr[i] = dststrides[i] * elemSize;
        cnt[i]=count[i];
      }
    cnt[strlvls]=count[strlvls];
  }

  switch (strlvls) {
  case 0:
    chpl_comm_put(srcaddr_arg, dstlocale, dstaddr_arg, cnt[0], commID, ln, fn);
    break;

  case 1:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;
    for(i=0; i<cnt[1]; i++) {
      strd_nb_helper(chpl_comm_put_nb,
                     srcaddr, dstlocale, dstaddr, cnt[0],
                     handles, &currHandles, maxOutstandingXfers, yieldFn,
                     commID, ln, fn);
      srcaddr+=srcstr[0];
      dstaddr+=dststr[0];
    }
    break;

  case 2:
    for(i=0; i<cnt[2]; i++) {
      srcaddr = (int8_t*)srcaddr_arg + srcstr[1]*i;
      dstaddr = (int8_t*)dstaddr_arg + dststr[1]*i;
      for(j=0; j<cnt[1]; j++) {
        strd_nb_helper(chpl_comm_put_nb,
                       srcaddr, dstlocale, dstaddr, cnt[0],
                       handles, &currHandles, maxOutstandingXfers, yieldFn,
                       commID, ln, fn);
        srcaddr+=srcstr[0];
        dstaddr+=dststr[0];
      }
    }
    break;

  case 3:
    for(i=0; i<cnt[3]; i++) {
      srcaddr1 = (int8_t*)srcaddr_arg + srcstr[2]*i;
      dstaddr1 = (int8_t*)dstaddr_arg + dststr[2]*i;
      for(j=0; j<cnt[2]; j++) {
        srcaddr = srcaddr1 + srcstr[1]*j;
        dstaddr = dstaddr1 + dststr[1]*j;
        for(k=0; k<cnt[1]; k++) {
          strd_nb_helper(chpl_comm_put_nb,
                         srcaddr, dstlocale, dstaddr, cnt[0],
                         handles, &currHandles, maxOutstandingXfers, yieldFn,
                         commID, ln, fn);
          srcaddr+=srcstr[0];
          dstaddr+=dststr[0];
        }
      }
    }
    break;

  default:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;

    //Number of chpl_comm_put operations to do
    total=1;
    for (i=0; i<strlvls; i++)
      total=total*cnt[i+1];

    //displacement from the dstaddr and srcaddr start points
    srcdisp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
    dstdisp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);

    for (j=0; j<total; j++) {
      carry=1;
      for (t=1;t<=strlvls;t++) {
        if (cnt[t]*carry>=j+1) {  //IF 1
          x=j/carry;
          off =j-(carry*x);

          if (carry!=1) {  //IF 2
            srcdisp[j]=srcstr[t-1]*x+srcdisp[off];
            dstdisp[j]=dststr[t-1]*x+dstdisp[off];
          } else {  //ELSE 2
            srcdisp[j]=srcstr[t-1]*x;
            dstdisp[j]=dststr[t-1]*x;
          }
          strd_nb_helper(chpl_comm_put_nb,
                         srcaddr+srcdisp[j], dstlocale, dstaddr+dstdisp[j],
                         cnt[0],
                         handles, &currHandles, maxOutstandingXfers, yieldFn,
                         commID, ln, fn);
          break;

        } else { //ELSE 1
          carry=carry*cnt[t];
        }
      }
    } // for j
    chpl_mem_free(srcdisp,0,0);
    chpl_mem_free(dstdisp,0,0);
    break;
  }

  if (currHandles > 0) {
    (void) chpl_comm_wait_nb_some(handles, currHandles);
  }
}


static inline
void get_strd_common(void* dstaddr_arg, size_t* dststrides, int32_t srclocale,
                     void* srcaddr_arg, size_t* srcstrides,
                     size_t* count, int32_t stridelevels, size_t elemSize,
                     size_t maxOutstandingXfers, void (yieldFn)(void),
                     int32_t commID, int ln, int32_t fn) {
  const size_t strlvls=(size_t)stridelevels;
  size_t i,j,k,t,total,off,x,carry;

  int8_t* dstaddr,*dstaddr1;
  int8_t* srcaddr,*srcaddr1;

  int *srcdisp, *dstdisp;
  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  chpl_comm_nb_handle_t handles[maxOutstandingXfers];
  size_t currHandles = 0;


  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get_strd)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_get_strd, chpl_nodeID, srclocale,
       .iu.comm_strd={srcaddr_arg, srcstrides, dstaddr_arg, dststrides, count,
                      stridelevels, elemSize, commID, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }

  //Only count[0] and strides are measured in number of bytes.
  cnt[0]=count[0] * elemSize;
  if(strlvls>0){
    srcstr[0] = srcstrides[0] * elemSize;
    dststr[0] = dststrides[0] * elemSize;
    for (i=1;i<strlvls;i++)
      {
        srcstr[i] = srcstrides[i] * elemSize;
        dststr[i] = dststrides[i] * elemSize;
        cnt[i]=count[i];
      }
    cnt[strlvls]=count[strlvls];
  }

  switch(strlvls) {
  case 0:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;
    chpl_comm_get(dstaddr, srclocale, srcaddr, cnt[0],
                  commID, ln, fn);
    break;

  case 1:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;
    for(i=0; i<cnt[1]; i++) {
      strd_nb_helper(chpl_comm_get_nb,
                     dstaddr, srclocale, srcaddr, cnt[0],
                     handles, &currHandles, maxOutstandingXfers, yieldFn,
                     commID, ln, fn);
      srcaddr+=srcstr[0];
      dstaddr+=dststr[0];
    }
    break;

  case 2:
    for(i=0; i<cnt[2]; i++) {
      srcaddr = (int8_t*)srcaddr_arg + srcstr[1]*i;
      dstaddr = (int8_t*)dstaddr_arg + dststr[1]*i;
      for(j=0; j<cnt[1]; j++) {
        strd_nb_helper(chpl_comm_get_nb,
                       dstaddr, srclocale, srcaddr, cnt[0],
                       handles, &currHandles, maxOutstandingXfers, yieldFn,
                       commID, ln, fn);
        srcaddr+=srcstr[0];
        dstaddr+=dststr[0];
      }
    }
    break;

  case 3:
    for(i=0; i<cnt[3]; i++) {
      srcaddr1 = (int8_t*)srcaddr_arg + srcstr[2]*i;
      dstaddr1 = (int8_t*)dstaddr_arg + dststr[2]*i;
      for(j=0; j<cnt[2]; j++) {
        srcaddr = srcaddr1 + srcstr[1]*j;
        dstaddr = dstaddr1 + dststr[1]*j;
        for(k=0; k<cnt[1]; k++) {
          strd_nb_helper(chpl_comm_get_nb,
                         dstaddr, srclocale, srcaddr, cnt[0],
                         handles, &currHandles, maxOutstandingXfers, yieldFn,
                         commID, ln, fn);
          srcaddr+=srcstr[0];
          dstaddr+=dststr[0];
        }
      }
    }
    break;

  default:
    dstaddr=(int8_t*)dstaddr_arg;
    srcaddr=(int8_t*)srcaddr_arg;

    //Number of chpl_comm_get operations to do
    total=1;
    for (i=0; i<strlvls; i++)
      total=total*cnt[i+1];

    //displacement from the dstaddr and srcaddr start points
    srcdisp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);
    dstdisp=chpl_mem_allocMany(total,sizeof(int),CHPL_RT_MD_GETS_PUTS_STRIDES,0,0);

    for (j=0; j<total; j++) {
      carry=1;
      for (t=1;t<=strlvls;t++) {
        if (cnt[t]*carry>=j+1) {  //IF 1
          x=j/carry;
          off =j-(carry*x);

          if (carry!=1) {  //IF 2
            srcdisp[j]=srcstr[t-1]*x+srcdisp[off];
            dstdisp[j]=dststr[t-1]*x+dstdisp[off];
          } else {  //ELSE 2
            srcdisp[j]=srcstr[t-1]*x;
            dstdisp[j]=dststr[t-1]*x;
          }
          strd_nb_helper(chpl_comm_get_nb,
                         dstaddr+dstdisp[j], srclocale, srcaddr+srcdisp[j],
                         cnt[0],
                         handles, &currHandles, maxOutstandingXfers, yieldFn,
                         commID, ln, fn);
          break;

        } else {  //ELSE 1
          carry=carry*cnt[t];
        }
      }
    }
    chpl_mem_free(srcdisp,0,0);
    chpl_mem_free(dstdisp,0,0);
    break;
  }

  if (currHandles > 0) {
    (void) chpl_comm_wait_nb_some(handles, currHandles);
  }
}


#include "chpl-comm-warning-macros.h"
