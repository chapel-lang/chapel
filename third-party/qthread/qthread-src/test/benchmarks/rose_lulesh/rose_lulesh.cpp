static void *xomp_critical_user_;
/*

                 Copyright (c) 2010.
      Lawrence Livermore National Security, LLC.
Produced at the Lawrence Livermore National Laboratory.
                  LLNL-CODE-461231
                All rights reserved.

This file is part of LULESH, Version 1.0.
Please also read this link -- http://www.opensource.org/licenses/index.php

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the disclaimer below.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the disclaimer (as noted below)
     in the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the LLNS/LLNL nor the names of its contributors
     may be used to endorse or promote products derived from this software
     without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL SECURITY, LLC,
THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


Additional BSD Notice

1. This notice is required to be provided under our contract with the U.S.
   Department of Energy (DOE). This work was produced at Lawrence Livermore
   National Laboratory under Contract No. DE-AC52-07NA27344 with the DOE.

2. Neither the United States Government nor Lawrence Livermore National
   Security, LLC nor any of their employees, makes any warranty, express
   or implied, or assumes any liability or responsibility for the accuracy,
   completeness, or usefulness of any information, apparatus, product, or
   process disclosed, or represents that its use would not infringe
   privately-owned rights.

3. Also, reference herein to any specific commercial products, process, or
   services by trade name, trademark, manufacturer or otherwise does not
   necessarily constitute or imply its endorsement, recommendation, or
   favoring by the United States Government or Lawrence Livermore National
   Security, LLC. The views and opinions of authors expressed herein do not
   necessarily state or reflect those of the United States Government or
   Lawrence Livermore National Security, LLC, and shall not be used for
   advertising or product endorsement purposes.

*/
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define LULESH_SHOW_PROGRESS 0
#include "libxomp.h" 
enum __unnamed_enum___F0_L72_C1_VolumeError__COMMA__QStopError {VolumeError=-1,QStopError=-2};
/****************************************************/
/* Allow flexibility for arithmetic representations */
/****************************************************/
/* Could also support fixed point and interval arithmetic types */
typedef float real4;
typedef double real8;
/* 10 bytes on x86 */
typedef long double real10;
/* array subscript and loop index */
typedef int Index_t;
/* floating point representation */
typedef real8 Real_t;
/* integer representation */
typedef int Int_t;

inline real4 SQRT(real4 arg)
{
  return sqrtf(arg);
}

inline real8 SQRT(real8 arg)
{
  return sqrt(arg);
}

inline real10 SQRT(real10 arg)
{
  return sqrtl(arg);
}

inline real4 CBRT(real4 arg)
{
  return cbrtf(arg);
}

inline real8 CBRT(real8 arg)
{
  return cbrt(arg);
}

inline real10 CBRT(real10 arg)
{
  return cbrtl(arg);
}

inline real4 FABS(real4 arg)
{
  return fabsf(arg);
}

inline real8 FABS(real8 arg)
{
  return fabs(arg);
}

inline real10 FABS(real10 arg)
{
  return fabsl(arg);
}
/************************************************************/
/* Allow for flexible data layout experiments by separating */
/* array interface from underlying implementation.          */
/************************************************************/
struct Domain ;
struct Domain {
/* This first implementation allows for runnable code */
/* and is not meant to be optimal. Final implementation */
/* should separate declaration and allocation phases */
/* so that allocation can be scheduled in a cache conscious */
/* manner. */
/**************/
/* Allocation */
/**************/

inline void AllocateNodalPersistent(size_t size)
{
  (this) -> m_x. resize (size);
  (this) -> m_y. resize (size);
  (this) -> m_z. resize (size);
  (this) -> m_xd. resize (size,(((Real_t )0.)));
  (this) -> m_yd. resize (size,(((Real_t )0.)));
  (this) -> m_zd. resize (size,(((Real_t )0.)));
  (this) -> m_xdd. resize (size,(((Real_t )0.)));
  (this) -> m_ydd. resize (size,(((Real_t )0.)));
  (this) -> m_zdd. resize (size,(((Real_t )0.)));
  (this) -> m_fx. resize (size);
  (this) -> m_fy. resize (size);
  (this) -> m_fz. resize (size);
  (this) -> m_nodalMass. resize (size,(((Real_t )0.)));
}

inline void AllocateElemPersistent(size_t size)
{
  (this) -> m_matElemlist. resize (size);
  (this) -> m_nodelist. resize (((8) * size));
  (this) -> m_lxim. resize (size);
  (this) -> m_lxip. resize (size);
  (this) -> m_letam. resize (size);
  (this) -> m_letap. resize (size);
  (this) -> m_lzetam. resize (size);
  (this) -> m_lzetap. resize (size);
  (this) -> m_elemBC. resize (size);
  (this) -> m_e. resize (size,(((Real_t )0.)));
  (this) -> m_p. resize (size,(((Real_t )0.)));
  (this) -> m_q. resize (size);
  (this) -> m_ql. resize (size);
  (this) -> m_qq. resize (size);
  (this) -> m_v. resize (size,(1.0));
  (this) -> m_volo. resize (size);
  (this) -> m_delv. resize (size);
  (this) -> m_vdov. resize (size);
  (this) -> m_arealg. resize (size);
  (this) -> m_ss. resize (size);
  (this) -> m_elemMass. resize (size);
}
/* Temporaries should not be initialized in bulk but */
/* this is a runnable placeholder for now */

inline void AllocateElemTemporary(size_t size)
{
  (this) -> m_dxx. resize (size);
  (this) -> m_dyy. resize (size);
  (this) -> m_dzz. resize (size);
  (this) -> m_delv_xi. resize (size);
  (this) -> m_delv_eta. resize (size);
  (this) -> m_delv_zeta. resize (size);
  (this) -> m_delx_xi. resize (size);
  (this) -> m_delx_eta. resize (size);
  (this) -> m_delx_zeta. resize (size);
  (this) -> m_vnew. resize (size);
}

inline void AllocateNodesets(size_t size)
{
  (this) -> m_symmX. resize (size);
  (this) -> m_symmY. resize (size);
  (this) -> m_symmZ. resize (size);
}

inline void AllocateNodeElemIndexes()
{
  Index_t m;
  Index_t numElem = (this) ->  numElem ();
  Index_t numNode = (this) ->  numNode ();
/* set up node-centered indexing of elements */
  (this) -> m_nodeElemCount. resize ((numNode));
  for (Index_t i = (0); i < numNode; ++i) {
    (this) ->  nodeElemCount (i) = (0);
  }
  for (Index_t i = (0); i < numElem; ++i) {
    Index_t *nl = (this) ->  nodelist (i);
    for (Index_t j = (0); j < 8; ++j) {
      ++(this) ->  nodeElemCount (nl[j]);
    }
  }
  (this) -> m_nodeElemStart. resize ((numNode));
  (this) ->  nodeElemStart ((0)) = (0);
  for (Index_t i = (1); i < numNode; ++i) {
    (this) ->  nodeElemStart (i) = ((this) ->  nodeElemStart ((i - 1)) + (this) ->  nodeElemCount ((i - 1)));
  }
//       m_nodeElemList.resize(nodeElemStart(numNode-1) +
//                             nodeElemCount(numNode-1));
  (this) -> m_nodeElemCornerList. resize ((((this) ->  nodeElemStart ((numNode - 1)) + (this) ->  nodeElemCount ((numNode - 1)))));
  for (Index_t i = (0); i < numNode; ++i) {
    (this) ->  nodeElemCount (i) = (0);
  }
  for (Index_t i = (0); i < numElem; ++i) {
    Index_t *nl = (this) ->  nodelist (i);
    for (Index_t j = (0); j < 8; ++j) {
      Index_t m = nl[j];
      Index_t k = ((i * 8) + j);
      Index_t offset = ((this) ->  nodeElemStart (m) + (this) ->  nodeElemCount (m));
//             nodeElemList(offset) = i;
      (this) ->  nodeElemCornerList (offset) = k;
      ++(this) ->  nodeElemCount (m);
    }
  }
  Index_t clSize = ((this) -> m_nodeElemCornerList. size ());
  for (Index_t i = (0); i < clSize; ++i) {
    Index_t clv = (this) ->  nodeElemCornerList (i);
    if ((clv < 0) || (clv > (numElem * 8))) {
      fprintf(stderr,"AllocateNodeElemIndexes(): nodeElemCornerList entry out of range!\n");
      exit(1);
    }
  }
}
/**********/
/* Access */
/**********/
/* Node-centered */

inline Real_t &x(Index_t idx)
{
  return (this) -> m_x[(idx)];
}

inline Real_t &y(Index_t idx)
{
  return (this) -> m_y[(idx)];
}

inline Real_t &z(Index_t idx)
{
  return (this) -> m_z[(idx)];
}

inline Real_t &xd(Index_t idx)
{
  return (this) -> m_xd[(idx)];
}

inline Real_t &yd(Index_t idx)
{
  return (this) -> m_yd[(idx)];
}

inline Real_t &zd(Index_t idx)
{
  return (this) -> m_zd[(idx)];
}

inline Real_t &xdd(Index_t idx)
{
  return (this) -> m_xdd[(idx)];
}

inline Real_t &ydd(Index_t idx)
{
  return (this) -> m_ydd[(idx)];
}

inline Real_t &zdd(Index_t idx)
{
  return (this) -> m_zdd[(idx)];
}

inline Real_t &fx(Index_t idx)
{
  return (this) -> m_fx[(idx)];
}

inline Real_t &fy(Index_t idx)
{
  return (this) -> m_fy[(idx)];
}

inline Real_t &fz(Index_t idx)
{
  return (this) -> m_fz[(idx)];
}

inline Real_t &nodalMass(Index_t idx)
{
  return (this) -> m_nodalMass[(idx)];
}

inline Index_t &symmX(Index_t idx)
{
  return (this) -> m_symmX[(idx)];
}

inline Index_t &symmY(Index_t idx)
{
  return (this) -> m_symmY[(idx)];
}

inline Index_t &symmZ(Index_t idx)
{
  return (this) -> m_symmZ[(idx)];
}

inline Index_t &nodeElemCount(Index_t idx)
{
  return (this) -> m_nodeElemCount[(idx)];
}

inline Index_t &nodeElemStart(Index_t idx)
{
  return (this) -> m_nodeElemStart[(idx)];
}
//   Index_t& nodeElemList(Index_t idx)  { return m_nodeElemList[idx] ; }

inline Index_t &nodeElemCornerList(Index_t i)
{
  return (this) -> m_nodeElemCornerList[(i)];
}
/* Element-centered */

inline Index_t &matElemlist(Index_t idx)
{
  return (this) -> m_matElemlist[(idx)];
}

inline Index_t *nodelist(Index_t idx)
{
  return &(this) -> m_nodelist[(((((Index_t )8)) * idx))];
}

inline Index_t &lxim(Index_t idx)
{
  return (this) -> m_lxim[(idx)];
}

inline Index_t &lxip(Index_t idx)
{
  return (this) -> m_lxip[(idx)];
}

inline Index_t &letam(Index_t idx)
{
  return (this) -> m_letam[(idx)];
}

inline Index_t &letap(Index_t idx)
{
  return (this) -> m_letap[(idx)];
}

inline Index_t &lzetam(Index_t idx)
{
  return (this) -> m_lzetam[(idx)];
}

inline Index_t &lzetap(Index_t idx)
{
  return (this) -> m_lzetap[(idx)];
}

inline Int_t &elemBC(Index_t idx)
{
  return (this) -> m_elemBC[(idx)];
}

inline Real_t &dxx(Index_t idx)
{
  return (this) -> m_dxx[(idx)];
}

inline Real_t &dyy(Index_t idx)
{
  return (this) -> m_dyy[(idx)];
}

inline Real_t &dzz(Index_t idx)
{
  return (this) -> m_dzz[(idx)];
}

inline Real_t &delv_xi(Index_t idx)
{
  return (this) -> m_delv_xi[(idx)];
}

inline Real_t &delv_eta(Index_t idx)
{
  return (this) -> m_delv_eta[(idx)];
}

inline Real_t &delv_zeta(Index_t idx)
{
  return (this) -> m_delv_zeta[(idx)];
}

inline Real_t &delx_xi(Index_t idx)
{
  return (this) -> m_delx_xi[(idx)];
}

inline Real_t &delx_eta(Index_t idx)
{
  return (this) -> m_delx_eta[(idx)];
}

inline Real_t &delx_zeta(Index_t idx)
{
  return (this) -> m_delx_zeta[(idx)];
}

inline Real_t &e(Index_t idx)
{
  return (this) -> m_e[(idx)];
}

inline Real_t &p(Index_t idx)
{
  return (this) -> m_p[(idx)];
}

inline Real_t &q(Index_t idx)
{
  return (this) -> m_q[(idx)];
}

inline Real_t &ql(Index_t idx)
{
  return (this) -> m_ql[(idx)];
}

inline Real_t &qq(Index_t idx)
{
  return (this) -> m_qq[(idx)];
}

inline Real_t &v(Index_t idx)
{
  return (this) -> m_v[(idx)];
}

inline Real_t &volo(Index_t idx)
{
  return (this) -> m_volo[(idx)];
}

inline Real_t &vnew(Index_t idx)
{
  return (this) -> m_vnew[(idx)];
}

inline Real_t &delv(Index_t idx)
{
  return (this) -> m_delv[(idx)];
}

inline Real_t &vdov(Index_t idx)
{
  return (this) -> m_vdov[(idx)];
}

inline Real_t &arealg(Index_t idx)
{
  return (this) -> m_arealg[(idx)];
}

inline Real_t &ss(Index_t idx)
{
  return (this) -> m_ss[(idx)];
}

inline Real_t &elemMass(Index_t idx)
{
  return (this) -> m_elemMass[(idx)];
}
/* Params */

inline Real_t &dtfixed()
{
  return (this) -> m_dtfixed;
}

inline Real_t &time()
{
  return (this) -> m_time;
}

inline Real_t &deltatime()
{
  return (this) -> m_deltatime;
}

inline Real_t &deltatimemultlb()
{
  return (this) -> m_deltatimemultlb;
}

inline Real_t &deltatimemultub()
{
  return (this) -> m_deltatimemultub;
}

inline Real_t &stoptime()
{
  return (this) -> m_stoptime;
}

inline Real_t &u_cut()
{
  return (this) -> m_u_cut;
}

inline Real_t &hgcoef()
{
  return (this) -> m_hgcoef;
}

inline Real_t &qstop()
{
  return (this) -> m_qstop;
}

inline Real_t &monoq_max_slope()
{
  return (this) -> m_monoq_max_slope;
}

inline Real_t &monoq_limiter_mult()
{
  return (this) -> m_monoq_limiter_mult;
}

inline Real_t &e_cut()
{
  return (this) -> m_e_cut;
}

inline Real_t &p_cut()
{
  return (this) -> m_p_cut;
}

inline Real_t &ss4o3()
{
  return (this) -> m_ss4o3;
}

inline Real_t &q_cut()
{
  return (this) -> m_q_cut;
}

inline Real_t &v_cut()
{
  return (this) -> m_v_cut;
}

inline Real_t &qlc_monoq()
{
  return (this) -> m_qlc_monoq;
}

inline Real_t &qqc_monoq()
{
  return (this) -> m_qqc_monoq;
}

inline Real_t &qqc()
{
  return (this) -> m_qqc;
}

inline Real_t &eosvmax()
{
  return (this) -> m_eosvmax;
}

inline Real_t &eosvmin()
{
  return (this) -> m_eosvmin;
}

inline Real_t &pmin()
{
  return (this) -> m_pmin;
}

inline Real_t &emin()
{
  return (this) -> m_emin;
}

inline Real_t &dvovmax()
{
  return (this) -> m_dvovmax;
}

inline Real_t &refdens()
{
  return (this) -> m_refdens;
}

inline Real_t &dtcourant()
{
  return (this) -> m_dtcourant;
}

inline Real_t &dthydro()
{
  return (this) -> m_dthydro;
}

inline Real_t &dtmax()
{
  return (this) -> m_dtmax;
}

inline Int_t &cycle()
{
  return (this) -> m_cycle;
}

inline Index_t &sizeX()
{
  return (this) -> m_sizeX;
}

inline Index_t &sizeY()
{
  return (this) -> m_sizeY;
}

inline Index_t &sizeZ()
{
  return (this) -> m_sizeZ;
}

inline Index_t &numElem()
{
  return (this) -> m_numElem;
}

inline Index_t &numNode()
{
  return (this) -> m_numNode;
}
/******************/
/* Implementation */
/******************/
/* Node-centered */
/* coordinates */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_x;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_y;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_z;
/* velocities */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_xd;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_yd;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_zd;
/* accelerations */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_xdd;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_ydd;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_zdd;
/* forces */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_fx;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_fy;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_fz;
/* mass */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_nodalMass;
/* symmetry plane nodesets */
class std::vector< Index_t  , std::allocator< Index_t  >  > m_symmX;
class std::vector< Index_t  , std::allocator< Index_t  >  > m_symmY;
class std::vector< Index_t  , std::allocator< Index_t  >  > m_symmZ;
class std::vector< Index_t  , std::allocator< Index_t  >  > m_nodeElemCount;
class std::vector< Index_t  , std::allocator< Index_t  >  > m_nodeElemStart;
//   std::vector<Index_t> m_nodeElemList ;
class std::vector< Index_t  , std::allocator< Index_t  >  > m_nodeElemCornerList;
/* Element-centered */
/* material indexset */
class std::vector< Index_t  , std::allocator< Index_t  >  > m_matElemlist;
/* elemToNode connectivity */
class std::vector< Index_t  , std::allocator< Index_t  >  > m_nodelist;
/* element connectivity across each face */
class std::vector< Index_t  , std::allocator< Index_t  >  > m_lxim;
class std::vector< Index_t  , std::allocator< Index_t  >  > m_lxip;
class std::vector< Index_t  , std::allocator< Index_t  >  > m_letam;
class std::vector< Index_t  , std::allocator< Index_t  >  > m_letap;
class std::vector< Index_t  , std::allocator< Index_t  >  > m_lzetam;
class std::vector< Index_t  , std::allocator< Index_t  >  > m_lzetap;
/* symmetry/free-surface flags for each elem face */
class std::vector< Index_t  , std::allocator< Index_t  >  > m_elemBC;
/* principal strains -- temporary */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_dxx;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_dyy;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_dzz;
/* velocity gradient -- temporary */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_delv_xi;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_delv_eta;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_delv_zeta;
/* coordinate gradient -- temporary */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_delx_xi;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_delx_eta;
class std::vector< Real_t  , std::allocator< Real_t  >  > m_delx_zeta;
/* energy */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_e;
/* pressure */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_p;
/* q */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_q;
/* linear term for q */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_ql;
/* quadratic term for q */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_qq;
/* relative volume */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_v;
/* reference volume */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_volo;
/* new relative volume -- temporary */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_vnew;
/* m_vnew - m_v */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_delv;
/* volume derivative over volume */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_vdov;
/* characteristic length of an element */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_arealg;
/* "sound speed" */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_ss;
/* mass */
class std::vector< Real_t  , std::allocator< Real_t  >  > m_elemMass;
/* Parameters */
/* fixed time increment */
Real_t m_dtfixed;
/* current time */
Real_t m_time;
/* variable time increment */
Real_t m_deltatime;
Real_t m_deltatimemultlb;
Real_t m_deltatimemultub;
/* end time for simulation */
Real_t m_stoptime;
/* velocity tolerance */
Real_t m_u_cut;
/* hourglass control */
Real_t m_hgcoef;
/* excessive q indicator */
Real_t m_qstop;
Real_t m_monoq_max_slope;
Real_t m_monoq_limiter_mult;
/* energy tolerance */
Real_t m_e_cut;
/* pressure tolerance */
Real_t m_p_cut;
Real_t m_ss4o3;
/* q tolerance */
Real_t m_q_cut;
/* relative volume tolerance */
Real_t m_v_cut;
/* linear term coef for q */
Real_t m_qlc_monoq;
/* quadratic term coef for q */
Real_t m_qqc_monoq;
Real_t m_qqc;
Real_t m_eosvmax;
Real_t m_eosvmin;
/* pressure floor */
Real_t m_pmin;
/* energy floor */
Real_t m_emin;
/* maximum allowable volume change */
Real_t m_dvovmax;
/* reference density */
Real_t m_refdens;
/* courant constraint */
Real_t m_dtcourant;
/* volume change constraint */
Real_t m_dthydro;
/* maximum allowable time increment */
Real_t m_dtmax;
/* iteration count for simulation */
Int_t m_cycle;
/* X,Y,Z extent of this block */
Index_t m_sizeX;
Index_t m_sizeY;
Index_t m_sizeZ;
/* Elements/Nodes in this domain */
Index_t m_numElem;
Index_t m_numNode;}domain;
template < typename T >
T * Allocate ( size_t size )
{
   return static_cast < T * > ( malloc ( sizeof ( T ) * size ) );
}
template < typename T >
void Release ( T * * ptr )
{
   if ( * ptr != __null ) {
      free ( * ptr );
      * ptr = __null;
   }
}
/* Stuff needed for boundary conditions */
/* 2 BCs on each of 6 hexahedral faces (12 bits) */
#define XI_M        0x003
#define XI_M_SYMM   0x001
#define XI_M_FREE   0x002
#define XI_P        0x00c
#define XI_P_SYMM   0x004
#define XI_P_FREE   0x008
#define ETA_M       0x030
#define ETA_M_SYMM  0x010
#define ETA_M_FREE  0x020
#define ETA_P       0x0c0
#define ETA_P_SYMM  0x040
#define ETA_P_FREE  0x080
#define ZETA_M      0x300
#define ZETA_M_SYMM 0x100
#define ZETA_M_FREE 0x200
#define ZETA_P      0xc00
#define ZETA_P_SYMM 0x400
#define ZETA_P_FREE 0x800

