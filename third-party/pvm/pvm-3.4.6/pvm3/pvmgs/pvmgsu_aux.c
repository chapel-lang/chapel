
static char rcsid[] =
	"$Id: pvmgsu_aux.c,v 1.10 2007/04/19 23:15:33 pvmsrc Exp $";

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */

/*
 *	pvmgsu_aux.c - auxiliary group library routines, gather and scatter  
 *       6 Jun 1995     Native mode reduce for Paragon. Donato
 *       8 Mar 1994     Added reduce & assoc routines. Donato & P.Papadopoulos
 *      24 Apr 1994     Added scatter, gather, gs_get_datasize routines. Donato
 *
 */

#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#include <stdio.h>
#include <pvm3.h>
#include "pvmalloc.h"
#include "bfunc.h"
#include "lpvm.h"
#include <pvmtev.h>
#include "tevmac.h"
#include "global.h"
#include "pvmmimd.h"
#include "pvmgsd.h"
#include "pvmproto.h"

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

extern int pvm_errno;
extern int pvmmytid;
extern int pvmtoplvl;
extern struct Pvmtracer pvmtrc;



/* ==========                                         ================ 
 * ========== Declarations & Routines for the Paragon ================
 * ==========                                         ================ 
 * Note:  int and long are the same on the paragon 
 */

#if defined(IMA_PGON)

extern int pvmpgonpartsize;           /* from lpvmmimd.c */

/* ================ NativeFunction() for the PGON ====================     */
/* This routine compares the address of the user provided function
   to the built-in pvm functions to determine and then call the
   appropriate native mode function on the Paragon.
*/
int
NativeFunction(user_func, datatype, x, y, num)
#ifdef IMA_SCO
  void (*user_func)(int*, void*, void*, int*, int*);
#else
  void (*user_func)();
#endif
void *x, *y;
int  datatype, num;
{
    if (user_func==PvmSum)
    {
        switch(datatype)
        {
            case (PVM_INT):
            case (PVM_LONG):
                _gisum((int *) x, num, (int *) y);
                break;
            case (PVM_FLOAT):
                _gssum((float *) x, num, (float *) y);
                break;
            case (PVM_DOUBLE):
                _gdsum((double *) x, num, (double *) y);
                break;
            default:
                return(PvmNotImpl);
        }   /* end switch */
    }
    else
    if (user_func==PvmMax)
    {
        switch(datatype)
        {
            case (PVM_INT):
            case (PVM_LONG):
                _gihigh((int *) x, num, (int *) y);
                break;
            case (PVM_FLOAT):
                _gshigh((float *) x, num, (float *) y);
                break;
            case (PVM_DOUBLE):
                _gdhigh((double *) x, num, (double *) y);
                break;
            default:
                return(PvmNotImpl);
        }   /* end switch */
    }
    else
    if (user_func==PvmMin)
    {
        switch(datatype)
        {
            case (PVM_INT):
            case (PVM_LONG):
                _gilow((int *) x, num, (int *) y);
                break;
            case (PVM_FLOAT):
                _gslow((float *) x, num, (float *) y);
                break;
            case (PVM_DOUBLE):
                _gdlow((double *) x, num, (double *) y);
                break;
            default:
                return(PvmNotImpl);
        }   /* end switch */
    }
    else
    if (user_func==PvmProduct)
    {
        switch(datatype)
        {
            case (PVM_INT):
            case (PVM_LONG):
                _giprod((int *) x, num, (int *) y);
                break;
            case (PVM_FLOAT):
                _gsprod((float *) x, num, (float *) y);
                break;
            case (PVM_DOUBLE):
                _gdprod((double *) x, num, (double *) y);
                break;
            default:
                return(PvmNotImpl);
        }  /* end switch */
    }
    else
        return(PvmNotImpl);

    return(PvmOk);

}  /* end NativeFunction() */


#endif /* #ifdef PGON */



/* ================ pvm_reduce() =====================================     */
/*
  int info = pvm_reduce(void (*func)(), void *data, int count, int datatype,
                        int msgtag, char *gname, int rootinst)
  where
      void (*func)(int *datatype, void *data, void *work, int *num, int *info)

  Currently, this implementation uses a fan in algorithm to perform
  the reduce operation.

  Each group member sends their data to the coordinator on their host machine.
  The coordinator performs the specified function combining its own data and 
  the data from the group members on the same host.  
  
  On the Paragon the nx native global operations are utilized if all the
  nodes of the Paragon are part of the group.

  The coordinators then pass their results on to the specified root node 
  of the reduce operation.
*/

