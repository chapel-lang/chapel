C StarPU --- Runtime system for heterogeneous multicore architectures.
C
C Copyright (C) 2010  Université de Bordeaux 1
C Copyright (C) 2010  Centre National de la Recherche Scientifique
C
C StarPU is free software; you can redistribute it and/or modify
C it under the terms of the GNU Lesser General Public License as published by
C the Free Software Foundation; either version 2.1 of the License, or (at
C your option) any later version.
C
C StarPU is distributed in the hope that it will be useful, but
C WITHOUT ANY WARRANTY; without even the implied warranty of
C MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
C
C See the GNU Lesser General Public License in COPYING.LGPL for more details.
C

      MODULE STARPU_FORTRAN
        USE ISO_C_BINDING

          TYPE codelet
              REAL :: A,B,C
          END TYPE codelet

      CONTAINS
      
          SUBROUTINE PRINT_INT(X)
              INTEGER :: X
              WRITE(*,*) 'X =', X
          END SUBROUTINE

          SUBROUTINE STARPU_SUBMIT_CODELET(CPUFUNC, ARG)
              INTEGER :: ARG

              INTERFACE
                  SUBROUTINE CPUFUNC(ARG)
                      INTEGER :: ARG
                  END SUBROUTINE
              END INTERFACE

              CALL CPUFUNC(ARG)
          END SUBROUTINE

      END MODULE STARPU_FORTRAN

      MODULE STARPU_FORTRAN2
        USE ISO_C_BINDING

      CONTAINS
          SUBROUTINE PRINT_INT2(X)
              INTEGER :: X
              WRITE(*,*) 'X =', X
          END SUBROUTINE

      END MODULE STARPU_FORTRAN2