inline static void TimeIncrement()
{
  Real_t targetdt = (domain. stoptime () - domain. time ());
  if ((domain. dtfixed () <= (((Real_t )0.))) && (domain. cycle () != (((Int_t )0)))) {
    Real_t ratio;
    Real_t olddt = domain. deltatime ();
/* This will require a reduction in parallel */
    Real_t newdt = ((Real_t )1.0e+20);
    if (domain. dtcourant () < newdt) {
      newdt = (domain. dtcourant () / (((Real_t )2.0)));
    }
    if (domain. dthydro () < newdt) {
      newdt = ((domain. dthydro () * (((Real_t )2.0))) / (((Real_t )3.0)));
    }
    ratio = (newdt / olddt);
    if (ratio >= (((Real_t )1.0))) {
      if (ratio < domain. deltatimemultlb ()) {
        newdt = olddt;
      }
      else if (ratio > domain. deltatimemultub ()) {
        newdt = (olddt * domain. deltatimemultub ());
      }
    }
    if (newdt > domain. dtmax ()) {
      newdt = domain. dtmax ();
    }
    domain. deltatime () = newdt;
  }
/* TRY TO PREVENT VERY SMALL SCALING ON THE NEXT CYCLE */
  if ((targetdt > domain. deltatime ()) && (targetdt < (((((Real_t )4.0)) * domain. deltatime ()) / (((Real_t )3.0))))) {
    targetdt = (((((Real_t )2.0)) * domain. deltatime ()) / (((Real_t )3.0)));
  }
  if (targetdt < domain. deltatime ()) {
    domain. deltatime () = targetdt;
  }
  domain. time () += domain. deltatime ();
  ++domain. cycle ();
}

struct OUT__30__1527___data 
{
  void *numElem_p;
  void *sigxx_p;
  void *sigyy_p;
  void *sigzz_p;
}
;
static void OUT__30__1527__(void *__out_argv);

inline static void InitStressTermsForElems(Index_t numElem,Real_t *sigxx,Real_t *sigyy,Real_t *sigzz)
{
//
// pull in the stresses appropriate to the hydro integration
//
  Index_t i;
  struct OUT__30__1527___data __out_argv30__1527__;
  __out_argv30__1527__.OUT__30__1527___data::sigzz_p = ((void *)(&sigzz));
  __out_argv30__1527__.OUT__30__1527___data::sigyy_p = ((void *)(&sigyy));
  __out_argv30__1527__.OUT__30__1527___data::sigxx_p = ((void *)(&sigxx));
  __out_argv30__1527__.OUT__30__1527___data::numElem_p = ((void *)(&numElem));
  XOMP_parallel_start(OUT__30__1527__,&__out_argv30__1527__,1,0,"OUT__30__1527__");
  XOMP_parallel_end();
}

inline static void CalcElemShapeFunctionDerivatives(const Real_t *const x,const Real_t *const y,const Real_t *const z,Real_t (b)[][8UL],Real_t *const volume)
{
  const Real_t x0 = x[0];
  const Real_t x1 = x[1];
  const Real_t x2 = x[2];
  const Real_t x3 = x[3];
  const Real_t x4 = x[4];
  const Real_t x5 = x[5];
  const Real_t x6 = x[6];
  const Real_t x7 = x[7];
  const Real_t y0 = y[0];
  const Real_t y1 = y[1];
  const Real_t y2 = y[2];
  const Real_t y3 = y[3];
  const Real_t y4 = y[4];
  const Real_t y5 = y[5];
  const Real_t y6 = y[6];
  const Real_t y7 = y[7];
  const Real_t z0 = z[0];
  const Real_t z1 = z[1];
  const Real_t z2 = z[2];
  const Real_t z3 = z[3];
  const Real_t z4 = z[4];
  const Real_t z5 = z[5];
  const Real_t z6 = z[6];
  const Real_t z7 = z[7];
  Real_t fjxxi;
  Real_t fjxet;
  Real_t fjxze;
  Real_t fjyxi;
  Real_t fjyet;
  Real_t fjyze;
  Real_t fjzxi;
  Real_t fjzet;
  Real_t fjzze;
  Real_t cjxxi;
  Real_t cjxet;
  Real_t cjxze;
  Real_t cjyxi;
  Real_t cjyet;
  Real_t cjyze;
  Real_t cjzxi;
  Real_t cjzet;
  Real_t cjzze;
  fjxxi = (.125 * ((((x6 - x0) + (x5 - x3)) - (x7 - x1)) - (x4 - x2)));
  fjxet = (.125 * ((((x6 - x0) - (x5 - x3)) + (x7 - x1)) - (x4 - x2)));
  fjxze = (.125 * ((((x6 - x0) + (x5 - x3)) + (x7 - x1)) + (x4 - x2)));
  fjyxi = (.125 * ((((y6 - y0) + (y5 - y3)) - (y7 - y1)) - (y4 - y2)));
  fjyet = (.125 * ((((y6 - y0) - (y5 - y3)) + (y7 - y1)) - (y4 - y2)));
  fjyze = (.125 * ((((y6 - y0) + (y5 - y3)) + (y7 - y1)) + (y4 - y2)));
  fjzxi = (.125 * ((((z6 - z0) + (z5 - z3)) - (z7 - z1)) - (z4 - z2)));
  fjzet = (.125 * ((((z6 - z0) - (z5 - z3)) + (z7 - z1)) - (z4 - z2)));
  fjzze = (.125 * ((((z6 - z0) + (z5 - z3)) + (z7 - z1)) + (z4 - z2)));
/* compute cofactors */
  cjxxi = ((fjyet * fjzze) - (fjzet * fjyze));
  cjxet = (-(fjyxi * fjzze) + (fjzxi * fjyze));
  cjxze = ((fjyxi * fjzet) - (fjzxi * fjyet));
  cjyxi = (-(fjxet * fjzze) + (fjzet * fjxze));
  cjyet = ((fjxxi * fjzze) - (fjzxi * fjxze));
  cjyze = (-(fjxxi * fjzet) + (fjzxi * fjxet));
  cjzxi = ((fjxet * fjyze) - (fjyet * fjxze));
  cjzet = (-(fjxxi * fjyze) + (fjyxi * fjxze));
  cjzze = ((fjxxi * fjyet) - (fjyxi * fjxet));
/* calculate partials :
     this need only be done for l = 0,1,2,3   since , by symmetry ,
     (6,7,4,5) = - (0,1,2,3) .
  */
  b[0][0] = ((-cjxxi - cjxet) - cjxze);
  b[0][1] = ((cjxxi - cjxet) - cjxze);
  b[0][2] = ((cjxxi + cjxet) - cjxze);
  b[0][3] = ((-cjxxi + cjxet) - cjxze);
  b[0][4] = -b[0][2];
  b[0][5] = -b[0][3];
  b[0][6] = -b[0][0];
  b[0][7] = -b[0][1];
  b[1][0] = ((-cjyxi - cjyet) - cjyze);
  b[1][1] = ((cjyxi - cjyet) - cjyze);
  b[1][2] = ((cjyxi + cjyet) - cjyze);
  b[1][3] = ((-cjyxi + cjyet) - cjyze);
  b[1][4] = -b[1][2];
  b[1][5] = -b[1][3];
  b[1][6] = -b[1][0];
  b[1][7] = -b[1][1];
  b[2][0] = ((-cjzxi - cjzet) - cjzze);
  b[2][1] = ((cjzxi - cjzet) - cjzze);
  b[2][2] = ((cjzxi + cjzet) - cjzze);
  b[2][3] = ((-cjzxi + cjzet) - cjzze);
  b[2][4] = -b[2][2];
  b[2][5] = -b[2][3];
  b[2][6] = -b[2][0];
  b[2][7] = -b[2][1];
/* calculate jacobian determinant (volume) */
   *volume = ((((Real_t )8.)) * (((fjxet * cjxet) + (fjyet * cjyet)) + (fjzet * cjzet)));
}

inline static void SumElemFaceNormal(Real_t *normalX0,Real_t *normalY0,Real_t *normalZ0,Real_t *normalX1,Real_t *normalY1,Real_t *normalZ1,Real_t *normalX2,Real_t *normalY2,Real_t *normalZ2,Real_t *normalX3,Real_t *normalY3,Real_t *normalZ3,const Real_t x0,const Real_t y0,const Real_t z0,const Real_t x1,const Real_t y1,const Real_t z1,const Real_t x2,const Real_t y2,const Real_t z2,const Real_t x3,const Real_t y3,const Real_t z3)
{
  Real_t bisectX0 = ((((Real_t )0.5)) * (((x3 + x2) - x1) - x0));
  Real_t bisectY0 = ((((Real_t )0.5)) * (((y3 + y2) - y1) - y0));
  Real_t bisectZ0 = ((((Real_t )0.5)) * (((z3 + z2) - z1) - z0));
  Real_t bisectX1 = ((((Real_t )0.5)) * (((x2 + x1) - x3) - x0));
  Real_t bisectY1 = ((((Real_t )0.5)) * (((y2 + y1) - y3) - y0));
  Real_t bisectZ1 = ((((Real_t )0.5)) * (((z2 + z1) - z3) - z0));
  Real_t areaX = ((((Real_t )0.25)) * ((bisectY0 * bisectZ1) - (bisectZ0 * bisectY1)));
  Real_t areaY = ((((Real_t )0.25)) * ((bisectZ0 * bisectX1) - (bisectX0 * bisectZ1)));
  Real_t areaZ = ((((Real_t )0.25)) * ((bisectX0 * bisectY1) - (bisectY0 * bisectX1)));
   *normalX0 += areaX;
   *normalX1 += areaX;
   *normalX2 += areaX;
   *normalX3 += areaX;
   *normalY0 += areaY;
   *normalY1 += areaY;
   *normalY2 += areaY;
   *normalY3 += areaY;
   *normalZ0 += areaZ;
   *normalZ1 += areaZ;
   *normalZ2 += areaZ;
   *normalZ3 += areaZ;
}

inline static void CalcElemNodeNormals(Real_t pfx[8UL],Real_t pfy[8UL],Real_t pfz[8UL],const Real_t x[8UL],const Real_t y[8UL],const Real_t z[8UL])
{
  for (Index_t i = (0); i < 8; ++i) {
    pfx[i] = ((Real_t )0.);
    pfy[i] = ((Real_t )0.);
    pfz[i] = ((Real_t )0.);
  }
/* evaluate face one: nodes 0, 1, 2, 3 */
  SumElemFaceNormal((pfx + 0),(pfy + 0),(pfz + 0),(pfx + 1),(pfy + 1),(pfz + 1),(pfx + 2),(pfy + 2),(pfz + 2),(pfx + 3),(pfy + 3),(pfz + 3),x[0],y[0],z[0],x[1],y[1],z[1],x[2],y[2],z[2],x[3],y[3],z[3]);
/* evaluate face two: nodes 0, 4, 5, 1 */
  SumElemFaceNormal((pfx + 0),(pfy + 0),(pfz + 0),(pfx + 4),(pfy + 4),(pfz + 4),(pfx + 5),(pfy + 5),(pfz + 5),(pfx + 1),(pfy + 1),(pfz + 1),x[0],y[0],z[0],x[4],y[4],z[4],x[5],y[5],z[5],x[1],y[1],z[1]);
/* evaluate face three: nodes 1, 5, 6, 2 */
  SumElemFaceNormal((pfx + 1),(pfy + 1),(pfz + 1),(pfx + 5),(pfy + 5),(pfz + 5),(pfx + 6),(pfy + 6),(pfz + 6),(pfx + 2),(pfy + 2),(pfz + 2),x[1],y[1],z[1],x[5],y[5],z[5],x[6],y[6],z[6],x[2],y[2],z[2]);
/* evaluate face four: nodes 2, 6, 7, 3 */
  SumElemFaceNormal((pfx + 2),(pfy + 2),(pfz + 2),(pfx + 6),(pfy + 6),(pfz + 6),(pfx + 7),(pfy + 7),(pfz + 7),(pfx + 3),(pfy + 3),(pfz + 3),x[2],y[2],z[2],x[6],y[6],z[6],x[7],y[7],z[7],x[3],y[3],z[3]);
/* evaluate face five: nodes 3, 7, 4, 0 */
  SumElemFaceNormal((pfx + 3),(pfy + 3),(pfz + 3),(pfx + 7),(pfy + 7),(pfz + 7),(pfx + 4),(pfy + 4),(pfz + 4),(pfx + 0),(pfy + 0),(pfz + 0),x[3],y[3],z[3],x[7],y[7],z[7],x[4],y[4],z[4],x[0],y[0],z[0]);
/* evaluate face six: nodes 4, 7, 6, 5 */
  SumElemFaceNormal((pfx + 4),(pfy + 4),(pfz + 4),(pfx + 7),(pfy + 7),(pfz + 7),(pfx + 6),(pfy + 6),(pfz + 6),(pfx + 5),(pfy + 5),(pfz + 5),x[4],y[4],z[4],x[7],y[7],z[7],x[6],y[6],z[6],x[5],y[5],z[5]);
}

inline static void SumElemStressesToNodeForces(const Real_t B[][8UL],const Real_t stress_xx,const Real_t stress_yy,const Real_t stress_zz,Real_t *const fx,Real_t *const fy,Real_t *const fz)
{
  Real_t pfx0 = B[0][0];
  Real_t pfx1 = B[0][1];
  Real_t pfx2 = B[0][2];
  Real_t pfx3 = B[0][3];
  Real_t pfx4 = B[0][4];
  Real_t pfx5 = B[0][5];
  Real_t pfx6 = B[0][6];
  Real_t pfx7 = B[0][7];
  Real_t pfy0 = B[1][0];
  Real_t pfy1 = B[1][1];
  Real_t pfy2 = B[1][2];
  Real_t pfy3 = B[1][3];
  Real_t pfy4 = B[1][4];
  Real_t pfy5 = B[1][5];
  Real_t pfy6 = B[1][6];
  Real_t pfy7 = B[1][7];
  Real_t pfz0 = B[2][0];
  Real_t pfz1 = B[2][1];
  Real_t pfz2 = B[2][2];
  Real_t pfz3 = B[2][3];
  Real_t pfz4 = B[2][4];
  Real_t pfz5 = B[2][5];
  Real_t pfz6 = B[2][6];
  Real_t pfz7 = B[2][7];
  fx[0] = -(stress_xx * pfx0);
  fx[1] = -(stress_xx * pfx1);
  fx[2] = -(stress_xx * pfx2);
  fx[3] = -(stress_xx * pfx3);
  fx[4] = -(stress_xx * pfx4);
  fx[5] = -(stress_xx * pfx5);
  fx[6] = -(stress_xx * pfx6);
  fx[7] = -(stress_xx * pfx7);
  fy[0] = -(stress_yy * pfy0);
  fy[1] = -(stress_yy * pfy1);
  fy[2] = -(stress_yy * pfy2);
  fy[3] = -(stress_yy * pfy3);
  fy[4] = -(stress_yy * pfy4);
  fy[5] = -(stress_yy * pfy5);
  fy[6] = -(stress_yy * pfy6);
  fy[7] = -(stress_yy * pfy7);
  fz[0] = -(stress_zz * pfz0);
  fz[1] = -(stress_zz * pfz1);
  fz[2] = -(stress_zz * pfz2);
  fz[3] = -(stress_zz * pfz3);
  fz[4] = -(stress_zz * pfz4);
  fz[5] = -(stress_zz * pfz5);
  fz[6] = -(stress_zz * pfz6);
  fz[7] = -(stress_zz * pfz7);
}

struct OUT__28__1527___data 
{
  void *fx_elem_p;
  void *fy_elem_p;
  void *fz_elem_p;
  void *numNode_p;
}
;
static void OUT__28__1527__(void *__out_argv);

struct OUT__29__1527___data 
{
  void *numElem_p;
  void *sigxx_p;
  void *sigyy_p;
  void *sigzz_p;
  void *determ_p;
  void *fx_elem_p;
  void *fy_elem_p;
  void *fz_elem_p;
}
;
static void OUT__29__1527__(void *__out_argv);

inline static void IntegrateStressForElems(Index_t numElem,Real_t *sigxx,Real_t *sigyy,Real_t *sigzz,Real_t *determ)
{
  Index_t numElem8 = (numElem * 8);
  Real_t *fx_elem = Allocate < Real_t > ((numElem8));
  Real_t *fy_elem = Allocate < Real_t > ((numElem8));
  Real_t *fz_elem = Allocate < Real_t > ((numElem8));
// loop over all elements
  Index_t k;
  struct OUT__29__1527___data __out_argv29__1527__;
  __out_argv29__1527__.OUT__29__1527___data::fz_elem_p = ((void *)(&fz_elem));
  __out_argv29__1527__.OUT__29__1527___data::fy_elem_p = ((void *)(&fy_elem));
  __out_argv29__1527__.OUT__29__1527___data::fx_elem_p = ((void *)(&fx_elem));
  __out_argv29__1527__.OUT__29__1527___data::determ_p = ((void *)(&determ));
  __out_argv29__1527__.OUT__29__1527___data::sigzz_p = ((void *)(&sigzz));
  __out_argv29__1527__.OUT__29__1527___data::sigyy_p = ((void *)(&sigyy));
  __out_argv29__1527__.OUT__29__1527___data::sigxx_p = ((void *)(&sigxx));
  __out_argv29__1527__.OUT__29__1527___data::numElem_p = ((void *)(&numElem));
  XOMP_parallel_start(OUT__29__1527__,&__out_argv29__1527__,1,0,"OUT__29__1527__");
  XOMP_parallel_end();
{
    Index_t numNode = domain. numNode ();
    Index_t gnode;
    struct OUT__28__1527___data __out_argv28__1527__;
    __out_argv28__1527__.OUT__28__1527___data::numNode_p = ((void *)(&numNode));
    __out_argv28__1527__.OUT__28__1527___data::fz_elem_p = ((void *)(&fz_elem));
    __out_argv28__1527__.OUT__28__1527___data::fy_elem_p = ((void *)(&fy_elem));
    __out_argv28__1527__.OUT__28__1527___data::fx_elem_p = ((void *)(&fx_elem));
    XOMP_parallel_start(OUT__28__1527__,&__out_argv28__1527__,1,0,"OUT__28__1527__");
    XOMP_parallel_end();
  }
  Release(&fz_elem);
  Release(&fy_elem);
  Release(&fx_elem);
}

inline static void CollectDomainNodesToElemNodes(const Index_t *elemToNode,Real_t (elemX)[8UL],Real_t elemY[8UL],Real_t elemZ[8UL])
{
  Index_t nd0i = elemToNode[0];
  Index_t nd1i = elemToNode[1];
  Index_t nd2i = elemToNode[2];
  Index_t nd3i = elemToNode[3];
  Index_t nd4i = elemToNode[4];
  Index_t nd5i = elemToNode[5];
  Index_t nd6i = elemToNode[6];
  Index_t nd7i = elemToNode[7];
  elemX[0] = domain. x (nd0i);
  elemX[1] = domain. x (nd1i);
  elemX[2] = domain. x (nd2i);
  elemX[3] = domain. x (nd3i);
  elemX[4] = domain. x (nd4i);
  elemX[5] = domain. x (nd5i);
  elemX[6] = domain. x (nd6i);
  elemX[7] = domain. x (nd7i);
  elemY[0] = domain. y (nd0i);
  elemY[1] = domain. y (nd1i);
  elemY[2] = domain. y (nd2i);
  elemY[3] = domain. y (nd3i);
  elemY[4] = domain. y (nd4i);
  elemY[5] = domain. y (nd5i);
  elemY[6] = domain. y (nd6i);
  elemY[7] = domain. y (nd7i);
  elemZ[0] = domain. z (nd0i);
  elemZ[1] = domain. z (nd1i);
  elemZ[2] = domain. z (nd2i);
  elemZ[3] = domain. z (nd3i);
  elemZ[4] = domain. z (nd4i);
  elemZ[5] = domain. z (nd5i);
  elemZ[6] = domain. z (nd6i);
  elemZ[7] = domain. z (nd7i);
}

inline static void VoluDer(const Real_t x0,const Real_t x1,const Real_t x2,const Real_t x3,const Real_t x4,const Real_t x5,const Real_t y0,const Real_t y1,const Real_t y2,const Real_t y3,const Real_t y4,const Real_t y5,const Real_t z0,const Real_t z1,const Real_t z2,const Real_t z3,const Real_t z4,const Real_t z5,Real_t *dvdx,Real_t *dvdy,Real_t *dvdz)
{
  const Real_t twelfth = (((((Real_t )1.0)) / (((Real_t )12.0))));
   *dvdx = (((((((y1 + y2) * (z0 + z1)) - ((y0 + y1) * (z1 + z2))) + ((y0 + y4) * (z3 + z4))) - ((y3 + y4) * (z0 + z4))) - ((y2 + y5) * (z3 + z5))) + ((y3 + y5) * (z2 + z5)));
   *dvdy = ((((((-(x1 + x2) * (z0 + z1)) + ((x0 + x1) * (z1 + z2))) - ((x0 + x4) * (z3 + z4))) + ((x3 + x4) * (z0 + z4))) + ((x2 + x5) * (z3 + z5))) - ((x3 + x5) * (z2 + z5)));
   *dvdz = ((((((-(y1 + y2) * (x0 + x1)) + ((y0 + y1) * (x1 + x2))) - ((y0 + y4) * (x3 + x4))) + ((y3 + y4) * (x0 + x4))) + ((y2 + y5) * (x3 + x5))) - ((y3 + y5) * (x2 + x5)));
   *dvdx *= twelfth;
   *dvdy *= twelfth;
   *dvdz *= twelfth;
}

inline static void CalcElemVolumeDerivative(Real_t dvdx[8UL],Real_t dvdy[8UL],Real_t dvdz[8UL],const Real_t x[8UL],const Real_t y[8UL],const Real_t z[8UL])
{
  VoluDer(x[1],x[2],x[3],x[4],x[5],x[7],y[1],y[2],y[3],y[4],y[5],y[7],z[1],z[2],z[3],z[4],z[5],z[7],(dvdx + 0),(dvdy + 0),(dvdz + 0));
  VoluDer(x[0],x[1],x[2],x[7],x[4],x[6],y[0],y[1],y[2],y[7],y[4],y[6],z[0],z[1],z[2],z[7],z[4],z[6],(dvdx + 3),(dvdy + 3),(dvdz + 3));
  VoluDer(x[3],x[0],x[1],x[6],x[7],x[5],y[3],y[0],y[1],y[6],y[7],y[5],z[3],z[0],z[1],z[6],z[7],z[5],(dvdx + 2),(dvdy + 2),(dvdz + 2));
  VoluDer(x[2],x[3],x[0],x[5],x[6],x[4],y[2],y[3],y[0],y[5],y[6],y[4],z[2],z[3],z[0],z[5],z[6],z[4],(dvdx + 1),(dvdy + 1),(dvdz + 1));
  VoluDer(x[7],x[6],x[5],x[0],x[3],x[1],y[7],y[6],y[5],y[0],y[3],y[1],z[7],z[6],z[5],z[0],z[3],z[1],(dvdx + 4),(dvdy + 4),(dvdz + 4));
  VoluDer(x[4],x[7],x[6],x[1],x[0],x[2],y[4],y[7],y[6],y[1],y[0],y[2],z[4],z[7],z[6],z[1],z[0],z[2],(dvdx + 5),(dvdy + 5),(dvdz + 5));
  VoluDer(x[5],x[4],x[7],x[2],x[1],x[3],y[5],y[4],y[7],y[2],y[1],y[3],z[5],z[4],z[7],z[2],z[1],z[3],(dvdx + 6),(dvdy + 6),(dvdz + 6));
  VoluDer(x[6],x[5],x[4],x[3],x[2],x[0],y[6],y[5],y[4],y[3],y[2],y[0],z[6],z[5],z[4],z[3],z[2],z[0],(dvdx + 7),(dvdy + 7),(dvdz + 7));
}