int pvm_reduce(func, data, count, datatype, msgtag, gname, rootinst)
#ifdef  IMA_SCO
    void (*func)(int*, void*, void*, int*, int*);
#else
    void (*func)();
#endif
    void *data;
    int count, datatype, msgtag, rootinst;
    char *gname; 
{
    int cnt, roottid, datasize, cc=PvmOk, rbuf, sbuf;
    int coordinator, nmembers_on_host, nhosts_in_group, mask=0;
    void *work = NULL;      /* work array to be allocated */

    int  (*packfunc)(), (*unpackfunc)();
    int x;
	int savectx;

	TEV_DECLS

    BGN_TRACE( TEV_REDUCE, gname, TEV_DID_MC, &msgtag );

    rbuf = pvm_setrbuf(0);            /* set receive buf */
    sbuf = pvm_mkbuf(PvmDataDefault);
    sbuf = pvm_setsbuf(sbuf);

	/* set context for dynamic groups */
	savectx = pvm_setcontext( SYSCTX_DG );

    if ( (data == NULL) || (count <= 0) ) /* check some parameters */
    {
      cc = PvmBadParam;
      goto done;
    }
  
    /* get instance number - caller must be in group, root must be in group */
    if ( (cc = pvm_getinst(gname, pvmmytid))            < PvmOk ) goto done;
    if ( (cc = roottid  = pvm_gettid (gname, rootinst)) < PvmOk ) goto done;
    if ( (cc = datasize = gs_get_datasize(datatype))    < PvmOk ) goto done;

    /* set up pointers to the appropriate pack and unpack routines */
    if ( (cc = gs_pack_unpack(datatype, &packfunc, &unpackfunc) ) < PvmOk)
        goto done;

    if ((work = (void *) PVM_ALLOC(count*datasize, "pvm_reduce")) == NULL)
    {
        cc = PvmNoMem;
        goto done;
    }

    pvm_grphostinfo(gname, gs_tidtohost(pvmmytid), &coordinator, 
                    &nmembers_on_host, &nhosts_in_group);

#if defined(IMA_PGON)
    /*  if all the nodes are participating,  
        then call the Native mode version, if one exists.
        The native function call is a side-effect of the call to NativeFunction.
        If the coordinator and roottid are PGON nodes, there is no
        need to differentiate (e.g. pass data from coordinator to roottid).
    */

    mask = TIDHOST | TIDPTYPE;

    if ( TIDISNODE(pvmmytid) && 
       ( pvmpgonpartsize == nmembers_on_host ) &&
       ( (cc = NativeFunction(func, datatype, data, work, count)) == PvmOk) )
    {
        if ( TIDISNODE(roottid) && (pvmmytid & mask) == (roottid & mask) )
            coordinator = roottid;
    }
    else
#endif    /* end if defined(IMA_PGON) */
    {
        if ( (pvmmytid==coordinator) && (nmembers_on_host>1) )
        {
            /* recv data from other group members on same host, perform func */
            for (cnt = nmembers_on_host-1; cnt>0; cnt--)
            {
                if ( (cc = pvm_recv(-1, msgtag) )          < PvmOk) goto done;
                if ( (cc = (*unpackfunc)( work, count, 1)) < PvmOk) goto done;
                (*func)( &datatype, data, work, &count, &cc );
                if (cc < PvmOk) goto done;           /* error flag from func */
            }  
        }
        else if (pvmmytid != coordinator)
        {
            /* send data to the data coordinator on this same host */
            pvm_initsend(PvmDataDefault);
            if ( (cc = (*packfunc)( data, count, 1) )  < PvmOk ) goto done;
            if ( (cc = pvm_send( coordinator, msgtag)) < PvmOk ) goto done;;
        }
    } 

    if ( (pvmmytid==coordinator) && (pvmmytid != roottid) )
    {
        /* send data to the roottid for the reduce operation */
        pvm_initsend(PvmDataDefault);
        if ( (cc = (*packfunc)( data, count, 1) )  < PvmOk ) goto done;
        if ( (cc = pvm_send( roottid, msgtag)) < PvmOk ) goto done;
    } 

    /* Root node of the reduce operation: 
       - get data from my host coordinator, if necessary
       - get work values from each of the other hosts
       - perform the specified functions on data from other hosts
    */
    if (pvmmytid == roottid) 
    {
        /* if root isn't the host coordinator, receive from coordinator 1st */
        if  (pvmmytid != coordinator) 
        {
            if ( (cc = pvm_recv(coordinator, msgtag) ) < PvmOk) goto done;
            if ( (cc = (*unpackfunc)( data, count, 1)) < PvmOk) goto done;
        }

        if (nhosts_in_group-- <=  0) goto done;

        /* recv data from other group members on diff host, perform func */
        for (cnt = nhosts_in_group; cnt>0; cnt--)
        {
            if ( (cc = pvm_recv(-1, msgtag) )          < PvmOk) goto done;
            if ( (cc = (*unpackfunc)( work, count, 1)) < PvmOk) goto done;
            (*func)( &datatype, data, work, &count, &cc );
            if (cc < PvmOk) goto done;               /* error flag from func */
        }   /* end for */
    }
    
    cc = PvmOk;
    
  done:
  
    /* restore user's buffers */
    pvm_freebuf(pvm_setrbuf(rbuf));
    pvm_freebuf(pvm_setsbuf(sbuf));

	/* restore user context */
	pvm_setcontext( savectx );

    if (work != NULL) PVM_FREE(work);         /* free work space  */

    if (cc < 0) lpvmerr("pvm_reduce",cc);
  
    END_TRACE( TEV_REDUCE, TEV_DID_CC, &cc );

    return(cc);

} /* end pvm_reduce() */



