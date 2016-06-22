*******************************************************************************
LCALS: Livermore Compiler Analysis Loop Suite, version 1.0
 by Rich Hornung, Center for Applied Scientific Computing,
                  Lawrence Livermore National Laboratory

Unclassified/Unlimited Distribution
LLNL-CODE-638939  
OCEC-13-189

** NOTE: This code was originally released under the name LLoops21.
         The content is essentially unchanged under the new name.

*******************************************************************************

This code was developed and is maintained by Lawrence Livermore
National Laboratory (LLNL). It is intended to be shared widely with the 
HPC community (including other laboratories, universities, and industrial 
partners) as part of ASC and DOE exascale co-design efforts.

o The software is unrestricted in its distribution.

o LLNL retains copyright (see Copyright statement below)

o If the code and/or results generated from it are used in a publication,
  please cite LCALS as follows:

  @misc{LCALScode,
     author = {Richard D. Hornung},
     title = {{LCALS}, version 1.0},
     howpublished = {\texttt{https://codesign.llnl.gov/LCALS.php}},
     note = {{LLNL}-{CODE}-638939},
     year = {2013}
  }

o Please direct improvements, additions, comments, suggestions, etc. to 
  proxyapp-info@llnl.gov or hornung1@llnl.gov

o  This README-LCALS_license.txt file must be included in any redistribution of
   the software (either partial or in its entiretly) as well as any of its 
   derivatives.

*******************************************************************************
*******************************************************************************

This work was produced at Lawrence Livermore National Laboratory (LLNL) under
contract no. DE-AC52-07NA27344 (Contract 44) between the U.S. Department of
Energy (DOE) and Lawrence Livermore National Security, LLC (LLNS) for the
operation of LLNL. Copyright is reserved to Lawrence Livermore National
Security, LLC for purposes of controlled dissemination, commercialization
through formal licensing, or other disposition under terms of Contract 44; DOE
policies, regulations and orders; and U.S. statutes. The rights of the Federal
Government are reserved under Contract 44.

*******************************************************************************
*******************************************************************************

                                   DISCLAIMER

This work was prepared as an account of work sponsored by an agency of the
United States Government. Neither the United States Government nor Lawrence
Livermore National Security, LLC nor any of their employees, makes any warranty,
express or implied, or assumes any liability or responsibility for the accuracy,
completeness, or usefulness of any information, apparatus, product, or process
disclosed, or represents that its use would not infringe privately-owned rights.
Reference herein to any specific commercial products, process, or service by
trade name, trademark, manufacturer or otherwise does not necessarily constitute
or imply its endorsement, recommendation, or favoring by the United States
Government or Lawrence Livermore National Security, LLC. The views and opinions
of authors expressed herein do not necessarily state or reflect those of the
United States Government or Lawrence Livermore National Security, LLC, and shall
not be used for advertising or product endorsement purposes.

*******************************************************************************
*******************************************************************************

                           NOTIFICATION OF COMMERCIAL USE

Commercialization of this product is prohibited without notifying the 
Department of Energy (DOE) or Lawrence Livermore National Laboratory (LLNL).



*******************************************************************************
*******************************************************************************

//
// The following is the original copyright statement from Steve Langer's
// Livermore Loops coded in C.
//
// NOTE: Fonzi's Law (mentioned below) is actually called
//       Flon's Law (just Google it).
//

/* 
 *********************************************************************** 
 * 
 * Livermore Loops coded in C        Latest File Modification  27 Jul 90
 *
 * NOTE NOTE NOTE: Modified for use in the pure ANSI C version
 * of the LFK test program by Steven H. Langer.
 * Changes include calling sequence from Fortran to C and
 * minor changes in COMMON block arguments.
 * Split into separate header and source code files for convenience
 * in converting the main program to C.
 * Feb. 14, 1995.
 *
 * Copyright (c) 1995.  The Regents of the University of California.
 *                  All rights reserved.
 *
 *
 *     SUBROUTINE KERNEL( TK)  replaces the Fortran routine in LFK Test program.
 ************************************************************************
 *                                                                      *
 *          KERNEL     executes 24 samples of "C" numerical computation *
 *                                                                      *
 *                TK(1) - total cpu time to execute only the 24 kernels.*
 *                TK(2) - total Flops executed by the 24 Kernels        *
 *                                                                      *
 *   Link this C module with the rest of LFK Test compiled with Fortran *
 *   using a version of the LFK Test dated April 1990 or later.         *
 ************************************************************************
 *                                                                      *
 *     L. L. N. L.   " C "   K E R N E L S  T E S T:   M F L O P S      *
 *                                                                      *
 *     These kernels measure   " C "   numerical computation            *
 *     rates for  a  spectrum  of  cpu-limited computational            *
 *     structures or benchmarks.   Mathematical  through-put            *
 *     is measured  in  units  of millions of floating-point            *
 *     operations executed per second, called Megaflops/sec.            *
 *                                                                      *
 *     Fonzi's Law: There is not now and there never will be a language *
 *                  in which it is the least bit difficult to write     *
 *                  bad programs.                                       *
 *                                                                      *
 *Originally from  Greg Astfalk, AT&T, P.O.Box 900, Princeton, NJ. 08540*
 *by way of Frank McMahon, LLNL, PO Box 808, Livermore, CA, 94550. 1986 *
 *                                                                      *
 *    Changes made to correct many array subscripting problems,         *
 *      make more readable (added #define's), include the original      *
 *      FORTRAN versions of the runs as comments, and make more         *
 *      portable by Kelly O'Hair (LLNL) and Chuck Rasbold (LLNL)        *
 *           and by Mark Seager  (LLNL).                                *
 *                                                                      *
 *      please send copy of sdtout to:   MCMAHON3@LLNL.GOV              *
 *                                 or:   mcmahon@lll-crg.llnl.gov       *
 *                                                                      *
 ************************************************************************
 *                                                                      *
 *                               REFERENCE                              *
 *                                                                      *
 *              F.H.McMahon,   The Livermore Fortran Kernels:           *
 *              A Computer Test Of The Numerical Performance Range,     *
 *              Lawrence Livermore National Laboratory,                 *
 *              Livermore, California, UCRL-53745, December 1986.       *
 *                                                                      *
 *       from:  National Technical Information Service                  *
 *              U.S. Department of Commerce                             *
 *              5285 Port Royal Road                                    *
 *              Springfield, VA.  22161                                 *
 *                                                                      *
 *                                                                      *
 *                   (C) Copyright 1986 the Regents of the              *
 *               University of California. All Rights Reserved.         *
 *                                                                      *
 *              This work was produced under the sponsorship of         *
 *               the U.S. Department of Energy. The Government          *
 *                      retains certain rights therein.                 *
 *                                                                      *
 ************************************************************************
 */