inline static void CalcElemFBHourglassForce(Real_t *xd,Real_t *yd,Real_t *zd,Real_t *hourgam0,Real_t *hourgam1,Real_t *hourgam2,Real_t *hourgam3,Real_t *hourgam4,Real_t *hourgam5,Real_t *hourgam6,Real_t *hourgam7,Real_t coefficient,Real_t *hgfx,Real_t *hgfy,Real_t *hgfz)
{
  Index_t i00 = (0);
  Index_t i01 = (1);
  Index_t i02 = (2);
  Index_t i03 = (3);
  Real_t h00 = ((((((((hourgam0[i00] * xd[0]) + (hourgam1[i00] * xd[1])) + (hourgam2[i00] * xd[2])) + (hourgam3[i00] * xd[3])) + (hourgam4[i00] * xd[4])) + (hourgam5[i00] * xd[5])) + (hourgam6[i00] * xd[6])) + (hourgam7[i00] * xd[7]));
  Real_t h01 = ((((((((hourgam0[i01] * xd[0]) + (hourgam1[i01] * xd[1])) + (hourgam2[i01] * xd[2])) + (hourgam3[i01] * xd[3])) + (hourgam4[i01] * xd[4])) + (hourgam5[i01] * xd[5])) + (hourgam6[i01] * xd[6])) + (hourgam7[i01] * xd[7]));
  Real_t h02 = ((((((((hourgam0[i02] * xd[0]) + (hourgam1[i02] * xd[1])) + (hourgam2[i02] * xd[2])) + (hourgam3[i02] * xd[3])) + (hourgam4[i02] * xd[4])) + (hourgam5[i02] * xd[5])) + (hourgam6[i02] * xd[6])) + (hourgam7[i02] * xd[7]));
  Real_t h03 = ((((((((hourgam0[i03] * xd[0]) + (hourgam1[i03] * xd[1])) + (hourgam2[i03] * xd[2])) + (hourgam3[i03] * xd[3])) + (hourgam4[i03] * xd[4])) + (hourgam5[i03] * xd[5])) + (hourgam6[i03] * xd[6])) + (hourgam7[i03] * xd[7]));
  hgfx[0] = (coefficient * ((((hourgam0[i00] * h00) + (hourgam0[i01] * h01)) + (hourgam0[i02] * h02)) + (hourgam0[i03] * h03)));
  hgfx[1] = (coefficient * ((((hourgam1[i00] * h00) + (hourgam1[i01] * h01)) + (hourgam1[i02] * h02)) + (hourgam1[i03] * h03)));
  hgfx[2] = (coefficient * ((((hourgam2[i00] * h00) + (hourgam2[i01] * h01)) + (hourgam2[i02] * h02)) + (hourgam2[i03] * h03)));
  hgfx[3] = (coefficient * ((((hourgam3[i00] * h00) + (hourgam3[i01] * h01)) + (hourgam3[i02] * h02)) + (hourgam3[i03] * h03)));
  hgfx[4] = (coefficient * ((((hourgam4[i00] * h00) + (hourgam4[i01] * h01)) + (hourgam4[i02] * h02)) + (hourgam4[i03] * h03)));
  hgfx[5] = (coefficient * ((((hourgam5[i00] * h00) + (hourgam5[i01] * h01)) + (hourgam5[i02] * h02)) + (hourgam5[i03] * h03)));
  hgfx[6] = (coefficient * ((((hourgam6[i00] * h00) + (hourgam6[i01] * h01)) + (hourgam6[i02] * h02)) + (hourgam6[i03] * h03)));
  hgfx[7] = (coefficient * ((((hourgam7[i00] * h00) + (hourgam7[i01] * h01)) + (hourgam7[i02] * h02)) + (hourgam7[i03] * h03)));
  h00 = ((((((((hourgam0[i00] * yd[0]) + (hourgam1[i00] * yd[1])) + (hourgam2[i00] * yd[2])) + (hourgam3[i00] * yd[3])) + (hourgam4[i00] * yd[4])) + (hourgam5[i00] * yd[5])) + (hourgam6[i00] * yd[6])) + (hourgam7[i00] * yd[7]));
  h01 = ((((((((hourgam0[i01] * yd[0]) + (hourgam1[i01] * yd[1])) + (hourgam2[i01] * yd[2])) + (hourgam3[i01] * yd[3])) + (hourgam4[i01] * yd[4])) + (hourgam5[i01] * yd[5])) + (hourgam6[i01] * yd[6])) + (hourgam7[i01] * yd[7]));
  h02 = ((((((((hourgam0[i02] * yd[0]) + (hourgam1[i02] * yd[1])) + (hourgam2[i02] * yd[2])) + (hourgam3[i02] * yd[3])) + (hourgam4[i02] * yd[4])) + (hourgam5[i02] * yd[5])) + (hourgam6[i02] * yd[6])) + (hourgam7[i02] * yd[7]));
  h03 = ((((((((hourgam0[i03] * yd[0]) + (hourgam1[i03] * yd[1])) + (hourgam2[i03] * yd[2])) + (hourgam3[i03] * yd[3])) + (hourgam4[i03] * yd[4])) + (hourgam5[i03] * yd[5])) + (hourgam6[i03] * yd[6])) + (hourgam7[i03] * yd[7]));
  hgfy[0] = (coefficient * ((((hourgam0[i00] * h00) + (hourgam0[i01] * h01)) + (hourgam0[i02] * h02)) + (hourgam0[i03] * h03)));
  hgfy[1] = (coefficient * ((((hourgam1[i00] * h00) + (hourgam1[i01] * h01)) + (hourgam1[i02] * h02)) + (hourgam1[i03] * h03)));
  hgfy[2] = (coefficient * ((((hourgam2[i00] * h00) + (hourgam2[i01] * h01)) + (hourgam2[i02] * h02)) + (hourgam2[i03] * h03)));
  hgfy[3] = (coefficient * ((((hourgam3[i00] * h00) + (hourgam3[i01] * h01)) + (hourgam3[i02] * h02)) + (hourgam3[i03] * h03)));
  hgfy[4] = (coefficient * ((((hourgam4[i00] * h00) + (hourgam4[i01] * h01)) + (hourgam4[i02] * h02)) + (hourgam4[i03] * h03)));
  hgfy[5] = (coefficient * ((((hourgam5[i00] * h00) + (hourgam5[i01] * h01)) + (hourgam5[i02] * h02)) + (hourgam5[i03] * h03)));
  hgfy[6] = (coefficient * ((((hourgam6[i00] * h00) + (hourgam6[i01] * h01)) + (hourgam6[i02] * h02)) + (hourgam6[i03] * h03)));
  hgfy[7] = (coefficient * ((((hourgam7[i00] * h00) + (hourgam7[i01] * h01)) + (hourgam7[i02] * h02)) + (hourgam7[i03] * h03)));
  h00 = ((((((((hourgam0[i00] * zd[0]) + (hourgam1[i00] * zd[1])) + (hourgam2[i00] * zd[2])) + (hourgam3[i00] * zd[3])) + (hourgam4[i00] * zd[4])) + (hourgam5[i00] * zd[5])) + (hourgam6[i00] * zd[6])) + (hourgam7[i00] * zd[7]));
  h01 = ((((((((hourgam0[i01] * zd[0]) + (hourgam1[i01] * zd[1])) + (hourgam2[i01] * zd[2])) + (hourgam3[i01] * zd[3])) + (hourgam4[i01] * zd[4])) + (hourgam5[i01] * zd[5])) + (hourgam6[i01] * zd[6])) + (hourgam7[i01] * zd[7]));
  h02 = ((((((((hourgam0[i02] * zd[0]) + (hourgam1[i02] * zd[1])) + (hourgam2[i02] * zd[2])) + (hourgam3[i02] * zd[3])) + (hourgam4[i02] * zd[4])) + (hourgam5[i02] * zd[5])) + (hourgam6[i02] * zd[6])) + (hourgam7[i02] * zd[7]));
  h03 = ((((((((hourgam0[i03] * zd[0]) + (hourgam1[i03] * zd[1])) + (hourgam2[i03] * zd[2])) + (hourgam3[i03] * zd[3])) + (hourgam4[i03] * zd[4])) + (hourgam5[i03] * zd[5])) + (hourgam6[i03] * zd[6])) + (hourgam7[i03] * zd[7]));
  hgfz[0] = (coefficient * ((((hourgam0[i00] * h00) + (hourgam0[i01] * h01)) + (hourgam0[i02] * h02)) + (hourgam0[i03] * h03)));
  hgfz[1] = (coefficient * ((((hourgam1[i00] * h00) + (hourgam1[i01] * h01)) + (hourgam1[i02] * h02)) + (hourgam1[i03] * h03)));
  hgfz[2] = (coefficient * ((((hourgam2[i00] * h00) + (hourgam2[i01] * h01)) + (hourgam2[i02] * h02)) + (hourgam2[i03] * h03)));
  hgfz[3] = (coefficient * ((((hourgam3[i00] * h00) + (hourgam3[i01] * h01)) + (hourgam3[i02] * h02)) + (hourgam3[i03] * h03)));
  hgfz[4] = (coefficient * ((((hourgam4[i00] * h00) + (hourgam4[i01] * h01)) + (hourgam4[i02] * h02)) + (hourgam4[i03] * h03)));
  hgfz[5] = (coefficient * ((((hourgam5[i00] * h00) + (hourgam5[i01] * h01)) + (hourgam5[i02] * h02)) + (hourgam5[i03] * h03)));
  hgfz[6] = (coefficient * ((((hourgam6[i00] * h00) + (hourgam6[i01] * h01)) + (hourgam6[i02] * h02)) + (hourgam6[i03] * h03)));
  hgfz[7] = (coefficient * ((((hourgam7[i00] * h00) + (hourgam7[i01] * h01)) + (hourgam7[i02] * h02)) + (hourgam7[i03] * h03)));
}

struct OUT__26__1527___data 
{
  void *fx_elem_p;
  void *fy_elem_p;
  void *fz_elem_p;
  void *numNode_p;
}
;
static void OUT__26__1527__(void *__out_argv);

struct OUT__27__1527___data 
{
  void *determ_p;
  void *x8n_p;
  void *y8n_p;
  void *z8n_p;
  void *dvdx_p;
  void *dvdy_p;
  void *dvdz_p;
  void *hourg_p;
  void *numElem_p;
  void *fx_elem_p;
  void *fy_elem_p;
  void *fz_elem_p;
  void *gamma_p;
}
;
static void OUT__27__1527__(void *__out_argv);

inline static void CalcFBHourglassForceForElems(Real_t *determ,Real_t *x8n,Real_t *y8n,Real_t *z8n,Real_t *dvdx,Real_t *dvdy,Real_t *dvdz,Real_t hourg)
{
  Index_t gnode_nom_1;
/*************************************************
    *
    *     FUNCTION: Calculates the Flanagan-Belytschko anti-hourglass
    *               force.
    *
    *************************************************/
  Index_t numElem = domain. numElem ();
  Index_t numElem8 = (numElem * 8);
  Real_t *fx_elem = Allocate < Real_t > ((numElem8));
  Real_t *fy_elem = Allocate < Real_t > ((numElem8));
  Real_t *fz_elem = Allocate < Real_t > ((numElem8));
  Real_t gamma[4UL][8UL];
  gamma[0][0] = ((Real_t )1.0);
  gamma[0][1] = ((Real_t )1.0);
  gamma[0][2] = ((Real_t )(-1.0));
  gamma[0][3] = ((Real_t )(-1.0));
  gamma[0][4] = ((Real_t )(-1.0));
  gamma[0][5] = ((Real_t )(-1.0));
  gamma[0][6] = ((Real_t )1.0);
  gamma[0][7] = ((Real_t )1.0);
  gamma[1][0] = ((Real_t )1.0);
  gamma[1][1] = ((Real_t )(-1.0));
  gamma[1][2] = ((Real_t )(-1.0));
  gamma[1][3] = ((Real_t )1.0);
  gamma[1][4] = ((Real_t )(-1.0));
  gamma[1][5] = ((Real_t )1.0);
  gamma[1][6] = ((Real_t )1.0);
  gamma[1][7] = ((Real_t )(-1.0));
  gamma[2][0] = ((Real_t )1.0);
  gamma[2][1] = ((Real_t )(-1.0));
  gamma[2][2] = ((Real_t )1.0);
  gamma[2][3] = ((Real_t )(-1.0));
  gamma[2][4] = ((Real_t )1.0);
  gamma[2][5] = ((Real_t )(-1.0));
  gamma[2][6] = ((Real_t )1.0);
  gamma[2][7] = ((Real_t )(-1.0));
  gamma[3][0] = ((Real_t )(-1.0));
  gamma[3][1] = ((Real_t )1.0);
  gamma[3][2] = ((Real_t )(-1.0));
  gamma[3][3] = ((Real_t )1.0);
  gamma[3][4] = ((Real_t )1.0);
  gamma[3][5] = ((Real_t )(-1.0));
  gamma[3][6] = ((Real_t )1.0);
  gamma[3][7] = ((Real_t )(-1.0));
/*************************************************/
/*    compute the hourglass modes */
  Index_t i2;
  struct OUT__27__1527___data __out_argv27__1527__;
  __out_argv27__1527__.OUT__27__1527___data::gamma_p = ((void *)(&gamma));
  __out_argv27__1527__.OUT__27__1527___data::fz_elem_p = ((void *)(&fz_elem));
  __out_argv27__1527__.OUT__27__1527___data::fy_elem_p = ((void *)(&fy_elem));
  __out_argv27__1527__.OUT__27__1527___data::fx_elem_p = ((void *)(&fx_elem));
  __out_argv27__1527__.OUT__27__1527___data::numElem_p = ((void *)(&numElem));
  __out_argv27__1527__.OUT__27__1527___data::hourg_p = ((void *)(&hourg));
  __out_argv27__1527__.OUT__27__1527___data::dvdz_p = ((void *)(&dvdz));
  __out_argv27__1527__.OUT__27__1527___data::dvdy_p = ((void *)(&dvdy));
  __out_argv27__1527__.OUT__27__1527___data::dvdx_p = ((void *)(&dvdx));
  __out_argv27__1527__.OUT__27__1527___data::z8n_p = ((void *)(&z8n));
  __out_argv27__1527__.OUT__27__1527___data::y8n_p = ((void *)(&y8n));
  __out_argv27__1527__.OUT__27__1527___data::x8n_p = ((void *)(&x8n));
  __out_argv27__1527__.OUT__27__1527___data::determ_p = ((void *)(&determ));
  XOMP_parallel_start(OUT__27__1527__,&__out_argv27__1527__,1,0,"OUT__27__1527__");
  XOMP_parallel_end();
{
    Index_t numNode = domain. numNode ();
    Index_t gnode;
    struct OUT__26__1527___data __out_argv26__1527__;
    __out_argv26__1527__.OUT__26__1527___data::numNode_p = ((void *)(&numNode));
    __out_argv26__1527__.OUT__26__1527___data::fz_elem_p = ((void *)(&fz_elem));
    __out_argv26__1527__.OUT__26__1527___data::fy_elem_p = ((void *)(&fy_elem));
    __out_argv26__1527__.OUT__26__1527___data::fx_elem_p = ((void *)(&fx_elem));
    XOMP_parallel_start(OUT__26__1527__,&__out_argv26__1527__,1,0,"OUT__26__1527__");
    XOMP_parallel_end();
  }
  Release(&fz_elem);
  Release(&fy_elem);
  Release(&fx_elem);
}

struct OUT__25__1527___data 
{
  void *determ_p;
  void *numElem_p;
  void *dvdx_p;
  void *dvdy_p;
  void *dvdz_p;
  void *x8n_p;
  void *y8n_p;
  void *z8n_p;
}
;
static void OUT__25__1527__(void *__out_argv);

inline static void CalcHourglassControlForElems(Real_t determ[],Real_t hgcoef)
{
  Index_t numElem = domain. numElem ();
  Index_t numElem8 = (numElem * 8);
  Real_t *dvdx = Allocate < Real_t > ((numElem8));
  Real_t *dvdy = Allocate < Real_t > ((numElem8));
  Real_t *dvdz = Allocate < Real_t > ((numElem8));
  Real_t *x8n = Allocate < Real_t > ((numElem8));
  Real_t *y8n = Allocate < Real_t > ((numElem8));
  Real_t *z8n = Allocate < Real_t > ((numElem8));
/* start loop over elements */
  Index_t i;
  struct OUT__25__1527___data __out_argv25__1527__;
  __out_argv25__1527__.OUT__25__1527___data::z8n_p = ((void *)(&z8n));
  __out_argv25__1527__.OUT__25__1527___data::y8n_p = ((void *)(&y8n));
  __out_argv25__1527__.OUT__25__1527___data::x8n_p = ((void *)(&x8n));
  __out_argv25__1527__.OUT__25__1527___data::dvdz_p = ((void *)(&dvdz));
  __out_argv25__1527__.OUT__25__1527___data::dvdy_p = ((void *)(&dvdy));
  __out_argv25__1527__.OUT__25__1527___data::dvdx_p = ((void *)(&dvdx));
  __out_argv25__1527__.OUT__25__1527___data::numElem_p = ((void *)(&numElem));
  __out_argv25__1527__.OUT__25__1527___data::determ_p = ((void *)(&determ));
  XOMP_parallel_start(OUT__25__1527__,&__out_argv25__1527__,1,0,"OUT__25__1527__");
  XOMP_parallel_end();
  if (hgcoef > (((Real_t )0.))) {
    CalcFBHourglassForceForElems(determ,x8n,y8n,z8n,dvdx,dvdy,dvdz,hgcoef);
  }
  Release(&z8n);
  Release(&y8n);
  Release(&x8n);
  Release(&dvdz);
  Release(&dvdy);
  Release(&dvdx);
}

struct OUT__24__1527___data 
{
  void *numElem_p;
  void *determ_p;
}
;
static void OUT__24__1527__(void *__out_argv);

inline static void CalcVolumeForceForElems()
{
  Index_t numElem = domain. numElem ();
  if (numElem != 0) {
    Real_t hgcoef = domain. hgcoef ();
    Real_t *sigxx = Allocate < Real_t > ((numElem));
    Real_t *sigyy = Allocate < Real_t > ((numElem));
    Real_t *sigzz = Allocate < Real_t > ((numElem));
    Real_t *determ = Allocate < Real_t > ((numElem));
/* Sum contributions to total stress tensor */
    InitStressTermsForElems(numElem,sigxx,sigyy,sigzz);
// call elemlib stress integration loop to produce nodal forces from
// material stresses.
    IntegrateStressForElems(numElem,sigxx,sigyy,sigzz,determ);
// check for negative element volume
    Index_t k;
    struct OUT__24__1527___data __out_argv24__1527__;
    __out_argv24__1527__.OUT__24__1527___data::determ_p = ((void *)(&determ));
    __out_argv24__1527__.OUT__24__1527___data::numElem_p = ((void *)(&numElem));
    XOMP_parallel_start(OUT__24__1527__,&__out_argv24__1527__,1,0,"OUT__24__1527__");
    XOMP_parallel_end();
    CalcHourglassControlForElems(determ,hgcoef);
    Release(&determ);
    Release(&sigzz);
    Release(&sigyy);
    Release(&sigxx);
  }
}

struct OUT__23__1527___data 
{
  void *numNode_p;
}
;
static void OUT__23__1527__(void *__out_argv);

inline static void CalcForceForNodes()
{
  Index_t numNode = domain. numNode ();
  Index_t i;
  struct OUT__23__1527___data __out_argv23__1527__;
  __out_argv23__1527__.OUT__23__1527___data::numNode_p = ((void *)(&numNode));
  XOMP_parallel_start(OUT__23__1527__,&__out_argv23__1527__,1,0,"OUT__23__1527__");
  XOMP_parallel_end();
/* Calcforce calls partial, force, hourq */
  CalcVolumeForceForElems();
/* Calculate Nodal Forces at domain boundaries */
/* problem->commSBN->Transfer(CommSBN::forces); */
}

struct OUT__22__1527___data 
{
  void *numNode_p;
}
;
static void OUT__22__1527__(void *__out_argv);

inline static void CalcAccelerationForNodes()
{
  Index_t numNode = domain. numNode ();
  Index_t i;
  struct OUT__22__1527___data __out_argv22__1527__;
  __out_argv22__1527__.OUT__22__1527___data::numNode_p = ((void *)(&numNode));
  XOMP_parallel_start(OUT__22__1527__,&__out_argv22__1527__,1,0,"OUT__22__1527__");
  XOMP_parallel_end();
}

struct OUT__21__1527___data 
{
  void *numNodeBC_p;
}
;
static void OUT__21__1527__(void *__out_argv);

inline static void ApplyAccelerationBoundaryConditionsForNodes()
{
  Index_t numNodeBC = ((domain. sizeX () + 1) * (domain. sizeX () + 1));
  struct OUT__21__1527___data __out_argv21__1527__;
  __out_argv21__1527__.OUT__21__1527___data::numNodeBC_p = ((void *)(&numNodeBC));
  XOMP_parallel_start(OUT__21__1527__,&__out_argv21__1527__,1,0,"OUT__21__1527__");
  XOMP_parallel_end();
}

struct OUT__20__1527___data 
{
  void *dt_p;
  void *u_cut_p;
  void *numNode_p;
}
;
static void OUT__20__1527__(void *__out_argv);