/* ================ gs_pack_unpack()==================================     */
/*  
    int info = gs_pack_unpack( int datatype, 
                               int (**packfunc)(), int (**unpackfunc)() )

    Sets up pointers to the appropriate pack and unpack function based
    on datatype specified.
*/

int 
gs_pack_unpack(datatype, packfunc, unpackfunc)
int datatype, (**packfunc)(), (**unpackfunc)();
{

    switch(datatype) 
    {
        case (PVM_STR):  
            *packfunc = pvm_pkstr; 
            *unpackfunc = pvm_upkstr; 
            break;
        case (PVM_BYTE):  
            *packfunc = pvm_pkbyte; 
            *unpackfunc = pvm_upkbyte; 
            break;
        case (PVM_SHORT):
            *packfunc = pvm_pkshort; 
            *unpackfunc = pvm_upkshort; 
            break;
        case (PVM_INT):
            *packfunc = pvm_pkint;
            *unpackfunc = pvm_upkint; 
            break;
        case (PVM_LONG):      
            *packfunc = pvm_pklong; 
            *unpackfunc = pvm_upklong; 
            break;
        case (PVM_FLOAT):
            *packfunc = pvm_pkfloat; 
            *unpackfunc = pvm_upkfloat; 
            break;
        case (PVM_DOUBLE):
            *packfunc = pvm_pkdouble; 
            *unpackfunc = pvm_upkdouble; 
            break;
        case (PVM_CPLX):
            *packfunc = pvm_pkcplx; 
            *unpackfunc = pvm_upkcplx; 
            break;
        case (PVM_DCPLX):
            *packfunc = pvm_pkdcplx; 
            *unpackfunc = pvm_upkdcplx; 
            break;
        default:
            return(PvmBadParam);
    }

    return(PvmOk);
}   
/* ================ PvmMax()==========================================     */
/* 
  void PvmMax(int *datatype, void *x, void *y, int *num, int *info)
  Assigns the elements of x the maximum value between the
  corresponding elements of x and y.  
  For complex values the maximum is determined by maximum modulus.
*/

void 
PvmMax(datatype, x, y, num, info)
int *datatype;
void *x, *y; 
int *num, *info;
{
    char   *xchar,   *ychar;
    int    *xint,    *yint;
    short  *xshort,  *yshort;
    long   *xlong,   *ylong;
    float  *xfloat,  *yfloat;
    double *xdouble, *ydouble;
    float   xfreal, xfimag, yfreal, yfimag;
    double  xdreal, xdimag, ydreal, ydimag;
    float   xsqrfloat, ysqrfloat;
    double  xsqrdouble, ysqrdouble;
    
    int i, count;
  
    count = *num;
  
    switch(*datatype) 
    {
        case (PVM_BYTE):
            xchar = (char *) x;
            ychar = (char *) y;
            for (i=0; i<count; i++) xchar[i] = MAX(xchar[i], ychar[i]);
            break;
        case (PVM_SHORT):
            xshort = (short *) x;
            yshort = (short *) y;
            for (i=0; i<count; i++) xshort[i] = MAX(xshort[i], yshort[i]);
            break;
        case (PVM_INT):
            xint = (int *) x;
            yint = (int *) y;
            for (i=0; i<count; i++) xint[i] = MAX(xint[i], yint[i]);
            break;
        case (PVM_LONG):
            xlong = (long *) x;
            ylong = (long *) y;
            for (i=0; i<count; i++) xlong[i] = MAX(xlong[i], ylong[i]);
            break;
        case (PVM_FLOAT):
            xfloat = (float *) x;
            yfloat = (float *) y;
            for (i=0; i<count; i++) xfloat[i] = MAX(xfloat[i], yfloat[i]);
            break;
        case (PVM_DOUBLE):
            xdouble = (double *) x;
            ydouble = (double *) y;
            for (i=0; i<count; i++) xdouble[i] = MAX(xdouble[i], ydouble[i]);
            break;
        case (PVM_CPLX):
            /* complex - complex*8 in fortran - treated as two floats */
            /* returns the complex pair with the greatest magnitude */
            xfloat = (float *) x;
            yfloat = (float *) y;
            for (i=0; i<2*count; i+=2)
            {
                xfreal = xfloat[i];
                xfimag = xfloat[i+1];
                yfreal = yfloat[i];
                yfimag = yfloat[i+1];
                xsqrfloat = xfreal*xfreal + xfimag*xfimag;
                ysqrfloat = yfreal*yfreal + yfimag*yfimag;
                if (ysqrfloat > xsqrfloat)
                {
                    xfloat[i]   = yfreal;
                    xfloat[i+1] = yfimag;
                }
            }
            break;
        case (PVM_DCPLX):
            /* double complex - complex*16 in fortran - treated as 2 doubles */
            /* returns the complex pair with the greatest magnitude */
            xdouble = (double *) x;
            ydouble = (double *) y;
            for (i=0; i<2*count; i+=2)
            {
                xdreal = xdouble[i];
                xdimag = xdouble[i+1];
                ydreal = ydouble[i];
                ydimag = ydouble[i+1];
                xsqrdouble = xdreal*xdreal + xdimag*xdimag;
                ysqrdouble = ydreal*ydreal + ydimag*ydimag;
                if (ysqrdouble > xsqrdouble)
                {
                    xdouble[i]   = ydreal;
                    xdouble[i+1] = ydimag;
                }
            }
            break;
        default:
            *info = PvmBadParam;
            return;
    }  /* end switch */

    *info = PvmOk;
    return;

}  /* end of PvmMax() */



/* ================ PvmMin()==========================================     */
/* 
  void PvmMin(int *datatype, void *x, void *y, int *num, int *info)

  Assigns the elements of x the minimum value between the
  corresponding elements of x and y.
  For complex values the minimum is determined by minimum modulus.

*/