inline static void CalcVelocityForNodes(const Real_t dt,const Real_t u_cut)
{
  Index_t numNode = domain. numNode ();
  Index_t i;
  struct OUT__20__1527___data __out_argv20__1527__;
  __out_argv20__1527__.OUT__20__1527___data::numNode_p = ((void *)(&numNode));
  __out_argv20__1527__.OUT__20__1527___data::u_cut_p = ((void *)(&u_cut));
  __out_argv20__1527__.OUT__20__1527___data::dt_p = ((void *)(&dt));
  XOMP_parallel_start(OUT__20__1527__,&__out_argv20__1527__,1,0,"OUT__20__1527__");
  XOMP_parallel_end();
}

struct OUT__19__1527___data 
{
  void *dt_p;
  void *numNode_p;
}
;
static void OUT__19__1527__(void *__out_argv);

inline static void CalcPositionForNodes(const Real_t dt)
{
  Index_t numNode = domain. numNode ();
  Index_t i;
  struct OUT__19__1527___data __out_argv19__1527__;
  __out_argv19__1527__.OUT__19__1527___data::numNode_p = ((void *)(&numNode));
  __out_argv19__1527__.OUT__19__1527___data::dt_p = ((void *)(&dt));
  XOMP_parallel_start(OUT__19__1527__,&__out_argv19__1527__,1,0,"OUT__19__1527__");
  XOMP_parallel_end();
}

inline static void LagrangeNodal()
{
  const Real_t delt = domain. deltatime ();
  Real_t u_cut = domain. u_cut ();
/* time of boundary condition evaluation is beginning of step for force and
   * acceleration boundary conditions. */
  CalcForceForNodes();
  CalcAccelerationForNodes();
  ApplyAccelerationBoundaryConditionsForNodes();
  CalcVelocityForNodes(delt,u_cut);
  CalcPositionForNodes(delt);
}

inline static Real_t CalcElemVolume(const Real_t x0,const Real_t x1,const Real_t x2,const Real_t x3,const Real_t x4,const Real_t x5,const Real_t x6,const Real_t x7,const Real_t y0,const Real_t y1,const Real_t y2,const Real_t y3,const Real_t y4,const Real_t y5,const Real_t y6,const Real_t y7,const Real_t z0,const Real_t z1,const Real_t z2,const Real_t z3,const Real_t z4,const Real_t z5,const Real_t z6,const Real_t z7)
{
  Real_t twelveth = (((((Real_t )1.0)) / (((Real_t )12.0))));
  Real_t dx61 = (x6 - x1);
  Real_t dy61 = (y6 - y1);
  Real_t dz61 = (z6 - z1);
  Real_t dx70 = (x7 - x0);
  Real_t dy70 = (y7 - y0);
  Real_t dz70 = (z7 - z0);
  Real_t dx63 = (x6 - x3);
  Real_t dy63 = (y6 - y3);
  Real_t dz63 = (z6 - z3);
  Real_t dx20 = (x2 - x0);
  Real_t dy20 = (y2 - y0);
  Real_t dz20 = (z2 - z0);
  Real_t dx50 = (x5 - x0);
  Real_t dy50 = (y5 - y0);
  Real_t dz50 = (z5 - z0);
  Real_t dx64 = (x6 - x4);
  Real_t dy64 = (y6 - y4);
  Real_t dz64 = (z6 - z4);
  Real_t dx31 = (x3 - x1);
  Real_t dy31 = (y3 - y1);
  Real_t dz31 = (z3 - z1);
  Real_t dx72 = (x7 - x2);
  Real_t dy72 = (y7 - y2);
  Real_t dz72 = (z7 - z2);
  Real_t dx43 = (x4 - x3);
  Real_t dy43 = (y4 - y3);
  Real_t dz43 = (z4 - z3);
  Real_t dx57 = (x5 - x7);
  Real_t dy57 = (y5 - y7);
  Real_t dz57 = (z5 - z7);
  Real_t dx14 = (x1 - x4);
  Real_t dy14 = (y1 - y4);
  Real_t dz14 = (z1 - z4);
  Real_t dx25 = (x2 - x5);
  Real_t dy25 = (y2 - y5);
  Real_t dz25 = (z2 - z5);
#define TRIPLE_PRODUCT(x1, y1, z1, x2, y2, z2, x3, y3, z3) ((x1)*((y2)*(z3) - (z2)*(y3)) + (x2)*((z1)*(y3) - (y1)*(z3)) + (x3)*((y1)*(z2) - (z1)*(y2)))
  Real_t volume = ((((((dx31 + dx72) * ((dy63 * dz20) - (dy20 * dz63))) + ((dy31 + dy72) * ((dx20 * dz63) - (dx63 * dz20)))) + ((dz31 + dz72) * ((dx63 * dy20) - (dx20 * dy63)))) + ((((dx43 + dx57) * ((dy64 * dz70) - (dy70 * dz64))) + ((dy43 + dy57) * ((dx70 * dz64) - (dx64 * dz70)))) + ((dz43 + dz57) * ((dx64 * dy70) - (dx70 * dy64))))) + ((((dx14 + dx25) * ((dy61 * dz50) - (dy50 * dz61))) + ((dy14 + dy25) * ((dx50 * dz61) - (dx61 * dz50)))) + ((dz14 + dz25) * ((dx61 * dy50) - (dx50 * dy61)))));
#undef TRIPLE_PRODUCT
  volume *= twelveth;
  return volume;
}

inline static Real_t CalcElemVolume(const Real_t x[8UL],const Real_t y[8UL],const Real_t z[8UL])
{
  return CalcElemVolume(x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],y[0],y[1],y[2],y[3],y[4],y[5],y[6],y[7],z[0],z[1],z[2],z[3],z[4],z[5],z[6],z[7]);
}

inline static Real_t AreaFace(const Real_t x0,const Real_t x1,const Real_t x2,const Real_t x3,const Real_t y0,const Real_t y1,const Real_t y2,const Real_t y3,const Real_t z0,const Real_t z1,const Real_t z2,const Real_t z3)
{
  Real_t fx = ((x2 - x0) - (x3 - x1));
  Real_t fy = ((y2 - y0) - (y3 - y1));
  Real_t fz = ((z2 - z0) - (z3 - z1));
  Real_t gx = ((x2 - x0) + (x3 - x1));
  Real_t gy = ((y2 - y0) + (y3 - y1));
  Real_t gz = ((z2 - z0) + (z3 - z1));
  Real_t area = (((((fx * fx) + (fy * fy)) + (fz * fz)) * (((gx * gx) + (gy * gy)) + (gz * gz))) - ((((fx * gx) + (fy * gy)) + (fz * gz)) * (((fx * gx) + (fy * gy)) + (fz * gz))));
  return area;
}

inline static Real_t CalcElemCharacteristicLength(const Real_t x[8UL],const Real_t y[8UL],const Real_t z[8UL],const Real_t volume)
{
  Real_t a;
  Real_t charLength = ((Real_t )0.);
  a = AreaFace(x[0],x[1],x[2],x[3],y[0],y[1],y[2],y[3],z[0],z[1],z[2],z[3]);
  charLength = std::max(a,charLength);
  a = AreaFace(x[4],x[5],x[6],x[7],y[4],y[5],y[6],y[7],z[4],z[5],z[6],z[7]);
  charLength = std::max(a,charLength);
  a = AreaFace(x[0],x[1],x[5],x[4],y[0],y[1],y[5],y[4],z[0],z[1],z[5],z[4]);
  charLength = std::max(a,charLength);
  a = AreaFace(x[1],x[2],x[6],x[5],y[1],y[2],y[6],y[5],z[1],z[2],z[6],z[5]);
  charLength = std::max(a,charLength);
  a = AreaFace(x[2],x[3],x[7],x[6],y[2],y[3],y[7],y[6],z[2],z[3],z[7],z[6]);
  charLength = std::max(a,charLength);
  a = AreaFace(x[3],x[0],x[4],x[7],y[3],y[0],y[4],y[7],z[3],z[0],z[4],z[7]);
  charLength = std::max(a,charLength);
  charLength = (((((Real_t )4.0)) * volume) / SQRT(charLength));
  return charLength;
}

inline static void CalcElemVelocityGrandient(const Real_t *const xvel,const Real_t *const yvel,const Real_t *const zvel,const Real_t (b)[][8UL],const Real_t detJ,Real_t *const d)
{
  const Real_t inv_detJ = ((((Real_t )1.0)) / detJ);
  Real_t dyddx;
  Real_t dxddy;
  Real_t dzddx;
  Real_t dxddz;
  Real_t dzddy;
  Real_t dyddz;
  const Real_t *const pfx = b[0];
  const Real_t *const pfy = b[1];
  const Real_t *const pfz = b[2];
  d[0] = (inv_detJ * ((((pfx[0] * (xvel[0] - xvel[6])) + (pfx[1] * (xvel[1] - xvel[7]))) + (pfx[2] * (xvel[2] - xvel[4]))) + (pfx[3] * (xvel[3] - xvel[5]))));
  d[1] = (inv_detJ * ((((pfy[0] * (yvel[0] - yvel[6])) + (pfy[1] * (yvel[1] - yvel[7]))) + (pfy[2] * (yvel[2] - yvel[4]))) + (pfy[3] * (yvel[3] - yvel[5]))));
  d[2] = (inv_detJ * ((((pfz[0] * (zvel[0] - zvel[6])) + (pfz[1] * (zvel[1] - zvel[7]))) + (pfz[2] * (zvel[2] - zvel[4]))) + (pfz[3] * (zvel[3] - zvel[5]))));
  dyddx = (inv_detJ * ((((pfx[0] * (yvel[0] - yvel[6])) + (pfx[1] * (yvel[1] - yvel[7]))) + (pfx[2] * (yvel[2] - yvel[4]))) + (pfx[3] * (yvel[3] - yvel[5]))));
  dxddy = (inv_detJ * ((((pfy[0] * (xvel[0] - xvel[6])) + (pfy[1] * (xvel[1] - xvel[7]))) + (pfy[2] * (xvel[2] - xvel[4]))) + (pfy[3] * (xvel[3] - xvel[5]))));
  dzddx = (inv_detJ * ((((pfx[0] * (zvel[0] - zvel[6])) + (pfx[1] * (zvel[1] - zvel[7]))) + (pfx[2] * (zvel[2] - zvel[4]))) + (pfx[3] * (zvel[3] - zvel[5]))));
  dxddz = (inv_detJ * ((((pfz[0] * (xvel[0] - xvel[6])) + (pfz[1] * (xvel[1] - xvel[7]))) + (pfz[2] * (xvel[2] - xvel[4]))) + (pfz[3] * (xvel[3] - xvel[5]))));
  dzddy = (inv_detJ * ((((pfy[0] * (zvel[0] - zvel[6])) + (pfy[1] * (zvel[1] - zvel[7]))) + (pfy[2] * (zvel[2] - zvel[4]))) + (pfy[3] * (zvel[3] - zvel[5]))));
  dyddz = (inv_detJ * ((((pfz[0] * (yvel[0] - yvel[6])) + (pfz[1] * (yvel[1] - yvel[7]))) + (pfz[2] * (yvel[2] - yvel[4]))) + (pfz[3] * (yvel[3] - yvel[5]))));
  d[5] = ((((Real_t )0.5)) * (dxddy + dyddx));
  d[4] = ((((Real_t )0.5)) * (dxddz + dzddx));
  d[3] = ((((Real_t )0.5)) * (dzddy + dyddz));
}

struct OUT__18__1527___data 
{
  void *numElem_p;
  void *dt_p;
}
;
static void OUT__18__1527__(void *__out_argv);

inline static void CalcKinematicsForElems(Index_t numElem,Real_t dt)
{
// loop over all elements
  Index_t k;
  struct OUT__18__1527___data __out_argv18__1527__;
  __out_argv18__1527__.OUT__18__1527___data::dt_p = ((void *)(&dt));
  __out_argv18__1527__.OUT__18__1527___data::numElem_p = ((void *)(&numElem));
  XOMP_parallel_start(OUT__18__1527__,&__out_argv18__1527__,1,0,"OUT__18__1527__");
  XOMP_parallel_end();
}

struct OUT__17__1527___data 
{
  void *numElem_p;
}
;
static void OUT__17__1527__(void *__out_argv);

inline static void CalcLagrangeElements(Real_t deltatime)
{
  Index_t numElem = domain. numElem ();
  if (numElem > 0) {
    CalcKinematicsForElems(numElem,deltatime);
// element loop to do some stuff not included in the elemlib function.
    Index_t k;
    struct OUT__17__1527___data __out_argv17__1527__;
    __out_argv17__1527__.OUT__17__1527___data::numElem_p = ((void *)(&numElem));
    XOMP_parallel_start(OUT__17__1527__,&__out_argv17__1527__,1,0,"OUT__17__1527__");
    XOMP_parallel_end();
  }
}

struct OUT__16__1527___data 
{
  void *numElem_p;
}
;
static void OUT__16__1527__(void *__out_argv);

inline static void CalcMonotonicQGradientsForElems()
{
#define SUM4(a,b,c,d) (a + b + c + d)
  Index_t numElem = domain. numElem ();
  Index_t i;
  struct OUT__16__1527___data __out_argv16__1527__;
  __out_argv16__1527__.OUT__16__1527___data::numElem_p = ((void *)(&numElem));
  XOMP_parallel_start(OUT__16__1527__,&__out_argv16__1527__,1,0,"OUT__16__1527__");
  XOMP_parallel_end();
#undef SUM4
}

struct OUT__15__1527___data 
{
  void *qlc_monoq_p;
  void *qqc_monoq_p;
  void *monoq_limiter_mult_p;
  void *monoq_max_slope_p;
  void *ptiny_p;
  void *elength_p;
}
;
static void OUT__15__1527__(void *__out_argv);

inline static void CalcMonotonicQRegionForElems(
// parameters
Real_t qlc_monoq,Real_t qqc_monoq,Real_t monoq_limiter_mult,Real_t monoq_max_slope,Real_t ptiny,
// the elementset length
Index_t elength)
{
  Index_t ielem;
  struct OUT__15__1527___data __out_argv15__1527__;
  __out_argv15__1527__.OUT__15__1527___data::elength_p = ((void *)(&elength));
  __out_argv15__1527__.OUT__15__1527___data::ptiny_p = ((void *)(&ptiny));
  __out_argv15__1527__.OUT__15__1527___data::monoq_max_slope_p = ((void *)(&monoq_max_slope));
  __out_argv15__1527__.OUT__15__1527___data::monoq_limiter_mult_p = ((void *)(&monoq_limiter_mult));
  __out_argv15__1527__.OUT__15__1527___data::qqc_monoq_p = ((void *)(&qqc_monoq));
  __out_argv15__1527__.OUT__15__1527___data::qlc_monoq_p = ((void *)(&qlc_monoq));
  XOMP_parallel_start(OUT__15__1527__,&__out_argv15__1527__,1,0,"OUT__15__1527__");
  XOMP_parallel_end();
}

inline static void CalcMonotonicQForElems()
{
//
// initialize parameters
// 
  const Real_t ptiny = ((Real_t )1.e-36);
  Real_t monoq_max_slope = domain. monoq_max_slope ();
  Real_t monoq_limiter_mult = domain. monoq_limiter_mult ();
//
// calculate the monotonic q for pure regions
//
  Index_t elength = domain. numElem ();
  if (elength > 0) {
    Real_t qlc_monoq = domain. qlc_monoq ();
    Real_t qqc_monoq = domain. qqc_monoq ();
// parameters
    CalcMonotonicQRegionForElems(qlc_monoq,qqc_monoq,monoq_limiter_mult,monoq_max_slope,ptiny,elength);
// the elemset length
  }
}

inline static void CalcQForElems()
{
  Real_t qstop = domain. qstop ();
  Index_t numElem = domain. numElem ();
//
// MONOTONIC Q option
//
/* Calculate velocity gradients */
  CalcMonotonicQGradientsForElems();
/* Transfer veloctiy gradients in the first order elements */
/* problem->commElements->Transfer(CommElements::monoQ) ; */
  CalcMonotonicQForElems();
/* Don't allow excessive artificial viscosity */
  if (numElem != 0) {
    Index_t idx = ((-1));
    for (Index_t i = (0); i < numElem; ++i) {
      if (domain. q (i) > qstop) {
        idx = i;
        break; 
      }
    }
    if (idx >= 0) {
      exit((QStopError));
    }
  }
}

struct OUT__13__1527___data 
{
  void *p_new_p;
  void *bvc_p;
  void *e_old_p;
  void *vnewc_p;
  void *pmin_p;
  void *p_cut_p;
  void *eosvmax_p;
  void *length_p;
}
;
static void OUT__13__1527__(void *__out_argv);

struct OUT__14__1527___data 
{
  void *bvc_p;
  void *pbvc_p;
  void *compression_p;
  void *length_p;
}
;
static void OUT__14__1527__(void *__out_argv);

inline static void CalcPressureForElems(Real_t *p_new,Real_t *bvc,Real_t *pbvc,Real_t *e_old,Real_t *compression,Real_t *vnewc,Real_t pmin,Real_t p_cut,Real_t eosvmax,Index_t length)
{
  Index_t i;
  struct OUT__14__1527___data __out_argv14__1527__;
  __out_argv14__1527__.OUT__14__1527___data::length_p = ((void *)(&length));
  __out_argv14__1527__.OUT__14__1527___data::compression_p = ((void *)(&compression));
  __out_argv14__1527__.OUT__14__1527___data::pbvc_p = ((void *)(&pbvc));
  __out_argv14__1527__.OUT__14__1527___data::bvc_p = ((void *)(&bvc));
  XOMP_parallel_start(OUT__14__1527__,&__out_argv14__1527__,1,0,"OUT__14__1527__");
  XOMP_parallel_end();
  struct OUT__13__1527___data __out_argv13__1527__;
  __out_argv13__1527__.OUT__13__1527___data::length_p = ((void *)(&length));
  __out_argv13__1527__.OUT__13__1527___data::eosvmax_p = ((void *)(&eosvmax));
  __out_argv13__1527__.OUT__13__1527___data::p_cut_p = ((void *)(&p_cut));
  __out_argv13__1527__.OUT__13__1527___data::pmin_p = ((void *)(&pmin));
  __out_argv13__1527__.OUT__13__1527___data::vnewc_p = ((void *)(&vnewc));
  __out_argv13__1527__.OUT__13__1527___data::e_old_p = ((void *)(&e_old));
  __out_argv13__1527__.OUT__13__1527___data::bvc_p = ((void *)(&bvc));
  __out_argv13__1527__.OUT__13__1527___data::p_new_p = ((void *)(&p_new));
  XOMP_parallel_start(OUT__13__1527__,&__out_argv13__1527__,1,0,"OUT__13__1527__");
  XOMP_parallel_end();
}

struct OUT__8__1527___data 
{
  void *p_new_p;
  void *e_new_p;
  void *q_new_p;
  void *bvc_p;
  void *pbvc_p;
  void *vnewc_p;
  void *delvc_p;
  void *q_cut_p;
  void *qq_p;
  void *ql_p;
  void *rho0_p;
  void *length_p;
}
;
static void OUT__8__1527__(void *__out_argv);

struct OUT__9__1527___data 
{
  void *p_new_p;
  void *e_new_p;
  void *q_new_p;
  void *bvc_p;
  void *pbvc_p;
  void *p_old_p;
  void *q_old_p;
  void *vnewc_p;
  void *delvc_p;
  void *e_cut_p;
  void *emin_p;
  void *qq_p;
  void *ql_p;
  void *rho0_p;
  void *length_p;
  void *pHalfStep_p;
}
;
static void OUT__9__1527__(void *__out_argv);

struct OUT__10__1527___data 
{
  void *e_new_p;
  void *work_p;
  void *e_cut_p;
  void *emin_p;
  void *length_p;
}
;
static void OUT__10__1527__(void *__out_argv);

struct OUT__11__1527___data 
{
  void *e_new_p;
  void *q_new_p;
  void *bvc_p;
  void *pbvc_p;
  void *p_old_p;
  void *q_old_p;
  void *compHalfStep_p;
  void *delvc_p;
  void *qq_p;
  void *ql_p;
  void *rho0_p;
  void *length_p;
  void *pHalfStep_p;
}
;
static void OUT__11__1527__(void *__out_argv);

struct OUT__12__1527___data 
{
  void *e_new_p;
  void *p_old_p;
  void *e_old_p;
  void *q_old_p;
  void *work_p;
  void *delvc_p;
  void *emin_p;
  void *length_p;
}
;
static void OUT__12__1527__(void *__out_argv);