void 
PvmMin(datatype, x, y, num, info)
int *datatype;
void *x, *y;
int  *num, *info;
{
    char   *xchar,   *ychar;
    short  *xshort,  *yshort;
    int    *xint,    *yint;
    long   *xlong,   *ylong;
    float  *xfloat,  *yfloat;
    double *xdouble, *ydouble;
    float   xfreal, xfimag, yfreal, yfimag;
    double  xdreal, xdimag, ydreal, ydimag;
    float   xsqrfloat, ysqrfloat;
    double  xsqrdouble, ysqrdouble;
  
    int i, count;
  
    count = *num;
  
    switch(*datatype) 
    {
        case (PVM_BYTE):
            xchar = (char *) x;
            ychar = (char *) y;
            for (i=0; i<count; i++) xchar[i] = MIN(xchar[i], ychar[i]);
            break;
        case (PVM_SHORT):
            xshort = (short *) x;
            yshort = (short *) y;
            for (i=0; i<count; i++) xshort[i] = MIN(xshort[i], yshort[i]);
            break;
        case (PVM_INT):
            xint = (int *) x;
            yint = (int *) y;
            for (i=0; i<count; i++) xint[i] = MIN(xint[i], yint[i]);
            break;
        case (PVM_LONG):
            xlong = (long *) x;
            ylong = (long *) y;
            for (i=0; i<count; i++) xlong[i] = MIN(xlong[i], ylong[i]);
            break;
        case (PVM_FLOAT):
            xfloat = (float *) x;
            yfloat = (float *) y;
            for (i=0; i<count; i++) xfloat[i] = MIN(xfloat[i], yfloat[i]);
            break;
        case (PVM_DOUBLE):
            xdouble = (double *) x;
            ydouble = (double *) y;
            for (i=0; i<count; i++) xdouble[i] = MIN(xdouble[i], ydouble[i]);
            break;
        case (PVM_CPLX):
            /* complex - complex*8 in fortran - treated as two floats */
            /* returns the complex pair with the smaller magnitude */
            xfloat = (float *) x;
            yfloat = (float *) y;
            for (i=0; i<2*count; i+=2)
            {
                xfreal = xfloat[i];
                xfimag = xfloat[i+1];
                yfreal = yfloat[i];
                yfimag = yfloat[i+1];
                xsqrfloat = xfreal*xfreal + xfimag*xfimag;
                ysqrfloat = yfreal*yfreal + yfimag*yfimag;
                if (ysqrfloat < xsqrfloat)
                {
                    xfloat[i]   = yfreal;
                    xfloat[i+1] = yfimag;
                }
            }
            break;
        case (PVM_DCPLX):
            /* double complex - complex*16 in fortran - treated as 2 doubles */
            /* returns the complex pair with the smaller magnitude */
            xdouble = (double *) x;
            ydouble = (double *) y;
            for (i=0; i<2*count; i+=2)
            {
                xdreal = xdouble[i];
                xdimag = xdouble[i+1];
                ydreal = ydouble[i];
                ydimag = ydouble[i+1];
                xsqrdouble = xdreal*xdreal + xdimag*xdimag;
                ysqrdouble = ydreal*ydreal + ydimag*ydimag;
                if (ysqrdouble < xsqrdouble)
                {
                    xdouble[i]   = ydreal;
                    xdouble[i+1] = ydimag;
                }
            }
            break;
        default:
            *info = PvmBadParam;
            return;
    }  /* end switch */
     
    *info = PvmOk;
    return;

}  /* end of PvmMin() */


/* ================ PvmSum()==========================================     */

/* 
  void PvmSum(int *datatype, void *x, void *y, *num, *info)

  Assigns the elements of x the sum of the corresponding elements of x and y.
*/

void 
PvmSum(datatype, x, y, num, info)
int *datatype;
void *x, *y;
int *num, *info;
{
    short  *xshort,  *yshort;
    int    *xint,    *yint;
    long   *xlong,   *ylong;
    float  *xfloat,  *yfloat;
    double *xdouble, *ydouble;
  
    int i, count;
  
    count = *num;
  
    switch(*datatype) 
    {
        case (PVM_SHORT):
            xshort = (short *) x;
            yshort = (short *) y;
            for (i=0; i<count; i++) xshort[i] += yshort[i];
            break;
        case (PVM_INT):
            xint = (int *) x;
            yint = (int *) y;
            for (i=0; i<count; i++) xint[i] += yint[i];
            break;
        case (PVM_LONG):
            xlong = (long *) x;
            ylong = (long *) y;
            for (i=0; i<count; i++) xlong[i] += ylong[i];
            break;
        case (PVM_FLOAT):
            xfloat = (float *) x;
            yfloat = (float *) y;
            for (i=0; i<count; i++) xfloat[i] += yfloat[i];
            break;
        case (PVM_DOUBLE):
            xdouble = (double *) x;
            ydouble = (double *) y;
            for (i=0; i<count; i++) xdouble[i] += ydouble[i];
            break;
        case (PVM_CPLX):
            /* complex - complex*8 in fortran - treated as two floats */
            /* returns the sum of the two complex pairs */
            xfloat = (float *) x;
            yfloat = (float *) y;
            for (i=0; i<2*count; i++) xfloat[i]  += yfloat[i];
            break;
        case (PVM_DCPLX):
            /* double complex - complex*16 in fortran - treated as 2 doubles */
            /* returns the sum of the two complex pairs */
            xdouble = (double *) x;
            ydouble = (double *) y;
            for (i=0; i<2*count; i++) xdouble[i]   += ydouble[i];
            break;
        default:
            *info = PvmBadParam;
            return;
    }  /* end switch */

    *info = PvmOk;
    return;

}  /* end of PvmSum() */