inline static void CalcEnergyForElems(Real_t *p_new,Real_t *e_new,Real_t *q_new,Real_t *bvc,Real_t *pbvc,Real_t *p_old,Real_t *e_old,Real_t *q_old,Real_t *compression,Real_t *compHalfStep,Real_t *vnewc,Real_t *work,Real_t *delvc,Real_t pmin,Real_t p_cut,Real_t e_cut,Real_t q_cut,Real_t emin,Real_t *qq,Real_t *ql,Real_t rho0,Real_t eosvmax,Index_t length)
{
  Real_t *pHalfStep = Allocate < Real_t > ((length));
  Index_t i;
  struct OUT__12__1527___data __out_argv12__1527__;
  __out_argv12__1527__.OUT__12__1527___data::length_p = ((void *)(&length));
  __out_argv12__1527__.OUT__12__1527___data::emin_p = ((void *)(&emin));
  __out_argv12__1527__.OUT__12__1527___data::delvc_p = ((void *)(&delvc));
  __out_argv12__1527__.OUT__12__1527___data::work_p = ((void *)(&work));
  __out_argv12__1527__.OUT__12__1527___data::q_old_p = ((void *)(&q_old));
  __out_argv12__1527__.OUT__12__1527___data::e_old_p = ((void *)(&e_old));
  __out_argv12__1527__.OUT__12__1527___data::p_old_p = ((void *)(&p_old));
  __out_argv12__1527__.OUT__12__1527___data::e_new_p = ((void *)(&e_new));
  XOMP_parallel_start(OUT__12__1527__,&__out_argv12__1527__,1,0,"OUT__12__1527__");
  XOMP_parallel_end();
  CalcPressureForElems(pHalfStep,bvc,pbvc,e_new,compHalfStep,vnewc,pmin,p_cut,eosvmax,length);
  struct OUT__11__1527___data __out_argv11__1527__;
  __out_argv11__1527__.OUT__11__1527___data::pHalfStep_p = ((void *)(&pHalfStep));
  __out_argv11__1527__.OUT__11__1527___data::length_p = ((void *)(&length));
  __out_argv11__1527__.OUT__11__1527___data::rho0_p = ((void *)(&rho0));
  __out_argv11__1527__.OUT__11__1527___data::ql_p = ((void *)(&ql));
  __out_argv11__1527__.OUT__11__1527___data::qq_p = ((void *)(&qq));
  __out_argv11__1527__.OUT__11__1527___data::delvc_p = ((void *)(&delvc));
  __out_argv11__1527__.OUT__11__1527___data::compHalfStep_p = ((void *)(&compHalfStep));
  __out_argv11__1527__.OUT__11__1527___data::q_old_p = ((void *)(&q_old));
  __out_argv11__1527__.OUT__11__1527___data::p_old_p = ((void *)(&p_old));
  __out_argv11__1527__.OUT__11__1527___data::pbvc_p = ((void *)(&pbvc));
  __out_argv11__1527__.OUT__11__1527___data::bvc_p = ((void *)(&bvc));
  __out_argv11__1527__.OUT__11__1527___data::q_new_p = ((void *)(&q_new));
  __out_argv11__1527__.OUT__11__1527___data::e_new_p = ((void *)(&e_new));
  XOMP_parallel_start(OUT__11__1527__,&__out_argv11__1527__,1,0,"OUT__11__1527__");
  XOMP_parallel_end();
  struct OUT__10__1527___data __out_argv10__1527__;
  __out_argv10__1527__.OUT__10__1527___data::length_p = ((void *)(&length));
  __out_argv10__1527__.OUT__10__1527___data::emin_p = ((void *)(&emin));
  __out_argv10__1527__.OUT__10__1527___data::e_cut_p = ((void *)(&e_cut));
  __out_argv10__1527__.OUT__10__1527___data::work_p = ((void *)(&work));
  __out_argv10__1527__.OUT__10__1527___data::e_new_p = ((void *)(&e_new));
  XOMP_parallel_start(OUT__10__1527__,&__out_argv10__1527__,1,0,"OUT__10__1527__");
  XOMP_parallel_end();
  CalcPressureForElems(p_new,bvc,pbvc,e_new,compression,vnewc,pmin,p_cut,eosvmax,length);
  struct OUT__9__1527___data __out_argv9__1527__;
  __out_argv9__1527__.OUT__9__1527___data::pHalfStep_p = ((void *)(&pHalfStep));
  __out_argv9__1527__.OUT__9__1527___data::length_p = ((void *)(&length));
  __out_argv9__1527__.OUT__9__1527___data::rho0_p = ((void *)(&rho0));
  __out_argv9__1527__.OUT__9__1527___data::ql_p = ((void *)(&ql));
  __out_argv9__1527__.OUT__9__1527___data::qq_p = ((void *)(&qq));
  __out_argv9__1527__.OUT__9__1527___data::emin_p = ((void *)(&emin));
  __out_argv9__1527__.OUT__9__1527___data::e_cut_p = ((void *)(&e_cut));
  __out_argv9__1527__.OUT__9__1527___data::delvc_p = ((void *)(&delvc));
  __out_argv9__1527__.OUT__9__1527___data::vnewc_p = ((void *)(&vnewc));
  __out_argv9__1527__.OUT__9__1527___data::q_old_p = ((void *)(&q_old));
  __out_argv9__1527__.OUT__9__1527___data::p_old_p = ((void *)(&p_old));
  __out_argv9__1527__.OUT__9__1527___data::pbvc_p = ((void *)(&pbvc));
  __out_argv9__1527__.OUT__9__1527___data::bvc_p = ((void *)(&bvc));
  __out_argv9__1527__.OUT__9__1527___data::q_new_p = ((void *)(&q_new));
  __out_argv9__1527__.OUT__9__1527___data::e_new_p = ((void *)(&e_new));
  __out_argv9__1527__.OUT__9__1527___data::p_new_p = ((void *)(&p_new));
  XOMP_parallel_start(OUT__9__1527__,&__out_argv9__1527__,1,0,"OUT__9__1527__");
  XOMP_parallel_end();
  CalcPressureForElems(p_new,bvc,pbvc,e_new,compression,vnewc,pmin,p_cut,eosvmax,length);
  struct OUT__8__1527___data __out_argv8__1527__;
  __out_argv8__1527__.OUT__8__1527___data::length_p = ((void *)(&length));
  __out_argv8__1527__.OUT__8__1527___data::rho0_p = ((void *)(&rho0));
  __out_argv8__1527__.OUT__8__1527___data::ql_p = ((void *)(&ql));
  __out_argv8__1527__.OUT__8__1527___data::qq_p = ((void *)(&qq));
  __out_argv8__1527__.OUT__8__1527___data::q_cut_p = ((void *)(&q_cut));
  __out_argv8__1527__.OUT__8__1527___data::delvc_p = ((void *)(&delvc));
  __out_argv8__1527__.OUT__8__1527___data::vnewc_p = ((void *)(&vnewc));
  __out_argv8__1527__.OUT__8__1527___data::pbvc_p = ((void *)(&pbvc));
  __out_argv8__1527__.OUT__8__1527___data::bvc_p = ((void *)(&bvc));
  __out_argv8__1527__.OUT__8__1527___data::q_new_p = ((void *)(&q_new));
  __out_argv8__1527__.OUT__8__1527___data::e_new_p = ((void *)(&e_new));
  __out_argv8__1527__.OUT__8__1527___data::p_new_p = ((void *)(&p_new));
  XOMP_parallel_start(OUT__8__1527__,&__out_argv8__1527__,1,0,"OUT__8__1527__");
  XOMP_parallel_end();
  Release(&pHalfStep);
}

struct OUT__7__1527___data 
{
  void *vnewc_p;
  void *rho0_p;
  void *enewc_p;
  void *pnewc_p;
  void *pbvc_p;
  void *bvc_p;
  void *ss4o3_p;
  void *nz_p;
}
;
static void OUT__7__1527__(void *__out_argv);

inline static void CalcSoundSpeedForElems(Real_t *vnewc,Real_t rho0,Real_t *enewc,Real_t *pnewc,Real_t *pbvc,Real_t *bvc,Real_t ss4o3,Index_t nz)
{
  Index_t i;
  struct OUT__7__1527___data __out_argv7__1527__;
  __out_argv7__1527__.OUT__7__1527___data::nz_p = ((void *)(&nz));
  __out_argv7__1527__.OUT__7__1527___data::ss4o3_p = ((void *)(&ss4o3));
  __out_argv7__1527__.OUT__7__1527___data::bvc_p = ((void *)(&bvc));
  __out_argv7__1527__.OUT__7__1527___data::pbvc_p = ((void *)(&pbvc));
  __out_argv7__1527__.OUT__7__1527___data::pnewc_p = ((void *)(&pnewc));
  __out_argv7__1527__.OUT__7__1527___data::enewc_p = ((void *)(&enewc));
  __out_argv7__1527__.OUT__7__1527___data::rho0_p = ((void *)(&rho0));
  __out_argv7__1527__.OUT__7__1527___data::vnewc_p = ((void *)(&vnewc));
  XOMP_parallel_start(OUT__7__1527__,&__out_argv7__1527__,1,0,"OUT__7__1527__");
  XOMP_parallel_end();
}

struct OUT__5__1527___data 
{
  void *length_p;
  void *p_new_p;
  void *e_new_p;
  void *q_new_p;
}
;
static void OUT__5__1527__(void *__out_argv);

struct OUT__6__1527___data 
{
  void *vnewc_p;
  void *length_p;
  void *eosvmax_p;
  void *eosvmin_p;
  void *e_old_p;
  void *delvc_p;
  void *p_old_p;
  void *q_old_p;
  void *compression_p;
  void *compHalfStep_p;
  void *qq_p;
  void *ql_p;
  void *work_p;
}
;
static void OUT__6__1527__(void *__out_argv);

inline static void EvalEOSForElems(Real_t *vnewc,Index_t length)
{
  Real_t e_cut = domain. e_cut ();
  Real_t p_cut = domain. p_cut ();
  Real_t ss4o3 = domain. ss4o3 ();
  Real_t q_cut = domain. q_cut ();
  Real_t eosvmax = domain. eosvmax ();
  Real_t eosvmin = domain. eosvmin ();
  Real_t pmin = domain. pmin ();
  Real_t emin = domain. emin ();
  Real_t rho0 = domain. refdens ();
  Real_t *e_old = Allocate < Real_t > ((length));
  Real_t *delvc = Allocate < Real_t > ((length));
  Real_t *p_old = Allocate < Real_t > ((length));
  Real_t *q_old = Allocate < Real_t > ((length));
  Real_t *compression = Allocate < Real_t > ((length));
  Real_t *compHalfStep = Allocate < Real_t > ((length));
  Real_t *qq = Allocate < Real_t > ((length));
  Real_t *ql = Allocate < Real_t > ((length));
  Real_t *work = Allocate < Real_t > ((length));
  Real_t *p_new = Allocate < Real_t > ((length));
  Real_t *e_new = Allocate < Real_t > ((length));
  Real_t *q_new = Allocate < Real_t > ((length));
  Real_t *bvc = Allocate < Real_t > ((length));
  Real_t *pbvc = Allocate < Real_t > ((length));
  struct OUT__6__1527___data __out_argv6__1527__;
  __out_argv6__1527__.OUT__6__1527___data::work_p = ((void *)(&work));
  __out_argv6__1527__.OUT__6__1527___data::ql_p = ((void *)(&ql));
  __out_argv6__1527__.OUT__6__1527___data::qq_p = ((void *)(&qq));
  __out_argv6__1527__.OUT__6__1527___data::compHalfStep_p = ((void *)(&compHalfStep));
  __out_argv6__1527__.OUT__6__1527___data::compression_p = ((void *)(&compression));
  __out_argv6__1527__.OUT__6__1527___data::q_old_p = ((void *)(&q_old));
  __out_argv6__1527__.OUT__6__1527___data::p_old_p = ((void *)(&p_old));
  __out_argv6__1527__.OUT__6__1527___data::delvc_p = ((void *)(&delvc));
  __out_argv6__1527__.OUT__6__1527___data::e_old_p = ((void *)(&e_old));
  __out_argv6__1527__.OUT__6__1527___data::eosvmin_p = ((void *)(&eosvmin));
  __out_argv6__1527__.OUT__6__1527___data::eosvmax_p = ((void *)(&eosvmax));
  __out_argv6__1527__.OUT__6__1527___data::length_p = ((void *)(&length));
  __out_argv6__1527__.OUT__6__1527___data::vnewc_p = ((void *)(&vnewc));
/* compress data, minimal set */
  XOMP_parallel_start(OUT__6__1527__,&__out_argv6__1527__,1,0,"OUT__6__1527__");
  XOMP_parallel_end();
  CalcEnergyForElems(p_new,e_new,q_new,bvc,pbvc,p_old,e_old,q_old,compression,compHalfStep,vnewc,work,delvc,pmin,p_cut,e_cut,q_cut,emin,qq,ql,rho0,eosvmax,length);
  struct OUT__5__1527___data __out_argv5__1527__;
  __out_argv5__1527__.OUT__5__1527___data::q_new_p = ((void *)(&q_new));
  __out_argv5__1527__.OUT__5__1527___data::e_new_p = ((void *)(&e_new));
  __out_argv5__1527__.OUT__5__1527___data::p_new_p = ((void *)(&p_new));
  __out_argv5__1527__.OUT__5__1527___data::length_p = ((void *)(&length));
  XOMP_parallel_start(OUT__5__1527__,&__out_argv5__1527__,1,0,"OUT__5__1527__");
  XOMP_parallel_end();
  CalcSoundSpeedForElems(vnewc,rho0,e_new,p_new,pbvc,bvc,ss4o3,length);
  Release(&pbvc);
  Release(&bvc);
  Release(&q_new);
  Release(&e_new);
  Release(&p_new);
  Release(&work);
  Release(&ql);
  Release(&qq);
  Release(&compHalfStep);
  Release(&compression);
  Release(&q_old);
  Release(&p_old);
  Release(&delvc);
  Release(&e_old);
}

struct OUT__4__1527___data 
{
  void *length_p;
  void *eosvmin_p;
  void *eosvmax_p;
  void *vnewc_p;
}
;
static void OUT__4__1527__(void *__out_argv);

inline static void ApplyMaterialPropertiesForElems()
{
  Index_t length = domain. numElem ();
  if (length != 0) {
/* Expose all of the variables needed for material evaluation */
    Real_t eosvmin = domain. eosvmin ();
    Real_t eosvmax = domain. eosvmax ();
    Real_t *vnewc = Allocate < Real_t > ((length));
    struct OUT__4__1527___data __out_argv4__1527__;
    __out_argv4__1527__.OUT__4__1527___data::vnewc_p = ((void *)(&vnewc));
    __out_argv4__1527__.OUT__4__1527___data::eosvmax_p = ((void *)(&eosvmax));
    __out_argv4__1527__.OUT__4__1527___data::eosvmin_p = ((void *)(&eosvmin));
    __out_argv4__1527__.OUT__4__1527___data::length_p = ((void *)(&length));
    XOMP_parallel_start(OUT__4__1527__,&__out_argv4__1527__,1,0,"OUT__4__1527__");
    XOMP_parallel_end();
    EvalEOSForElems(vnewc,length);
    Release(&vnewc);
  }
}

struct OUT__3__1527___data 
{
  void *numElem_p;
  void *v_cut_p;
}
;
static void OUT__3__1527__(void *__out_argv);

inline static void UpdateVolumesForElems()
{
  Index_t numElem = domain. numElem ();
  if (numElem != 0) {
    Real_t v_cut = domain. v_cut ();
    Index_t i;
    struct OUT__3__1527___data __out_argv3__1527__;
    __out_argv3__1527__.OUT__3__1527___data::v_cut_p = ((void *)(&v_cut));
    __out_argv3__1527__.OUT__3__1527___data::numElem_p = ((void *)(&numElem));
    XOMP_parallel_start(OUT__3__1527__,&__out_argv3__1527__,1,0,"OUT__3__1527__");
    XOMP_parallel_end();
  }
}

inline static void LagrangeElements()
{
  const Real_t deltatime = domain. deltatime ();
  CalcLagrangeElements(deltatime);
/* Calculate Q.  (Monotonic q option requires communication) */
  CalcQForElems();
  ApplyMaterialPropertiesForElems();
  UpdateVolumesForElems();
}

struct OUT__2__1527___data 
{
  void *dtcourant_p;
  void *courant_elem_p;
  void *length_p;
  void *qqc2_p;
}
;
static void OUT__2__1527__(void *__out_argv);

inline static void CalcCourantConstraintForElems()
{
  Real_t dtcourant = ((Real_t )1.0e+20);
  Index_t courant_elem = ((-1));
  Real_t qqc = domain. qqc ();
  Index_t length = domain. numElem ();
  Real_t qqc2 = (((((Real_t )64.0)) * qqc) * qqc);
  Index_t i;
  struct OUT__2__1527___data __out_argv2__1527__;
  __out_argv2__1527__.OUT__2__1527___data::qqc2_p = ((void *)(&qqc2));
  __out_argv2__1527__.OUT__2__1527___data::length_p = ((void *)(&length));
  __out_argv2__1527__.OUT__2__1527___data::courant_elem_p = ((void *)(&courant_elem));
  __out_argv2__1527__.OUT__2__1527___data::dtcourant_p = ((void *)(&dtcourant));
  XOMP_parallel_start(OUT__2__1527__,&__out_argv2__1527__,1,0,"OUT__2__1527__");
  XOMP_parallel_end();
/* Don't try to register a time constraint if none of the elements
    * were active */
  if (courant_elem != (-1)) {
    domain. dtcourant () = dtcourant;
  }
}

struct OUT__1__1527___data 
{
  void *dthydro_p;
  void *hydro_elem_p;
  void *dvovmax_p;
  void *length_p;
}
;
static void OUT__1__1527__(void *__out_argv);

inline static void CalcHydroConstraintForElems()
{
  Real_t dthydro = ((Real_t )1.0e+20);
  Index_t hydro_elem = ((-1));
  Real_t dvovmax = domain. dvovmax ();
  Index_t length = domain. numElem ();
  Index_t i;
  struct OUT__1__1527___data __out_argv1__1527__;
  __out_argv1__1527__.OUT__1__1527___data::length_p = ((void *)(&length));
  __out_argv1__1527__.OUT__1__1527___data::dvovmax_p = ((void *)(&dvovmax));
  __out_argv1__1527__.OUT__1__1527___data::hydro_elem_p = ((void *)(&hydro_elem));
  __out_argv1__1527__.OUT__1__1527___data::dthydro_p = ((void *)(&dthydro));
  XOMP_parallel_start(OUT__1__1527__,&__out_argv1__1527__,1,0,"OUT__1__1527__");
  XOMP_parallel_end();
  if (hydro_elem != (-1)) {
    domain. dthydro () = dthydro;
  }
}

inline static void CalcTimeConstraintsForElems()
{
/* evaluate time constraint */
  CalcCourantConstraintForElems();
/* check hydro constraint */
  CalcHydroConstraintForElems();
}

inline static void LagrangeLeapFrog()
{
/* calculate nodal forces, accelerations, velocities, positions, with
    * applied boundary conditions and slide surface considerations */
  LagrangeNodal();
/* calculate element quantities (i.e. velocity gradient & q), and update
    * material states */
  LagrangeElements();
  CalcTimeConstraintsForElems();
// LagrangeRelease() ;  Creation/destruction of temps may be important to capture 
}

int main(int argc,char *argv[])
{
  int status = 0;
  XOMP_init(argc,argv);
  Index_t edgeElems = (45);
  Index_t edgeNodes = (edgeElems + 1);
// Real_t ds = Real_t(1.125)/Real_t(edgeElems) ; /* may accumulate roundoff */
  Real_t tx;
  Real_t ty;
  Real_t tz;
  Index_t nidx;
  Index_t zidx;
  Index_t domElems;
/* get run options to measure various metrics */
/* ... */
/****************************/
/*   Initialize Sedov Mesh  */
/****************************/
/* construct a uniform box for this processor */
  domain. sizeX () = edgeElems;
  domain. sizeY () = edgeElems;
  domain. sizeZ () = edgeElems;
  domain. numElem () = ((edgeElems * edgeElems) * edgeElems);
  domain. numNode () = ((edgeNodes * edgeNodes) * edgeNodes);
  domElems = domain. numElem ();
/* allocate field memory */
  domain. AllocateElemPersistent (((domain. numElem ())));
  domain. AllocateElemTemporary (((domain. numElem ())));
  domain. AllocateNodalPersistent (((domain. numNode ())));
  domain. AllocateNodesets (((edgeNodes * edgeNodes)));
/* initialize nodal coordinates */
  nidx = (0);
  tz = ((Real_t )0.);
  for (Index_t plane = (0); plane < edgeNodes; ++plane) {
    ty = ((Real_t )0.);
    for (Index_t row = (0); row < edgeNodes; ++row) {
      tx = ((Real_t )0.);
      for (Index_t col = (0); col < edgeNodes; ++col) {
        domain. x (nidx) = tx;
        domain. y (nidx) = ty;
        domain. z (nidx) = tz;
        ++nidx;
// tx += ds ; /* may accumulate roundoff... */
        tx = (((((Real_t )1.125)) * ((Real_t )(col + 1))) / ((Real_t )edgeElems));
      }
// ty += ds ;  /* may accumulate roundoff... */
      ty = (((((Real_t )1.125)) * ((Real_t )(row + 1))) / ((Real_t )edgeElems));
    }
// tz += ds ;  /* may accumulate roundoff... */
    tz = (((((Real_t )1.125)) * ((Real_t )(plane + 1))) / ((Real_t )edgeElems));
  }
/* embed hexehedral elements in nodal point lattice */
  nidx = (0);
  zidx = (0);
  for (Index_t plane = (0); plane < edgeElems; ++plane) {
    for (Index_t row = (0); row < edgeElems; ++row) {
      for (Index_t col = (0); col < edgeElems; ++col) {
        Index_t *localNode = domain. nodelist (zidx);
        localNode[0] = nidx;
        localNode[1] = (nidx + 1);
        localNode[2] = ((nidx + edgeNodes) + 1);
        localNode[3] = (nidx + edgeNodes);
        localNode[4] = (nidx + (edgeNodes * edgeNodes));
        localNode[5] = ((nidx + (edgeNodes * edgeNodes)) + 1);
        localNode[6] = (((nidx + (edgeNodes * edgeNodes)) + edgeNodes) + 1);
        localNode[7] = ((nidx + (edgeNodes * edgeNodes)) + edgeNodes);
        ++zidx;
        ++nidx;
      }
      ++nidx;
    }
    nidx += edgeNodes;
  }
  domain. AllocateNodeElemIndexes ();
/* Create a material IndexSet (entire domain same material for now) */
  for (Index_t i = (0); i < domElems; ++i) {
    domain. matElemlist (i) = i;
  }
/* initialize material parameters */
  domain. dtfixed () = ((Real_t )(-1.0e-7));
  domain. deltatime () = ((Real_t )1.0e-7);
  domain. deltatimemultlb () = ((Real_t )1.1);
  domain. deltatimemultub () = ((Real_t )1.2);
  domain. stoptime () = ((Real_t )0.01);
  domain. dtcourant () = ((Real_t )1.0e+20);
  domain. dthydro () = ((Real_t )1.0e+20);
  domain. dtmax () = ((Real_t )0.01);
  domain. time () = ((Real_t )0.);
  domain. cycle () = (0);
  domain. e_cut () = ((Real_t )1.0e-7);
  domain. p_cut () = ((Real_t )1.0e-7);
  domain. q_cut () = ((Real_t )1.0e-7);
  domain. u_cut () = ((Real_t )1.0e-7);
  domain. v_cut () = ((Real_t )1.0e-10);
  domain. hgcoef () = ((Real_t )3.0);
  domain. ss4o3 () = (((((Real_t )4.0)) / (((Real_t )3.0))));
  domain. qstop () = ((Real_t )1.0e+12);
  domain. monoq_max_slope () = ((Real_t )1.0);
  domain. monoq_limiter_mult () = ((Real_t )2.0);
  domain. qlc_monoq () = ((Real_t )0.5);
  domain. qqc_monoq () = (((((Real_t )2.0)) / (((Real_t )3.0))));
  domain. qqc () = ((Real_t )2.0);
  domain. pmin () = ((Real_t )0.);
  domain. emin () = ((Real_t )(-1.0e+15));
  domain. dvovmax () = ((Real_t )0.1);
  domain. eosvmax () = ((Real_t )1.0e+9);
  domain. eosvmin () = ((Real_t )1.0e-9);
  domain. refdens () = ((Real_t )1.0);
/* initialize field data */
  for (Index_t i = (0); i < domElems; ++i) {
    Real_t x_local[8UL];
    Real_t y_local[8UL];
    Real_t z_local[8UL];
    Index_t *elemToNode = domain. nodelist (i);
    for (Index_t lnode = (0); lnode < 8; ++lnode) {
      Index_t gnode = elemToNode[lnode];
      x_local[lnode] = domain. x (gnode);
      y_local[lnode] = domain. y (gnode);
      z_local[lnode] = domain. z (gnode);
    }
// volume calculations
    Real_t volume = CalcElemVolume((x_local),(y_local),(z_local));
    domain. volo (i) = volume;
    domain. elemMass (i) = volume;
    for (Index_t j = (0); j < 8; ++j) {
      Index_t idx = elemToNode[j];
      domain. nodalMass (idx) += (volume / (((Real_t )8.)));
    }
  }
/* deposit energy */
  domain. e ((0)) = ((Real_t )3.948746e+7);
/* set up symmetry nodesets */
  nidx = (0);
  for (Index_t i = (0); i < edgeNodes; ++i) {
    Index_t planeInc = ((i * edgeNodes) * edgeNodes);
    Index_t rowInc = (i * edgeNodes);
    for (Index_t j = (0); j < edgeNodes; ++j) {
      domain. symmX (nidx) = (planeInc + (j * edgeNodes));
      domain. symmY (nidx) = (planeInc + j);
      domain. symmZ (nidx) = (rowInc + j);
      ++nidx;
    }
  }
/* set up elemement connectivity information */
  domain. lxim ((0)) = (0);
  for (Index_t i = (1); i < domElems; ++i) {
    domain. lxim (i) = (i - 1);
    domain. lxip ((i - 1)) = i;
  }
  domain. lxip ((domElems - 1)) = (domElems - 1);
  for (Index_t i = (0); i < edgeElems; ++i) {
    domain. letam (i) = i;
    domain. letap (((domElems - edgeElems) + i)) = ((domElems - edgeElems) + i);
  }
  for (Index_t i = edgeElems; i < domElems; ++i) {
    domain. letam (i) = (i - edgeElems);
    domain. letap ((i - edgeElems)) = i;
  }
  for (Index_t i = (0); i < (edgeElems * edgeElems); ++i) {
    domain. lzetam (i) = i;
    domain. lzetap (((domElems - (edgeElems * edgeElems)) + i)) = ((domElems - (edgeElems * edgeElems)) + i);
  }
  for (Index_t i = (edgeElems * edgeElems); i < domElems; ++i) {
    domain. lzetam (i) = (i - (edgeElems * edgeElems));
    domain. lzetap ((i - (edgeElems * edgeElems))) = i;
  }
/* set up boundary condition information */
  for (Index_t i = (0); i < domElems; ++i) {
/* clear BCs by default */
    domain. elemBC (i) = (0);
  }
/* faces on "external" boundaries will be */
/* symmetry plane or free surface BCs */
  for (Index_t i = (0); i < edgeElems; ++i) {
    Index_t planeInc = ((i * edgeElems) * edgeElems);
    Index_t rowInc = (i * edgeElems);
    for (Index_t j = (0); j < edgeElems; ++j) {
      domain. elemBC ((planeInc + (j * edgeElems))) |= 1;
      domain. elemBC ((((planeInc + (j * edgeElems)) + edgeElems) - 1)) |= 8;
      domain. elemBC ((planeInc + j)) |= 16;
      domain. elemBC ((((planeInc + j) + (edgeElems * edgeElems)) - edgeElems)) |= 0x080;
      domain. elemBC ((rowInc + j)) |= 0x100;
      domain. elemBC ((((rowInc + j) + domElems) - (edgeElems * edgeElems))) |= 0x800;
    }
  }
/* timestep to solution */
  while(domain. time () < domain. stoptime ()){
    TimeIncrement();
    LagrangeLeapFrog();
/* problem->commNodes->Transfer(CommNodes::syncposvel) ; */
#if LULESH_SHOW_PROGRESS
#endif
  }
  XOMP_terminate(status);
  return 0;
}

static void OUT__1__1527__(void *__out_argv)
{
  Real_t *dthydro = (Real_t *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::dthydro_p);
  Index_t *hydro_elem = (Index_t *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::hydro_elem_p);
  Real_t *dvovmax = (Real_t *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::dvovmax_p);
  Index_t *length = (Index_t *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::length_p);
  Index_t _p_length =  *length;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    Index_t indx = domain. matElemlist (p_index_);
    if (domain. vdov (indx) != (((Real_t )0.))) {
      Real_t dtdvov = ( *dvovmax / (FABS(domain. vdov (indx)) + (((Real_t )1.e-20))));
      if ( *dthydro > dtdvov) {
        XOMP_critical_start(&xomp_critical_user_);
{
           *dthydro = dtdvov;
           *hydro_elem = indx;
        }
        XOMP_critical_end(&xomp_critical_user_);
      }
    }
  }
  XOMP_barrier();
}

static void OUT__2__1527__(void *__out_argv)
{
  Real_t *dtcourant = (Real_t *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::dtcourant_p);
  Index_t *courant_elem = (Index_t *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::courant_elem_p);
  Index_t *length = (Index_t *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::length_p);
  Real_t *qqc2 = (Real_t *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::qqc2_p);
  Index_t _p_length =  *length;
  Real_t _p_qqc2 =  *qqc2;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    Index_t indx = domain. matElemlist (p_index_);
    Real_t dtf = (domain. ss (indx) * domain. ss (indx));
    if (domain. vdov (indx) < (((Real_t )0.))) {
      dtf = (dtf + ((((_p_qqc2 * domain. arealg (indx)) * domain. arealg (indx)) * domain. vdov (indx)) * domain. vdov (indx)));
    }
    dtf = SQRT(dtf);
    dtf = (domain. arealg (indx) / dtf);
/* determine minimum timestep with its corresponding elem */
    if (domain. vdov (indx) != (((Real_t )0.))) {
      if (dtf <  *dtcourant) {
        XOMP_critical_start(&xomp_critical_user_);
{
           *dtcourant = dtf;
           *courant_elem = indx;
        }
        XOMP_critical_end(&xomp_critical_user_);
      }
    }
  }
  XOMP_barrier();
}

static void OUT__3__1527__(void *__out_argv)
{
  Index_t *numElem = (Index_t *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::numElem_p);
  Real_t *v_cut = (Real_t *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::v_cut_p);
  Index_t _p_numElem =  *numElem;
  Real_t _p_v_cut =  *v_cut;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numElem - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    Real_t tmpV;
    tmpV = domain. vnew (p_index_);
    if (FABS((tmpV - (((Real_t )1.0)))) < _p_v_cut) 
      tmpV = ((Real_t )1.0);
    domain. v (p_index_) = tmpV;
  }
  XOMP_barrier();
}

static void OUT__4__1527__(void *__out_argv)
{
  Index_t *length = (Index_t *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::length_p);
  Real_t *eosvmin = (Real_t *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::eosvmin_p);
  Real_t *eosvmax = (Real_t *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::eosvmax_p);
  Real_t **vnewc = (Real_t **)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::vnewc_p);
  Index_t i;
{
    Index_t _p_length =  *length;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      Index_t zn = domain. matElemlist (p_index_);
      ( *vnewc)[p_index_] = domain. vnew (zn);
    }
  }
  if ( *eosvmin != (((Real_t )0.))) {{
      Index_t _p_length =  *length;
      Real_t _p_eosvmin =  *eosvmin;
      Index_t _p_i;
      long p_index_;
      long p_lower_;
      long p_upper_;
      XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
      for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
        if (( *vnewc)[p_index_] < _p_eosvmin) 
          ( *vnewc)[p_index_] = _p_eosvmin;
      }
    }
  }
  if ( *eosvmax != (((Real_t )0.))) {{
      Index_t _p_length =  *length;
      Real_t _p_eosvmax =  *eosvmax;
      Index_t _p_i;
      long p_index_;
      long p_lower_;
      long p_upper_;
      XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
      for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
        if (( *vnewc)[p_index_] > _p_eosvmax) 
          ( *vnewc)[p_index_] = _p_eosvmax;
      }
    }
  }
{
    Index_t _p_length =  *length;
    Real_t _p_eosvmin =  *eosvmin;
    Real_t _p_eosvmax =  *eosvmax;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      Index_t zn = domain. matElemlist (p_index_);
      Real_t vc = domain. v (zn);
      if (_p_eosvmin != (((Real_t )0.))) {
        if (vc < _p_eosvmin) 
          vc = _p_eosvmin;
      }
      if (_p_eosvmax != (((Real_t )0.))) {
        if (vc > _p_eosvmax) 
          vc = _p_eosvmax;
      }
      if (vc <= 0.) {
        exit((VolumeError));
      }
    }
    XOMP_barrier();
  }
}

static void OUT__5__1527__(void *__out_argv)
{
  Index_t *length = (Index_t *)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::length_p);
  Real_t **p_new = (Real_t **)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::p_new_p);
  Real_t **e_new = (Real_t **)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::e_new_p);
  Real_t **q_new = (Real_t **)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::q_new_p);
  Index_t i;
{
    Index_t _p_length =  *length;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      Index_t zidx = domain. matElemlist (p_index_);
      domain. p (zidx) = ( *p_new)[p_index_];
    }
  }
{
    Index_t _p_length =  *length;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      Index_t zidx = domain. matElemlist (p_index_);
      domain. e (zidx) = ( *e_new)[p_index_];
    }
  }
{
    Index_t _p_length =  *length;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      Index_t zidx = domain. matElemlist (p_index_);
      domain. q (zidx) = ( *q_new)[p_index_];
    }
    XOMP_barrier();
  }
}

static void OUT__6__1527__(void *__out_argv)
{
  Real_t **vnewc = (Real_t **)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::vnewc_p);
  Index_t *length = (Index_t *)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::length_p);
  Real_t *eosvmax = (Real_t *)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::eosvmax_p);
  Real_t *eosvmin = (Real_t *)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::eosvmin_p);
  Real_t **e_old = (Real_t **)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::e_old_p);
  Real_t **delvc = (Real_t **)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::delvc_p);
  Real_t **p_old = (Real_t **)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::p_old_p);
  Real_t **q_old = (Real_t **)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::q_old_p);
  Real_t **compression = (Real_t **)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::compression_p);
  Real_t **compHalfStep = (Real_t **)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::compHalfStep_p);
  Real_t **qq = (Real_t **)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::qq_p);
  Real_t **ql = (Real_t **)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::ql_p);
  Real_t **work = (Real_t **)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::work_p);
  Index_t i;
{
    Index_t _p_length =  *length;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      Index_t zidx = domain. matElemlist (p_index_);
      ( *e_old)[p_index_] = domain. e (zidx);
    }
  }
{
    Index_t _p_length =  *length;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      Index_t zidx = domain. matElemlist (p_index_);
      ( *delvc)[p_index_] = domain. delv (zidx);
    }
  }
{
    Index_t _p_length =  *length;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      Index_t zidx = domain. matElemlist (p_index_);
      ( *p_old)[p_index_] = domain. p (zidx);
    }
  }
{
    Index_t _p_length =  *length;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      Index_t zidx = domain. matElemlist (p_index_);
      ( *q_old)[p_index_] = domain. q (zidx);
    }
  }
{
    Index_t _p_length =  *length;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      Real_t vchalf;
      ( *compression)[p_index_] = (((((Real_t )1.0)) / ( *vnewc)[p_index_]) - (((Real_t )1.0)));
      vchalf = (( *vnewc)[p_index_] - (( *delvc)[p_index_] * (((Real_t )0.5))));
      ( *compHalfStep)[p_index_] = (((((Real_t )1.0)) / vchalf) - (((Real_t )1.0)));
    }
  }
/* Check for v > eosvmax or v < eosvmin */
  if ( *eosvmin != (((Real_t )0.))) {{
      Index_t _p_length =  *length;
      Real_t _p_eosvmin =  *eosvmin;
      Index_t _p_i;
      long p_index_;
      long p_lower_;
      long p_upper_;
      XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
      for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
/* impossible due to calling func? */
        if (( *vnewc)[p_index_] <= _p_eosvmin) {
          ( *compHalfStep)[p_index_] = ( *compression)[p_index_];
        }
      }
    }
  }
  if ( *eosvmax != (((Real_t )0.))) {{
      Index_t _p_length =  *length;
      Real_t _p_eosvmax =  *eosvmax;
      Index_t _p_i;
      long p_index_;
      long p_lower_;
      long p_upper_;
      XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
      for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
/* impossible due to calling func? */
        if (( *vnewc)[p_index_] >= _p_eosvmax) {
          ( *p_old)[p_index_] = ((Real_t )0.);
          ( *compression)[p_index_] = ((Real_t )0.);
          ( *compHalfStep)[p_index_] = ((Real_t )0.);
        }
      }
    }
  }
{
    Index_t _p_length =  *length;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      Index_t zidx = domain. matElemlist (p_index_);
      ( *qq)[p_index_] = domain. qq (zidx);
      ( *ql)[p_index_] = domain. ql (zidx);
      ( *work)[p_index_] = ((Real_t )0.);
    }
    XOMP_barrier();
  }
}

static void OUT__7__1527__(void *__out_argv)
{
  Real_t **vnewc = (Real_t **)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::vnewc_p);
  Real_t *rho0 = (Real_t *)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::rho0_p);
  Real_t **enewc = (Real_t **)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::enewc_p);
  Real_t **pnewc = (Real_t **)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::pnewc_p);
  Real_t **pbvc = (Real_t **)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::pbvc_p);
  Real_t **bvc = (Real_t **)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::bvc_p);
  Real_t *ss4o3 = (Real_t *)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::ss4o3_p);
  Index_t *nz = (Index_t *)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::nz_p);
  Real_t _p_rho0 =  *rho0;
  Real_t _p_ss4o3 =  *ss4o3;
  Index_t _p_nz =  *nz;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_nz - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    Index_t iz = domain. matElemlist (p_index_);
    Real_t ssTmp = (((( *pbvc)[p_index_] * ( *enewc)[p_index_]) + (((( *vnewc)[p_index_] * ( *vnewc)[p_index_]) * ( *bvc)[p_index_]) * ( *pnewc)[p_index_])) / _p_rho0);
    if (ssTmp <= (((Real_t )1.111111e-36))) {
      ssTmp = ((Real_t )1.111111e-36);
    }
    domain. ss (iz) = SQRT(ssTmp);
  }
  XOMP_barrier();
}

static void OUT__8__1527__(void *__out_argv)
{
  Real_t **p_new = (Real_t **)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::p_new_p);
  Real_t **e_new = (Real_t **)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::e_new_p);
  Real_t **q_new = (Real_t **)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::q_new_p);
  Real_t **bvc = (Real_t **)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::bvc_p);
  Real_t **pbvc = (Real_t **)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::pbvc_p);
  Real_t **vnewc = (Real_t **)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::vnewc_p);
  Real_t **delvc = (Real_t **)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::delvc_p);
  Real_t *q_cut = (Real_t *)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::q_cut_p);
  Real_t **qq = (Real_t **)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::qq_p);
  Real_t **ql = (Real_t **)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::ql_p);
  Real_t *rho0 = (Real_t *)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::rho0_p);
  Index_t *length = (Index_t *)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::length_p);
  Real_t _p_q_cut =  *q_cut;
  Real_t _p_rho0 =  *rho0;
  Index_t _p_length =  *length;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    if (( *delvc)[p_index_] <= (((Real_t )0.))) {
      Real_t ssc = (((( *pbvc)[p_index_] * ( *e_new)[p_index_]) + (((( *vnewc)[p_index_] * ( *vnewc)[p_index_]) * ( *bvc)[p_index_]) * ( *p_new)[p_index_])) / _p_rho0);
      if (ssc <= (((Real_t )0.))) {
        ssc = ((Real_t ).333333e-36);
      }
      else {
        ssc = SQRT(ssc);
      }
      ( *q_new)[p_index_] = ((ssc * ( *ql)[p_index_]) + ( *qq)[p_index_]);
      if (FABS(( *q_new)[p_index_]) < _p_q_cut) 
        ( *q_new)[p_index_] = ((Real_t )0.);
    }
  }
  XOMP_barrier();
}

static void OUT__9__1527__(void *__out_argv)
{
  Real_t **p_new = (Real_t **)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::p_new_p);
  Real_t **e_new = (Real_t **)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::e_new_p);
  Real_t **q_new = (Real_t **)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::q_new_p);
  Real_t **bvc = (Real_t **)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::bvc_p);
  Real_t **pbvc = (Real_t **)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::pbvc_p);
  Real_t **p_old = (Real_t **)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::p_old_p);
  Real_t **q_old = (Real_t **)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::q_old_p);
  Real_t **vnewc = (Real_t **)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::vnewc_p);
  Real_t **delvc = (Real_t **)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::delvc_p);
  Real_t *e_cut = (Real_t *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::e_cut_p);
  Real_t *emin = (Real_t *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::emin_p);
  Real_t **qq = (Real_t **)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::qq_p);
  Real_t **ql = (Real_t **)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::ql_p);
  Real_t *rho0 = (Real_t *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::rho0_p);
  Index_t *length = (Index_t *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::length_p);
  Real_t **pHalfStep = (Real_t **)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::pHalfStep_p);
  Real_t _p_e_cut =  *e_cut;
  Real_t _p_emin =  *emin;
  Real_t _p_rho0 =  *rho0;
  Index_t _p_length =  *length;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    const Real_t sixth = (((((Real_t )1.0)) / (((Real_t )6.0))));
    Real_t q_tilde;
    if (( *delvc)[p_index_] > (((Real_t )0.))) {
      q_tilde = ((Real_t )0.);
    }
    else {
      Real_t ssc = (((( *pbvc)[p_index_] * ( *e_new)[p_index_]) + (((( *vnewc)[p_index_] * ( *vnewc)[p_index_]) * ( *bvc)[p_index_]) * ( *p_new)[p_index_])) / _p_rho0);
      if (ssc <= (((Real_t )0.))) {
        ssc = ((Real_t ).333333e-36);
      }
      else {
        ssc = SQRT(ssc);
      }
      q_tilde = ((ssc * ( *ql)[p_index_]) + ( *qq)[p_index_]);
    }
    ( *e_new)[p_index_] = (( *e_new)[p_index_] - ((((((((Real_t )7.0)) * (( *p_old)[p_index_] + ( *q_old)[p_index_])) - ((((Real_t )8.)) * (( *pHalfStep)[p_index_] + ( *q_new)[p_index_]))) + (( *p_new)[p_index_] + q_tilde)) * ( *delvc)[p_index_]) * sixth));
    if (FABS(( *e_new)[p_index_]) < _p_e_cut) {
      ( *e_new)[p_index_] = ((Real_t )0.);
    }
    if (( *e_new)[p_index_] < _p_emin) {
      ( *e_new)[p_index_] = _p_emin;
    }
  }
  XOMP_barrier();
}

static void OUT__10__1527__(void *__out_argv)
{
  Real_t **e_new = (Real_t **)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::e_new_p);
  Real_t **work = (Real_t **)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::work_p);
  Real_t *e_cut = (Real_t *)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::e_cut_p);
  Real_t *emin = (Real_t *)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::emin_p);
  Index_t *length = (Index_t *)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::length_p);
  Real_t _p_e_cut =  *e_cut;
  Real_t _p_emin =  *emin;
  Index_t _p_length =  *length;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    ( *e_new)[p_index_] += ((((Real_t )0.5)) * ( *work)[p_index_]);
    if (FABS(( *e_new)[p_index_]) < _p_e_cut) {
      ( *e_new)[p_index_] = ((Real_t )0.);
    }
    if (( *e_new)[p_index_] < _p_emin) {
      ( *e_new)[p_index_] = _p_emin;
    }
  }
  XOMP_barrier();
}

static void OUT__11__1527__(void *__out_argv)
{
  Real_t **e_new = (Real_t **)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::e_new_p);
  Real_t **q_new = (Real_t **)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::q_new_p);
  Real_t **bvc = (Real_t **)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::bvc_p);
  Real_t **pbvc = (Real_t **)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::pbvc_p);
  Real_t **p_old = (Real_t **)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::p_old_p);
  Real_t **q_old = (Real_t **)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::q_old_p);
  Real_t **compHalfStep = (Real_t **)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::compHalfStep_p);
  Real_t **delvc = (Real_t **)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::delvc_p);
  Real_t **qq = (Real_t **)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::qq_p);
  Real_t **ql = (Real_t **)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::ql_p);
  Real_t *rho0 = (Real_t *)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::rho0_p);
  Index_t *length = (Index_t *)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::length_p);
  Real_t **pHalfStep = (Real_t **)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::pHalfStep_p);
  Real_t _p_rho0 =  *rho0;
  Index_t _p_length =  *length;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    Real_t vhalf = ((((Real_t )1.0)) / ((((Real_t )1.0)) + ( *compHalfStep)[p_index_]));
    if (( *delvc)[p_index_] > (((Real_t )0.))) {
/* = qq[i] = ql[i] */
      ( *q_new)[p_index_] = ((Real_t )0.);
    }
    else {
      Real_t ssc = (((( *pbvc)[p_index_] * ( *e_new)[p_index_]) + (((vhalf * vhalf) * ( *bvc)[p_index_]) * ( *pHalfStep)[p_index_])) / _p_rho0);
      if (ssc <= (((Real_t )0.))) {
        ssc = ((Real_t ).333333e-36);
      }
      else {
        ssc = SQRT(ssc);
      }
      ( *q_new)[p_index_] = ((ssc * ( *ql)[p_index_]) + ( *qq)[p_index_]);
    }
    ( *e_new)[p_index_] = (( *e_new)[p_index_] + (((((Real_t )0.5)) * ( *delvc)[p_index_]) * (((((Real_t )3.0)) * (( *p_old)[p_index_] + ( *q_old)[p_index_])) - ((((Real_t )4.0)) * (( *pHalfStep)[p_index_] + ( *q_new)[p_index_])))));
  }
  XOMP_barrier();
}