/* ================ PvmProduct()======================================     */
/* 
  void PvmProduct(int *datatype, void *x, void *y, *num, *info)

  Assigns the elements of x the sum of the corresponding elements of x and y.
*/

void 
PvmProduct(datatype, x, y, num, info)
int *datatype;
void *x, *y;
int *num, *info;
{
    short  *xshort,  *yshort;
    int    *xint,    *yint;
    long   *xlong,   *ylong;
    float  *xfloat,  *yfloat, a,b,c,d;
    double *xdouble, *ydouble, da,db,dc,dd;
  
    int i, count;
  
    count = *num;
  
    switch(*datatype) 
    {
        case (PVM_SHORT):
            xshort = (short *) x;
            yshort = (short *) y;
            for (i=0; i<count; i++) xshort[i] *= yshort[i];
            break;
        case (PVM_INT):
            xint = (int *) x;
            yint = (int *) y;
            for (i=0; i<count; i++) xint[i] *= yint[i];
            break;
        case (PVM_LONG):
            xlong = (long *) x;
            ylong = (long *) y;
            for (i=0; i<count; i++) xlong[i] *= ylong[i];
            break;
        case (PVM_FLOAT):
            xfloat = (float *) x;
            yfloat = (float *) y;
            for (i=0; i<count; i++) xfloat[i] *= yfloat[i];
            break;
        case (PVM_DOUBLE):
            xdouble = (double *) x;
            ydouble = (double *) y;
            for (i=0; i<count; i++) xdouble[i] *= ydouble[i];
            break;
        case (PVM_CPLX):
            /* complex - complex*8 in fortran - treated as two floats */
            /* returns the product of the two complex pairs */
            xfloat = (float *) x;
            yfloat = (float *) y;
            for (i=0; i<2*count; i += 2) 
            {
                a = xfloat[i]; b = xfloat[i+1], c = yfloat[i]; d = yfloat[i+1];
                xfloat[i] = a*c - b*d;
                xfloat[i+1] = a*d + b*c;
            }
            break;
        case (PVM_DCPLX):
            /* double complex - complex*16 in fortran - treated as 2 doubles */
            /* returns the  product of the two complex pairs */
            xdouble = (double *) x;
            ydouble = (double *) y;
            for (i=0; i<2*count; i+= 2) 
            {
                da = xdouble[i]; db = xdouble[i+1], 
                dc = ydouble[i]; dd = ydouble[i+1];
                xdouble[i] = da*dc - db*dd;
                xdouble[i+1] = da*dd + db*dc;
            }
            break;
        default:
            *info = PvmBadParam;
            return;
    }  /* end switch */

    *info = PvmOk;
    return;

}  /* end of PvmProduct() */


/* ================ pvm_gather()======================================     */
/*    
  int info = pvm_gather(void *result, void *data, int count, int datatype, 
                        int msgtag,  char *gname, int rootinst)

  Performs a gather of messages from each member of the group
  to a specified member of the group.

  Each member of the group 'gname' sends a message 'data' 
  of type 'datatype' and length 'count' to the root member of the group.
  The root receives these messages into a single array 'result'
  which is of length, at least, (number of group members)*'count'.
  The values received from the ith member of the group are
  placed into the 'result' array starting at position i*'count'.
  The root member of the group is specified by its instance number,
  'rootginst', in that group.
*/
 