static void OUT__12__1527__(void *__out_argv)
{
  Real_t **e_new = (Real_t **)(((struct OUT__12__1527___data *)__out_argv) -> OUT__12__1527___data::e_new_p);
  Real_t **p_old = (Real_t **)(((struct OUT__12__1527___data *)__out_argv) -> OUT__12__1527___data::p_old_p);
  Real_t **e_old = (Real_t **)(((struct OUT__12__1527___data *)__out_argv) -> OUT__12__1527___data::e_old_p);
  Real_t **q_old = (Real_t **)(((struct OUT__12__1527___data *)__out_argv) -> OUT__12__1527___data::q_old_p);
  Real_t **work = (Real_t **)(((struct OUT__12__1527___data *)__out_argv) -> OUT__12__1527___data::work_p);
  Real_t **delvc = (Real_t **)(((struct OUT__12__1527___data *)__out_argv) -> OUT__12__1527___data::delvc_p);
  Real_t *emin = (Real_t *)(((struct OUT__12__1527___data *)__out_argv) -> OUT__12__1527___data::emin_p);
  Index_t *length = (Index_t *)(((struct OUT__12__1527___data *)__out_argv) -> OUT__12__1527___data::length_p);
  Real_t _p_emin =  *emin;
  Index_t _p_length =  *length;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    ( *e_new)[p_index_] = ((( *e_old)[p_index_] - (((((Real_t )0.5)) * ( *delvc)[p_index_]) * (( *p_old)[p_index_] + ( *q_old)[p_index_]))) + ((((Real_t )0.5)) * ( *work)[p_index_]));
    if (( *e_new)[p_index_] < _p_emin) {
      ( *e_new)[p_index_] = _p_emin;
    }
  }
  XOMP_barrier();
}

static void OUT__13__1527__(void *__out_argv)
{
  Real_t **p_new = (Real_t **)(((struct OUT__13__1527___data *)__out_argv) -> OUT__13__1527___data::p_new_p);
  Real_t **bvc = (Real_t **)(((struct OUT__13__1527___data *)__out_argv) -> OUT__13__1527___data::bvc_p);
  Real_t **e_old = (Real_t **)(((struct OUT__13__1527___data *)__out_argv) -> OUT__13__1527___data::e_old_p);
  Real_t **vnewc = (Real_t **)(((struct OUT__13__1527___data *)__out_argv) -> OUT__13__1527___data::vnewc_p);
  Real_t *pmin = (Real_t *)(((struct OUT__13__1527___data *)__out_argv) -> OUT__13__1527___data::pmin_p);
  Real_t *p_cut = (Real_t *)(((struct OUT__13__1527___data *)__out_argv) -> OUT__13__1527___data::p_cut_p);
  Real_t *eosvmax = (Real_t *)(((struct OUT__13__1527___data *)__out_argv) -> OUT__13__1527___data::eosvmax_p);
  Index_t *length = (Index_t *)(((struct OUT__13__1527___data *)__out_argv) -> OUT__13__1527___data::length_p);
  Real_t _p_pmin =  *pmin;
  Real_t _p_p_cut =  *p_cut;
  Real_t _p_eosvmax =  *eosvmax;
  Index_t _p_length =  *length;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    ( *p_new)[p_index_] = (( *bvc)[p_index_] * ( *e_old)[p_index_]);
    if (FABS(( *p_new)[p_index_]) < _p_p_cut) 
      ( *p_new)[p_index_] = ((Real_t )0.);
/* impossible condition here? */
    if (( *vnewc)[p_index_] >= _p_eosvmax) 
      ( *p_new)[p_index_] = ((Real_t )0.);
    if (( *p_new)[p_index_] < _p_pmin) 
      ( *p_new)[p_index_] = _p_pmin;
  }
  XOMP_barrier();
}

static void OUT__14__1527__(void *__out_argv)
{
  Real_t **bvc = (Real_t **)(((struct OUT__14__1527___data *)__out_argv) -> OUT__14__1527___data::bvc_p);
  Real_t **pbvc = (Real_t **)(((struct OUT__14__1527___data *)__out_argv) -> OUT__14__1527___data::pbvc_p);
  Real_t **compression = (Real_t **)(((struct OUT__14__1527___data *)__out_argv) -> OUT__14__1527___data::compression_p);
  Index_t *length = (Index_t *)(((struct OUT__14__1527___data *)__out_argv) -> OUT__14__1527___data::length_p);
  Index_t _p_length =  *length;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_length - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    Real_t c1s = (((((Real_t )2.0)) / (((Real_t )3.0))));
    ( *bvc)[p_index_] = (c1s * (( *compression)[p_index_] + (((Real_t )1.0))));
    ( *pbvc)[p_index_] = c1s;
  }
  XOMP_barrier();
}

static void OUT__15__1527__(void *__out_argv)
{
  Real_t *qlc_monoq = (Real_t *)(((struct OUT__15__1527___data *)__out_argv) -> OUT__15__1527___data::qlc_monoq_p);
  Real_t *qqc_monoq = (Real_t *)(((struct OUT__15__1527___data *)__out_argv) -> OUT__15__1527___data::qqc_monoq_p);
  Real_t *monoq_limiter_mult = (Real_t *)(((struct OUT__15__1527___data *)__out_argv) -> OUT__15__1527___data::monoq_limiter_mult_p);
  Real_t *monoq_max_slope = (Real_t *)(((struct OUT__15__1527___data *)__out_argv) -> OUT__15__1527___data::monoq_max_slope_p);
  Real_t *ptiny = (Real_t *)(((struct OUT__15__1527___data *)__out_argv) -> OUT__15__1527___data::ptiny_p);
  Index_t *elength = (Index_t *)(((struct OUT__15__1527___data *)__out_argv) -> OUT__15__1527___data::elength_p);
  Real_t _p_qlc_monoq =  *qlc_monoq;
  Real_t _p_qqc_monoq =  *qqc_monoq;
  Real_t _p_monoq_limiter_mult =  *monoq_limiter_mult;
  Real_t _p_monoq_max_slope =  *monoq_max_slope;
  Real_t _p_ptiny =  *ptiny;
  Index_t _p_elength =  *elength;
  Index_t _p_ielem;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_elength - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    Real_t qlin;
    Real_t qquad;
    Real_t phixi;
    Real_t phieta;
    Real_t phizeta;
    Index_t i = domain. matElemlist (p_index_);
    Int_t bcMask = domain. elemBC (i);
    Real_t delvm;
    Real_t delvp;
/*  phixi     */
    Real_t norm = ((((Real_t )1.0)) / (domain. delv_xi (i) + _p_ptiny));
    switch(bcMask & 3){
      case 0:
{
        delvm = domain. delv_xi (domain. lxim (i));
        break; 
      }
      case 0x001:
{
        delvm = domain. delv_xi (i);
        break; 
      }
      case 0x002:
{
        delvm = ((Real_t )0.);
        break; 
      }
/* ERROR */
      default:
{
        break; 
      }
    }
    switch(bcMask & 12){
      case 0:
{
        delvp = domain. delv_xi (domain. lxip (i));
        break; 
      }
      case 0x004:
{
        delvp = domain. delv_xi (i);
        break; 
      }
      case 0x008:
{
        delvp = ((Real_t )0.);
        break; 
      }
/* ERROR */
      default:
{
        break; 
      }
    }
    delvm = (delvm * norm);
    delvp = (delvp * norm);
    phixi = ((((Real_t )0.5)) * (delvm + delvp));
    delvm *= _p_monoq_limiter_mult;
    delvp *= _p_monoq_limiter_mult;
    if (delvm < phixi) 
      phixi = delvm;
    if (delvp < phixi) 
      phixi = delvp;
    if (phixi < (((Real_t )0.))) 
      phixi = ((Real_t )0.);
    if (phixi > _p_monoq_max_slope) 
      phixi = _p_monoq_max_slope;
/*  phieta     */
    norm = ((((Real_t )1.0)) / (domain. delv_eta (i) + _p_ptiny));
    switch(bcMask & 48){
      case 0:
{
        delvm = domain. delv_eta (domain. letam (i));
        break; 
      }
      case 0x010:
{
        delvm = domain. delv_eta (i);
        break; 
      }
      case 0x020:
{
        delvm = ((Real_t )0.);
        break; 
      }
/* ERROR */
      default:
{
        break; 
      }
    }
    switch(bcMask & 0x0c0){
      case 0:
{
        delvp = domain. delv_eta (domain. letap (i));
        break; 
      }
      case 0x040:
{
        delvp = domain. delv_eta (i);
        break; 
      }
      case 0x080:
{
        delvp = ((Real_t )0.);
        break; 
      }
/* ERROR */
      default:
{
        break; 
      }
    }
    delvm = (delvm * norm);
    delvp = (delvp * norm);
    phieta = ((((Real_t )0.5)) * (delvm + delvp));
    delvm *= _p_monoq_limiter_mult;
    delvp *= _p_monoq_limiter_mult;
    if (delvm < phieta) 
      phieta = delvm;
    if (delvp < phieta) 
      phieta = delvp;
    if (phieta < (((Real_t )0.))) 
      phieta = ((Real_t )0.);
    if (phieta > _p_monoq_max_slope) 
      phieta = _p_monoq_max_slope;
/*  phizeta     */
    norm = ((((Real_t )1.0)) / (domain. delv_zeta (i) + _p_ptiny));
    switch(bcMask & 0x300){
      case 0:
{
        delvm = domain. delv_zeta (domain. lzetam (i));
        break; 
      }
      case 0x100:
{
        delvm = domain. delv_zeta (i);
        break; 
      }
      case 0x200:
{
        delvm = ((Real_t )0.);
        break; 
      }
/* ERROR */
      default:
{
        break; 
      }
    }
    switch(bcMask & 0xc00){
      case 0:
{
        delvp = domain. delv_zeta (domain. lzetap (i));
        break; 
      }
      case 0x400:
{
        delvp = domain. delv_zeta (i);
        break; 
      }
      case 0x800:
{
        delvp = ((Real_t )0.);
        break; 
      }
/* ERROR */
      default:
{
        break; 
      }
    }
    delvm = (delvm * norm);
    delvp = (delvp * norm);
    phizeta = ((((Real_t )0.5)) * (delvm + delvp));
    delvm *= _p_monoq_limiter_mult;
    delvp *= _p_monoq_limiter_mult;
    if (delvm < phizeta) 
      phizeta = delvm;
    if (delvp < phizeta) 
      phizeta = delvp;
    if (phizeta < (((Real_t )0.))) 
      phizeta = ((Real_t )0.);
    if (phizeta > _p_monoq_max_slope) 
      phizeta = _p_monoq_max_slope;
/* Remove length scale */
    if (domain. vdov (i) > (((Real_t )0.))) {
      qlin = ((Real_t )0.);
      qquad = ((Real_t )0.);
    }
    else {
      Real_t delvxxi = (domain. delv_xi (i) * domain. delx_xi (i));
      Real_t delvxeta = (domain. delv_eta (i) * domain. delx_eta (i));
      Real_t delvxzeta = (domain. delv_zeta (i) * domain. delx_zeta (i));
      if (delvxxi > (((Real_t )0.))) 
        delvxxi = ((Real_t )0.);
      if (delvxeta > (((Real_t )0.))) 
        delvxeta = ((Real_t )0.);
      if (delvxzeta > (((Real_t )0.))) 
        delvxzeta = ((Real_t )0.);
      Real_t rho = (domain. elemMass (i) / (domain. volo (i) * domain. vnew (i)));
      qlin = ((-_p_qlc_monoq * rho) * (((delvxxi * ((((Real_t )1.0)) - phixi)) + (delvxeta * ((((Real_t )1.0)) - phieta))) + (delvxzeta * ((((Real_t )1.0)) - phizeta))));
      qquad = ((_p_qqc_monoq * rho) * ((((delvxxi * delvxxi) * ((((Real_t )1.0)) - (phixi * phixi))) + ((delvxeta * delvxeta) * ((((Real_t )1.0)) - (phieta * phieta)))) + ((delvxzeta * delvxzeta) * ((((Real_t )1.0)) - (phizeta * phizeta)))));
    }
    domain. qq (i) = qquad;
    domain. ql (i) = qlin;
  }
  XOMP_barrier();
}

static void OUT__16__1527__(void *__out_argv)
{
  Index_t *numElem = (Index_t *)(((struct OUT__16__1527___data *)__out_argv) -> OUT__16__1527___data::numElem_p);
  Index_t _p_numElem =  *numElem;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numElem - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    const Real_t ptiny = ((Real_t )1.e-36);
    Real_t ax;
    Real_t ay;
    Real_t az;
    Real_t dxv;
    Real_t dyv;
    Real_t dzv;
    const Index_t *elemToNode = (domain. nodelist (p_index_));
    Index_t n0 = elemToNode[0];
    Index_t n1 = elemToNode[1];
    Index_t n2 = elemToNode[2];
    Index_t n3 = elemToNode[3];
    Index_t n4 = elemToNode[4];
    Index_t n5 = elemToNode[5];
    Index_t n6 = elemToNode[6];
    Index_t n7 = elemToNode[7];
    Real_t x0 = domain. x (n0);
    Real_t x1 = domain. x (n1);
    Real_t x2 = domain. x (n2);
    Real_t x3 = domain. x (n3);
    Real_t x4 = domain. x (n4);
    Real_t x5 = domain. x (n5);
    Real_t x6 = domain. x (n6);
    Real_t x7 = domain. x (n7);
    Real_t y0 = domain. y (n0);
    Real_t y1 = domain. y (n1);
    Real_t y2 = domain. y (n2);
    Real_t y3 = domain. y (n3);
    Real_t y4 = domain. y (n4);
    Real_t y5 = domain. y (n5);
    Real_t y6 = domain. y (n6);
    Real_t y7 = domain. y (n7);
    Real_t z0 = domain. z (n0);
    Real_t z1 = domain. z (n1);
    Real_t z2 = domain. z (n2);
    Real_t z3 = domain. z (n3);
    Real_t z4 = domain. z (n4);
    Real_t z5 = domain. z (n5);
    Real_t z6 = domain. z (n6);
    Real_t z7 = domain. z (n7);
    Real_t xv0 = domain. xd (n0);
    Real_t xv1 = domain. xd (n1);
    Real_t xv2 = domain. xd (n2);
    Real_t xv3 = domain. xd (n3);
    Real_t xv4 = domain. xd (n4);
    Real_t xv5 = domain. xd (n5);
    Real_t xv6 = domain. xd (n6);
    Real_t xv7 = domain. xd (n7);
    Real_t yv0 = domain. yd (n0);
    Real_t yv1 = domain. yd (n1);
    Real_t yv2 = domain. yd (n2);
    Real_t yv3 = domain. yd (n3);
    Real_t yv4 = domain. yd (n4);
    Real_t yv5 = domain. yd (n5);
    Real_t yv6 = domain. yd (n6);
    Real_t yv7 = domain. yd (n7);
    Real_t zv0 = domain. zd (n0);
    Real_t zv1 = domain. zd (n1);
    Real_t zv2 = domain. zd (n2);
    Real_t zv3 = domain. zd (n3);
    Real_t zv4 = domain. zd (n4);
    Real_t zv5 = domain. zd (n5);
    Real_t zv6 = domain. zd (n6);
    Real_t zv7 = domain. zd (n7);
    Real_t vol = (domain. volo (p_index_) * domain. vnew (p_index_));
    Real_t norm = ((((Real_t )1.0)) / (vol + ptiny));
    Real_t dxj = ((((Real_t )(-0.25))) * ((((x0 + x1) + x5) + x4) - (((x3 + x2) + x6) + x7)));
    Real_t dyj = ((((Real_t )(-0.25))) * ((((y0 + y1) + y5) + y4) - (((y3 + y2) + y6) + y7)));
    Real_t dzj = ((((Real_t )(-0.25))) * ((((z0 + z1) + z5) + z4) - (((z3 + z2) + z6) + z7)));
    Real_t dxi = ((((Real_t )0.25)) * ((((x1 + x2) + x6) + x5) - (((x0 + x3) + x7) + x4)));
    Real_t dyi = ((((Real_t )0.25)) * ((((y1 + y2) + y6) + y5) - (((y0 + y3) + y7) + y4)));
    Real_t dzi = ((((Real_t )0.25)) * ((((z1 + z2) + z6) + z5) - (((z0 + z3) + z7) + z4)));
    Real_t dxk = ((((Real_t )0.25)) * ((((x4 + x5) + x6) + x7) - (((x0 + x1) + x2) + x3)));
    Real_t dyk = ((((Real_t )0.25)) * ((((y4 + y5) + y6) + y7) - (((y0 + y1) + y2) + y3)));
    Real_t dzk = ((((Real_t )0.25)) * ((((z4 + z5) + z6) + z7) - (((z0 + z1) + z2) + z3)));
/* find delvk and delxk ( i cross j ) */
    ax = ((dyi * dzj) - (dzi * dyj));
    ay = ((dzi * dxj) - (dxi * dzj));
    az = ((dxi * dyj) - (dyi * dxj));
    domain. delx_zeta (p_index_) = (vol / SQRT(((((ax * ax) + (ay * ay)) + (az * az)) + ptiny)));
    ax *= norm;
    ay *= norm;
    az *= norm;
    dxv = ((((Real_t )0.25)) * ((((xv4 + xv5) + xv6) + xv7) - (((xv0 + xv1) + xv2) + xv3)));
    dyv = ((((Real_t )0.25)) * ((((yv4 + yv5) + yv6) + yv7) - (((yv0 + yv1) + yv2) + yv3)));
    dzv = ((((Real_t )0.25)) * ((((zv4 + zv5) + zv6) + zv7) - (((zv0 + zv1) + zv2) + zv3)));
    domain. delv_zeta (p_index_) = (((ax * dxv) + (ay * dyv)) + (az * dzv));
/* find delxi and delvi ( j cross k ) */
    ax = ((dyj * dzk) - (dzj * dyk));
    ay = ((dzj * dxk) - (dxj * dzk));
    az = ((dxj * dyk) - (dyj * dxk));
    domain. delx_xi (p_index_) = (vol / SQRT(((((ax * ax) + (ay * ay)) + (az * az)) + ptiny)));
    ax *= norm;
    ay *= norm;
    az *= norm;
    dxv = ((((Real_t )0.25)) * ((((xv1 + xv2) + xv6) + xv5) - (((xv0 + xv3) + xv7) + xv4)));
    dyv = ((((Real_t )0.25)) * ((((yv1 + yv2) + yv6) + yv5) - (((yv0 + yv3) + yv7) + yv4)));
    dzv = ((((Real_t )0.25)) * ((((zv1 + zv2) + zv6) + zv5) - (((zv0 + zv3) + zv7) + zv4)));
    domain. delv_xi (p_index_) = (((ax * dxv) + (ay * dyv)) + (az * dzv));
/* find delxj and delvj ( k cross i ) */
    ax = ((dyk * dzi) - (dzk * dyi));
    ay = ((dzk * dxi) - (dxk * dzi));
    az = ((dxk * dyi) - (dyk * dxi));
    domain. delx_eta (p_index_) = (vol / SQRT(((((ax * ax) + (ay * ay)) + (az * az)) + ptiny)));
    ax *= norm;
    ay *= norm;
    az *= norm;
    dxv = ((((Real_t )(-0.25))) * ((((xv0 + xv1) + xv5) + xv4) - (((xv3 + xv2) + xv6) + xv7)));
    dyv = ((((Real_t )(-0.25))) * ((((yv0 + yv1) + yv5) + yv4) - (((yv3 + yv2) + yv6) + yv7)));
    dzv = ((((Real_t )(-0.25))) * ((((zv0 + zv1) + zv5) + zv4) - (((zv3 + zv2) + zv6) + zv7)));
    domain. delv_eta (p_index_) = (((ax * dxv) + (ay * dyv)) + (az * dzv));
  }
  XOMP_barrier();
}

static void OUT__17__1527__(void *__out_argv)
{
  Index_t *numElem = (Index_t *)(((struct OUT__17__1527___data *)__out_argv) -> OUT__17__1527___data::numElem_p);
  Index_t _p_numElem =  *numElem;
  Index_t _p_k;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numElem - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
// calc strain rate and apply as constraint (only done in FB element)
    Real_t vdov = ((domain. dxx (p_index_) + domain. dyy (p_index_)) + domain. dzz (p_index_));
    Real_t vdovthird = (vdov / (((Real_t )3.0)));
// make the rate of deformation tensor deviatoric
    domain. vdov (p_index_) = vdov;
    domain. dxx (p_index_) -= vdovthird;
    domain. dyy (p_index_) -= vdovthird;
    domain. dzz (p_index_) -= vdovthird;
// See if any volumes are negative, and take appropriate action.
    if (domain. vnew (p_index_) <= (((Real_t )0.))) {
      exit((VolumeError));
    }
  }
  XOMP_barrier();
}

static void OUT__18__1527__(void *__out_argv)
{
  Index_t *numElem = (Index_t *)(((struct OUT__18__1527___data *)__out_argv) -> OUT__18__1527___data::numElem_p);
  Real_t *dt = (Real_t *)(((struct OUT__18__1527___data *)__out_argv) -> OUT__18__1527___data::dt_p);
  Index_t _p_numElem =  *numElem;
  Real_t _p_dt =  *dt;
  Index_t _p_k;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numElem - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
/** shape function derivatives */
    Real_t B[3UL][8UL];
    Real_t D[6UL];
    Real_t x_local[8UL];
    Real_t y_local[8UL];
    Real_t z_local[8UL];
    Real_t xd_local[8UL];
    Real_t yd_local[8UL];
    Real_t zd_local[8UL];
    Real_t detJ = ((Real_t )0.);
    Real_t volume;
    Real_t relativeVolume;
    const Index_t *const elemToNode = (domain. nodelist (p_index_));
// get nodal coordinates from global arrays and copy into local arrays.
    for (Index_t lnode = (0); lnode < 8; ++lnode) {
      Index_t gnode = elemToNode[lnode];
      x_local[lnode] = domain. x (gnode);
      y_local[lnode] = domain. y (gnode);
      z_local[lnode] = domain. z (gnode);
    }
// volume calculations
    volume = CalcElemVolume((x_local),(y_local),(z_local));
    relativeVolume = (volume / domain. volo (p_index_));
    domain. vnew (p_index_) = relativeVolume;
    domain. delv (p_index_) = (relativeVolume - domain. v (p_index_));
// set characteristic length
    domain. arealg (p_index_) = CalcElemCharacteristicLength((x_local),(y_local),(z_local),volume);
// get nodal velocities from global array and copy into local arrays.
    for (Index_t lnode = (0); lnode < 8; ++lnode) {
      Index_t gnode = elemToNode[lnode];
      xd_local[lnode] = domain. xd (gnode);
      yd_local[lnode] = domain. yd (gnode);
      zd_local[lnode] = domain. zd (gnode);
    }
    Real_t dt2 = ((((Real_t )0.5)) * _p_dt);
    for (Index_t j = (0); j < 8; ++j) {
      x_local[j] -= (dt2 * xd_local[j]);
      y_local[j] -= (dt2 * yd_local[j]);
      z_local[j] -= (dt2 * zd_local[j]);
    }
    CalcElemShapeFunctionDerivatives((x_local),(y_local),(z_local),B,&detJ);
    CalcElemVelocityGrandient((xd_local),(yd_local),(zd_local),(B),detJ,D);
// put velocity gradient quantities into their global arrays.
    domain. dxx (p_index_) = D[0];
    domain. dyy (p_index_) = D[1];
    domain. dzz (p_index_) = D[2];
  }
  XOMP_barrier();
}