int 
pvm_gather(result, data, count, datatype, msgtag, gname, rootinst)
void *result, *data;
int  count, datatype, msgtag, rootinst;
char *gname;
{
    int mytid, roottid, myginst, datasize, gsize, *tids = 0, i, cc;
    int sbuf, rbuf;
  
    int (*packfunc)(), (*unpackfunc)();  /* ptrs to pack and unpack functions */
    int x;
	int savectx;
  
	TEV_DECLS

    BGN_TRACE( TEV_GATHER, gname, TEV_DID_MC, &msgtag );

	/* set context for dynamic groups */
	savectx = pvm_setcontext( SYSCTX_DG );

    if ( (data == NULL) || (count <= 0) ) /* check some parameters */
    {
        cc = PvmBadParam;
        goto done;
    }
  
    /* set up pointers to the appropriate pack and unpack routines */
    if ( (cc = gs_pack_unpack(datatype, &packfunc, &unpackfunc) ) < 0)
        goto done;
  
    /* root must be member of the group */
    if ( (cc = roottid = pvm_gettid(gname,rootinst)) < 0 ) goto done;
  
    /* get instance number - caller must be in group */
    if ( (cc = myginst = pvm_getinst(gname, pvmmytid)) < 0 ) goto done;

    if (myginst == rootinst)     /* I am the root for the gather operation */
    {
        if ( result == NULL) /* check result parameter */
        {
            cc = PvmBadParam;
            goto done;
        }
  
        /* get the number of bytes per element of type datatype */
        if ( (cc = datasize = gs_get_datasize(datatype)) < 0  ) goto done;
  
        /* Get the list of tids.  These must be contiguous (no holes). */
        if ( (cc = gs_get_tidlist(gname, msgtag, &gsize, &tids, 1)) < 0)
            goto done;
  
        rbuf = pvm_setrbuf(0);
  
        /* Get the values, put them in the correct place in the result. 
           The instance numbers should be contiguous within the group.
        */
        for (i=0; i<gsize; i++)
        {
            /* The root copies its data into its result array */
            if (i == myginst) 
            {
                BCOPY((char *) data, (char *) result + i*datasize*count, 
                      datasize*count);
            }
            else
            {
                if ( (cc = pvm_recv( tids[i], msgtag )) < 0 )
                {
                    pvm_freebuf(pvm_setrbuf(rbuf)); /* restore user recv buf*/
                    goto done;
                }
                if ((cc=(*unpackfunc)((char *)result+i*datasize*count,count,1))<0)
                {
                    pvm_freebuf(pvm_setrbuf(rbuf)); /* restore user recv buf*/
                    goto done;
                }
  
            } /* end if (i == myginst) */
  
        } /* end for-loop */
  
        pvm_freebuf(pvm_setrbuf(rbuf)); /* restore user's receive buf */
    }
    else  
    {    /* everyone except the root sends data to the root */
        sbuf = pvm_mkbuf(PvmDataDefault);
        sbuf = pvm_setsbuf(sbuf);
     
        if ( (cc = (*packfunc)( data, count, 1)) < 0)
        {
          pvm_freebuf(pvm_setsbuf(sbuf)); /* restore user's send buf */
          goto done;
        }
        if ( (cc = pvm_send( roottid, msgtag)) < 0)
        {
          pvm_freebuf(pvm_setsbuf(sbuf)); /* restore user's send buf */
          goto done;
        }
        pvm_freebuf(pvm_setsbuf(sbuf)); /* restore user's send buf */
  
    }  /* end if-else */


  cc = PvmOk;

done:

	/* restore user context */
	pvm_setcontext( savectx );

    if (tids) free(tids);
    
    if (cc < 0) lpvmerr("pvm_gather",cc);
  
    END_TRACE( TEV_GATHER, TEV_DID_CC, &cc );

    return(cc);

}  /* end pvm_gather() */

/* ================ pvm_scatter()=====================================     */
/*    
  int info = pvm_scatter(void *result, int *data,  int count, int datatype, 
                         void msgtag, char *gname, int rootinst)

  Performs a scatter of messages from the specified root member of the
  group to each of the members of the group.

  Each member of the group 'gname' receives a message 'result' 
  of type 'datatype' and length 'count' from the root member of the group.
  The root sends these messages from a single array 'data'
  which is of length, at least, (number of group members)*'count'.
  The values sent to the ith member of the group are
  taken from the 'data' array starting at position i*'count'.
  The root member of the group is specified by its instance number,
  'rootginst', in that group.
*/
 
int 
pvm_scatter(result, data, count, datatype, msgtag, gname, rootinst)
void *result, *data;
int  count, datatype, msgtag, rootinst;
char *gname;
{
    int roottid, myginst, datasize, gsize, *tids = 0, i, cc;
    int sbuf, rbuf;

    int (*packfunc)(), (*unpackfunc)();  /* ptrs to pack and unpack functions */
    int x;
	int savectx;

	TEV_DECLS

    BGN_TRACE( TEV_SCATTER, gname, TEV_DID_MC, &msgtag );

	/* set context for dynamic groups */
	savectx = pvm_setcontext( SYSCTX_DG );

    if ( (result == NULL) || (count <= 0) ) /* check some parameters */
    {
        cc = PvmBadParam;
        goto done;
    }

    /* set up pointers to the appropriate pack and unpack routines */
    if ( (cc = gs_pack_unpack(datatype, &packfunc, &unpackfunc)) < 0)
        goto done;

    /* root must be member of the group */
    if ( (roottid = pvm_gettid(gname,rootinst)) < 0 )
    {
        cc = roottid;
        goto done;
    }

    /* get instance number - caller must be in group */
    if ( (cc = myginst = pvm_getinst(gname, pvmmytid)) < 0 ) goto done;

    /* I am the root node for the scatter operation */
    if (myginst == rootinst)
    {
        if ( data == NULL) /* check data parameter */
        {
            cc = PvmBadParam;
            goto done;
        }

        /* get the number of bytes per element of type datatype */
        if ( (cc = datasize = gs_get_datasize(datatype)) < 0  ) goto done;
  
        /* Get the list of tids.  These must be contiguous (no holes). */
        if ( (cc = gs_get_tidlist(gname, msgtag, &gsize, &tids, 1)) < 0)
            goto done;
  
        sbuf = pvm_mkbuf(PvmDataDefault);
  
        /* The root sends values to everyone, except itself, in the group.
           For itself, the root copies the data into its result array.
        */
        for (i=0; i<gsize; i++)
        {
            if (i == myginst)
              BCOPY((char *) data + i*datasize*count, (char *) result,
                    datasize*count);
            else
            {
                sbuf = pvm_initsend(PvmDataDefault);
                if ((cc=(*packfunc)((char *)data+i*datasize*count,count,1))<0)
                {
                pvm_freebuf(pvm_setsbuf(sbuf));   /* restore user's sendbuf */
                goto done;
                }
                if ( (cc = pvm_send( tids[i], msgtag)) < 0)
                {
                    pvm_freebuf(pvm_setsbuf(sbuf)); /* restore user's sendbuf */
                    goto done;
                }
            } /* end if-else */
        } /* end for-loop */
        pvm_freebuf(pvm_setsbuf(sbuf)); /* restore user's send buf */
    }
    else
    {
        /* everyone receives a result from the root, except the root */
        rbuf = pvm_setrbuf(0);
        if ( (cc = pvm_recv( roottid, msgtag )) < 0)
        {
            pvm_freebuf(pvm_setrbuf(rbuf)); /* restore user's receive buf */
            goto done;
        }
        if ( (cc = (*unpackfunc)( result, count, 1)) < 0)
        {
            pvm_freebuf(pvm_setrbuf(rbuf)); /* restore user's receive buf */
            goto done;
        }
        pvm_freebuf(pvm_setrbuf(rbuf)); /* restore user's receive buf */
    
    }  /* end if-else */
	
    cc = PvmOk;

done:

	/* restore user context */
	pvm_setcontext( savectx );

    if (tids) free(tids);
  
    if (cc < 0) lpvmerr("pvm_scatter",cc);

    END_TRACE( TEV_SCATTER, TEV_DID_CC, &cc);

    return(cc);

}  /* end pvm_scatter() */

/* ================ gs_get_datasize() ================================     */
/*    
  int info = gs_get_datasize(int datatype)

  Returns the size in bytes of a single element of type datatype.
*/

int 
gs_get_datasize(datatype)
int datatype;
{

    switch (datatype)
    {
        case (PVM_STR):
        case (PVM_BYTE):
            return(sizeof(char));
        case (PVM_SHORT):
            return(sizeof(short));
        case (PVM_INT):
            return(sizeof(int));
        case (PVM_LONG):
            return(sizeof(long));
        case (PVM_FLOAT):
            return(sizeof(float));
        case (PVM_DOUBLE):
            return(sizeof(double));
        case (PVM_CPLX):
            return(2*sizeof(float));
        case (PVM_DCPLX):
            return(2*sizeof(double));
        default:
            return(PvmBadParam);
    }  /* end switch (datatype) */

}  /* end gs_get_datasize() */