static void OUT__19__1527__(void *__out_argv)
{
  const Real_t *dt = (const Real_t *)(((struct OUT__19__1527___data *)__out_argv) -> OUT__19__1527___data::dt_p);
  Index_t *numNode = (Index_t *)(((struct OUT__19__1527___data *)__out_argv) -> OUT__19__1527___data::numNode_p);
  Index_t _p_numNode =  *numNode;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numNode - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    domain. x (p_index_) += (domain. xd (p_index_) *  *dt);
    domain. y (p_index_) += (domain. yd (p_index_) *  *dt);
    domain. z (p_index_) += (domain. zd (p_index_) *  *dt);
  }
  XOMP_barrier();
}

static void OUT__20__1527__(void *__out_argv)
{
  const Real_t *dt = (const Real_t *)(((struct OUT__20__1527___data *)__out_argv) -> OUT__20__1527___data::dt_p);
  const Real_t *u_cut = (const Real_t *)(((struct OUT__20__1527___data *)__out_argv) -> OUT__20__1527___data::u_cut_p);
  Index_t *numNode = (Index_t *)(((struct OUT__20__1527___data *)__out_argv) -> OUT__20__1527___data::numNode_p);
  Index_t _p_numNode =  *numNode;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numNode - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    Real_t xdtmp;
    Real_t ydtmp;
    Real_t zdtmp;
    xdtmp = (domain. xd (p_index_) + (domain. xdd (p_index_) *  *dt));
    if (FABS(xdtmp) <  *u_cut) 
      xdtmp = ((Real_t )0.);
    domain. xd (p_index_) = xdtmp;
    ydtmp = (domain. yd (p_index_) + (domain. ydd (p_index_) *  *dt));
    if (FABS(ydtmp) <  *u_cut) 
      ydtmp = ((Real_t )0.);
    domain. yd (p_index_) = ydtmp;
    zdtmp = (domain. zd (p_index_) + (domain. zdd (p_index_) *  *dt));
    if (FABS(zdtmp) <  *u_cut) 
      zdtmp = ((Real_t )0.);
    domain. zd (p_index_) = zdtmp;
  }
  XOMP_barrier();
}

static void OUT__21__1527__(void *__out_argv)
{
  Index_t *numNodeBC = (Index_t *)(((struct OUT__21__1527___data *)__out_argv) -> OUT__21__1527___data::numNodeBC_p);
  Index_t i;
{
    Index_t _p_numNodeBC =  *numNodeBC;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_numNodeBC - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      domain. xdd (domain. symmX (p_index_)) = ((Real_t )0.);
    }
  }
{
    Index_t _p_numNodeBC =  *numNodeBC;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_numNodeBC - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      domain. ydd (domain. symmY (p_index_)) = ((Real_t )0.);
    }
  }
{
    Index_t _p_numNodeBC =  *numNodeBC;
    Index_t _p_i;
    long p_index_;
    long p_lower_;
    long p_upper_;
    XOMP_loop_default((0),_p_numNodeBC - 1,1,&p_lower_,&p_upper_);
    for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
      domain. zdd (domain. symmZ (p_index_)) = ((Real_t )0.);
    }
    XOMP_barrier();
  }
}

static void OUT__22__1527__(void *__out_argv)
{
  Index_t *numNode = (Index_t *)(((struct OUT__22__1527___data *)__out_argv) -> OUT__22__1527___data::numNode_p);
  Index_t _p_numNode =  *numNode;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numNode - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    domain. xdd (p_index_) = (domain. fx (p_index_) / domain. nodalMass (p_index_));
    domain. ydd (p_index_) = (domain. fy (p_index_) / domain. nodalMass (p_index_));
    domain. zdd (p_index_) = (domain. fz (p_index_) / domain. nodalMass (p_index_));
  }
  XOMP_barrier();
}

static void OUT__23__1527__(void *__out_argv)
{
  Index_t *numNode = (Index_t *)(((struct OUT__23__1527___data *)__out_argv) -> OUT__23__1527___data::numNode_p);
  Index_t _p_numNode =  *numNode;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numNode - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    domain. fx (p_index_) = ((Real_t )0.);
    domain. fy (p_index_) = ((Real_t )0.);
    domain. fz (p_index_) = ((Real_t )0.);
  }
  XOMP_barrier();
}

static void OUT__24__1527__(void *__out_argv)
{
  Index_t *numElem = (Index_t *)(((struct OUT__24__1527___data *)__out_argv) -> OUT__24__1527___data::numElem_p);
  Real_t **determ = (Real_t **)(((struct OUT__24__1527___data *)__out_argv) -> OUT__24__1527___data::determ_p);
  Index_t _p_numElem =  *numElem;
  Index_t _p_k;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numElem - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    if (( *determ)[p_index_] <= (((Real_t )0.))) {
      exit((VolumeError));
    }
  }
  XOMP_barrier();
}

static void OUT__25__1527__(void *__out_argv)
{
  Real_t **determ = (Real_t **)(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::determ_p);
  Index_t *numElem = (Index_t *)(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::numElem_p);
  Real_t **dvdx = (Real_t **)(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::dvdx_p);
  Real_t **dvdy = (Real_t **)(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::dvdy_p);
  Real_t **dvdz = (Real_t **)(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::dvdz_p);
  Real_t **x8n = (Real_t **)(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::x8n_p);
  Real_t **y8n = (Real_t **)(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::y8n_p);
  Real_t **z8n = (Real_t **)(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::z8n_p);
  Index_t _p_numElem =  *numElem;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numElem - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    Real_t x1[8UL];
    Real_t y1[8UL];
    Real_t z1[8UL];
    Real_t pfx[8UL];
    Real_t pfy[8UL];
    Real_t pfz[8UL];
    Index_t *elemToNode = domain. nodelist (p_index_);
    CollectDomainNodesToElemNodes((elemToNode),x1,y1,z1);
    CalcElemVolumeDerivative(pfx,pfy,pfz,(x1),(y1),(z1));
/* load into temporary storage for FB Hour Glass control */
    for (Index_t ii = (0); ii < 8; ++ii) {
      Index_t jj = ((8 * p_index_) + ii);
      ( *dvdx)[jj] = pfx[ii];
      ( *dvdy)[jj] = pfy[ii];
      ( *dvdz)[jj] = pfz[ii];
      ( *x8n)[jj] = x1[ii];
      ( *y8n)[jj] = y1[ii];
      ( *z8n)[jj] = z1[ii];
    }
    ( *determ)[p_index_] = (domain. volo (p_index_) * domain. v (p_index_));
/* Do a check for negative volumes */
    if (domain. v (p_index_) <= (((Real_t )0.))) {
      exit((VolumeError));
    }
  }
  XOMP_barrier();
}

static void OUT__26__1527__(void *__out_argv)
{
  Real_t **fx_elem = (Real_t **)(((struct OUT__26__1527___data *)__out_argv) -> OUT__26__1527___data::fx_elem_p);
  Real_t **fy_elem = (Real_t **)(((struct OUT__26__1527___data *)__out_argv) -> OUT__26__1527___data::fy_elem_p);
  Real_t **fz_elem = (Real_t **)(((struct OUT__26__1527___data *)__out_argv) -> OUT__26__1527___data::fz_elem_p);
  Index_t *numNode = (Index_t *)(((struct OUT__26__1527___data *)__out_argv) -> OUT__26__1527___data::numNode_p);
  Index_t _p_numNode =  *numNode;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default(((Index_t )0),_p_numNode - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    Index_t count = domain. nodeElemCount (p_index_);
    Index_t start = domain. nodeElemStart (p_index_);
    Real_t fx = ((Real_t )0.);
    Real_t fy = ((Real_t )0.);
    Real_t fz = ((Real_t )0.);
    for (Index_t i = (0); i < count; ++i) {
      Index_t elem = domain. nodeElemCornerList ((start + i));
      fx += ( *fx_elem)[elem];
      fy += ( *fy_elem)[elem];
      fz += ( *fz_elem)[elem];
    }
    domain. fx (p_index_) += fx;
    domain. fy (p_index_) += fy;
    domain. fz (p_index_) += fz;
  }
  XOMP_barrier();
}

static void OUT__27__1527__(void *__out_argv)
{
  Real_t **determ = (Real_t **)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::determ_p);
  Real_t **x8n = (Real_t **)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::x8n_p);
  Real_t **y8n = (Real_t **)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::y8n_p);
  Real_t **z8n = (Real_t **)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::z8n_p);
  Real_t **dvdx = (Real_t **)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::dvdx_p);
  Real_t **dvdy = (Real_t **)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::dvdy_p);
  Real_t **dvdz = (Real_t **)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::dvdz_p);
  Real_t *hourg = (Real_t *)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::hourg_p);
  Index_t *numElem = (Index_t *)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::numElem_p);
  Real_t **fx_elem = (Real_t **)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::fx_elem_p);
  Real_t **fy_elem = (Real_t **)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::fy_elem_p);
  Real_t **fz_elem = (Real_t **)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::fz_elem_p);
  Real_t (*gamma)[4UL][8UL] = (Real_t (*)[4UL][8UL])(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::gamma_p);
  Real_t _p_hourg =  *hourg;
  Index_t _p_numElem =  *numElem;
  Index_t _p_i2;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numElem - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    Real_t *fx_local;
    Real_t *fy_local;
    Real_t *fz_local;
    Real_t hgfx[8UL];
    Real_t hgfy[8UL];
    Real_t hgfz[8UL];
    Real_t coefficient;
    Real_t hourgam0[4UL];
    Real_t hourgam1[4UL];
    Real_t hourgam2[4UL];
    Real_t hourgam3[4UL];
    Real_t hourgam4[4UL];
    Real_t hourgam5[4UL];
    Real_t hourgam6[4UL];
    Real_t hourgam7[4UL];
    Real_t xd1[8UL];
    Real_t yd1[8UL];
    Real_t zd1[8UL];
    const Index_t *elemToNode = (domain. nodelist (p_index_));
    Index_t i3 = (8 * p_index_);
    Real_t volinv = ((((Real_t )1.0)) / ( *determ)[p_index_]);
    Real_t ss1;
    Real_t mass1;
    Real_t volume13;
    for (Index_t i1 = (0); i1 < 4; ++i1) {
      Real_t hourmodx = ((((((((( *x8n)[i3] * ( *gamma)[i1][0]) + (( *x8n)[i3 + 1] * ( *gamma)[i1][1])) + (( *x8n)[i3 + 2] * ( *gamma)[i1][2])) + (( *x8n)[i3 + 3] * ( *gamma)[i1][3])) + (( *x8n)[i3 + 4] * ( *gamma)[i1][4])) + (( *x8n)[i3 + 5] * ( *gamma)[i1][5])) + (( *x8n)[i3 + 6] * ( *gamma)[i1][6])) + (( *x8n)[i3 + 7] * ( *gamma)[i1][7]));
      Real_t hourmody = ((((((((( *y8n)[i3] * ( *gamma)[i1][0]) + (( *y8n)[i3 + 1] * ( *gamma)[i1][1])) + (( *y8n)[i3 + 2] * ( *gamma)[i1][2])) + (( *y8n)[i3 + 3] * ( *gamma)[i1][3])) + (( *y8n)[i3 + 4] * ( *gamma)[i1][4])) + (( *y8n)[i3 + 5] * ( *gamma)[i1][5])) + (( *y8n)[i3 + 6] * ( *gamma)[i1][6])) + (( *y8n)[i3 + 7] * ( *gamma)[i1][7]));
      Real_t hourmodz = ((((((((( *z8n)[i3] * ( *gamma)[i1][0]) + (( *z8n)[i3 + 1] * ( *gamma)[i1][1])) + (( *z8n)[i3 + 2] * ( *gamma)[i1][2])) + (( *z8n)[i3 + 3] * ( *gamma)[i1][3])) + (( *z8n)[i3 + 4] * ( *gamma)[i1][4])) + (( *z8n)[i3 + 5] * ( *gamma)[i1][5])) + (( *z8n)[i3 + 6] * ( *gamma)[i1][6])) + (( *z8n)[i3 + 7] * ( *gamma)[i1][7]));
      hourgam0[i1] = (( *gamma)[i1][0] - (volinv * (((( *dvdx)[i3] * hourmodx) + (( *dvdy)[i3] * hourmody)) + (( *dvdz)[i3] * hourmodz))));
      hourgam1[i1] = (( *gamma)[i1][1] - (volinv * (((( *dvdx)[i3 + 1] * hourmodx) + (( *dvdy)[i3 + 1] * hourmody)) + (( *dvdz)[i3 + 1] * hourmodz))));
      hourgam2[i1] = (( *gamma)[i1][2] - (volinv * (((( *dvdx)[i3 + 2] * hourmodx) + (( *dvdy)[i3 + 2] * hourmody)) + (( *dvdz)[i3 + 2] * hourmodz))));
      hourgam3[i1] = (( *gamma)[i1][3] - (volinv * (((( *dvdx)[i3 + 3] * hourmodx) + (( *dvdy)[i3 + 3] * hourmody)) + (( *dvdz)[i3 + 3] * hourmodz))));
      hourgam4[i1] = (( *gamma)[i1][4] - (volinv * (((( *dvdx)[i3 + 4] * hourmodx) + (( *dvdy)[i3 + 4] * hourmody)) + (( *dvdz)[i3 + 4] * hourmodz))));
      hourgam5[i1] = (( *gamma)[i1][5] - (volinv * (((( *dvdx)[i3 + 5] * hourmodx) + (( *dvdy)[i3 + 5] * hourmody)) + (( *dvdz)[i3 + 5] * hourmodz))));
      hourgam6[i1] = (( *gamma)[i1][6] - (volinv * (((( *dvdx)[i3 + 6] * hourmodx) + (( *dvdy)[i3 + 6] * hourmody)) + (( *dvdz)[i3 + 6] * hourmodz))));
      hourgam7[i1] = (( *gamma)[i1][7] - (volinv * (((( *dvdx)[i3 + 7] * hourmodx) + (( *dvdy)[i3 + 7] * hourmody)) + (( *dvdz)[i3 + 7] * hourmodz))));
    }
/* compute forces */
/* store forces into h arrays (force arrays) */
    ss1 = domain. ss (p_index_);
    mass1 = domain. elemMass (p_index_);
    volume13 = CBRT(( *determ)[p_index_]);
    Index_t n0si2 = elemToNode[0];
    Index_t n1si2 = elemToNode[1];
    Index_t n2si2 = elemToNode[2];
    Index_t n3si2 = elemToNode[3];
    Index_t n4si2 = elemToNode[4];
    Index_t n5si2 = elemToNode[5];
    Index_t n6si2 = elemToNode[6];
    Index_t n7si2 = elemToNode[7];
    xd1[0] = domain. xd (n0si2);
    xd1[1] = domain. xd (n1si2);
    xd1[2] = domain. xd (n2si2);
    xd1[3] = domain. xd (n3si2);
    xd1[4] = domain. xd (n4si2);
    xd1[5] = domain. xd (n5si2);
    xd1[6] = domain. xd (n6si2);
    xd1[7] = domain. xd (n7si2);
    yd1[0] = domain. yd (n0si2);
    yd1[1] = domain. yd (n1si2);
    yd1[2] = domain. yd (n2si2);
    yd1[3] = domain. yd (n3si2);
    yd1[4] = domain. yd (n4si2);
    yd1[5] = domain. yd (n5si2);
    yd1[6] = domain. yd (n6si2);
    yd1[7] = domain. yd (n7si2);
    zd1[0] = domain. zd (n0si2);
    zd1[1] = domain. zd (n1si2);
    zd1[2] = domain. zd (n2si2);
    zd1[3] = domain. zd (n3si2);
    zd1[4] = domain. zd (n4si2);
    zd1[5] = domain. zd (n5si2);
    zd1[6] = domain. zd (n6si2);
    zd1[7] = domain. zd (n7si2);
    coefficient = ((((-_p_hourg * (((Real_t )0.01))) * ss1) * mass1) / volume13);
    CalcElemFBHourglassForce(xd1,yd1,zd1,hourgam0,hourgam1,hourgam2,hourgam3,hourgam4,hourgam5,hourgam6,hourgam7,coefficient,hgfx,hgfy,hgfz);
    fx_local = ( *fx_elem + i3);
    fx_local[0] = hgfx[0];
    fx_local[1] = hgfx[1];
    fx_local[2] = hgfx[2];
    fx_local[3] = hgfx[3];
    fx_local[4] = hgfx[4];
    fx_local[5] = hgfx[5];
    fx_local[6] = hgfx[6];
    fx_local[7] = hgfx[7];
    fy_local = ( *fy_elem + i3);
    fy_local[0] = hgfy[0];
    fy_local[1] = hgfy[1];
    fy_local[2] = hgfy[2];
    fy_local[3] = hgfy[3];
    fy_local[4] = hgfy[4];
    fy_local[5] = hgfy[5];
    fy_local[6] = hgfy[6];
    fy_local[7] = hgfy[7];
    fz_local = ( *fz_elem + i3);
    fz_local[0] = hgfz[0];
    fz_local[1] = hgfz[1];
    fz_local[2] = hgfz[2];
    fz_local[3] = hgfz[3];
    fz_local[4] = hgfz[4];
    fz_local[5] = hgfz[5];
    fz_local[6] = hgfz[6];
    fz_local[7] = hgfz[7];
#if 0
#endif
  }
  XOMP_barrier();
}

static void OUT__28__1527__(void *__out_argv)
{
  Real_t **fx_elem = (Real_t **)(((struct OUT__28__1527___data *)__out_argv) -> OUT__28__1527___data::fx_elem_p);
  Real_t **fy_elem = (Real_t **)(((struct OUT__28__1527___data *)__out_argv) -> OUT__28__1527___data::fy_elem_p);
  Real_t **fz_elem = (Real_t **)(((struct OUT__28__1527___data *)__out_argv) -> OUT__28__1527___data::fz_elem_p);
  Index_t *numNode = (Index_t *)(((struct OUT__28__1527___data *)__out_argv) -> OUT__28__1527___data::numNode_p);
  Index_t _p_numNode =  *numNode;
  Index_t _p_gnode;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numNode - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    Index_t count = domain. nodeElemCount (p_index_);
    Index_t start = domain. nodeElemStart (p_index_);
    Real_t fx = ((Real_t )0.);
    Real_t fy = ((Real_t )0.);
    Real_t fz = ((Real_t )0.);
    for (Index_t i = (0); i < count; ++i) {
      Index_t elem = domain. nodeElemCornerList ((start + i));
      fx += ( *fx_elem)[elem];
      fy += ( *fy_elem)[elem];
      fz += ( *fz_elem)[elem];
    }
    domain. fx (p_index_) = fx;
    domain. fy (p_index_) = fy;
    domain. fz (p_index_) = fz;
  }
  XOMP_barrier();
}

static void OUT__29__1527__(void *__out_argv)
{
  Index_t *numElem = (Index_t *)(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::numElem_p);
  Real_t **sigxx = (Real_t **)(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::sigxx_p);
  Real_t **sigyy = (Real_t **)(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::sigyy_p);
  Real_t **sigzz = (Real_t **)(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::sigzz_p);
  Real_t **determ = (Real_t **)(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::determ_p);
  Real_t **fx_elem = (Real_t **)(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::fx_elem_p);
  Real_t **fy_elem = (Real_t **)(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::fy_elem_p);
  Real_t **fz_elem = (Real_t **)(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::fz_elem_p);
  Index_t _p_numElem =  *numElem;
  Index_t _p_k;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numElem - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
// shape function derivatives
    Real_t B[3UL][8UL];
    Real_t x_local[8UL];
    Real_t y_local[8UL];
    Real_t z_local[8UL];
    const Index_t *const elemNodes = (domain. nodelist (p_index_));
// get nodal coordinates from global arrays and copy into local arrays.
    for (Index_t lnode = (0); lnode < 8; ++lnode) {
      Index_t gnode = elemNodes[lnode];
      x_local[lnode] = domain. x (gnode);
      y_local[lnode] = domain. y (gnode);
      z_local[lnode] = domain. z (gnode);
    }
/* Volume calculation involves extra work for numerical consistency. */
    CalcElemShapeFunctionDerivatives((x_local),(y_local),(z_local),B,( *determ + p_index_));
    CalcElemNodeNormals(B[0],B[1],B[2],(x_local),(y_local),(z_local));
    SumElemStressesToNodeForces((B),( *sigxx)[p_index_],( *sigyy)[p_index_],( *sigzz)[p_index_],( *fx_elem + (p_index_ * 8)),( *fy_elem + (p_index_ * 8)),( *fz_elem + (p_index_ * 8)));
#if 0
// copy nodal force contributions to global force arrray.
#endif
  }
  XOMP_barrier();
}

static void OUT__30__1527__(void *__out_argv)
{
  Index_t *numElem = (Index_t *)(((struct OUT__30__1527___data *)__out_argv) -> OUT__30__1527___data::numElem_p);
  Real_t **sigxx = (Real_t **)(((struct OUT__30__1527___data *)__out_argv) -> OUT__30__1527___data::sigxx_p);
  Real_t **sigyy = (Real_t **)(((struct OUT__30__1527___data *)__out_argv) -> OUT__30__1527___data::sigyy_p);
  Real_t **sigzz = (Real_t **)(((struct OUT__30__1527___data *)__out_argv) -> OUT__30__1527___data::sigzz_p);
  Index_t _p_numElem =  *numElem;
  Index_t _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default((0),_p_numElem - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    ( *sigxx)[p_index_] = (( *sigyy)[p_index_] = (( *sigzz)[p_index_] = (-domain. p (p_index_) - domain. q (p_index_))));
  }
  XOMP_barrier();
}
