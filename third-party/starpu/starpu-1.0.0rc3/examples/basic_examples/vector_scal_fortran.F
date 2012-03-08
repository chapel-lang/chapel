C
C StarPU --- Runtime system for heterogeneous multicore architectures.
C 
C Copyright (C) 2010  Centre National de la Recherche Scientifique
C Copyright (C) 2010  Institut National de Recherche en Informatique et Automatique
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

      PROGRAM VECTOR_SCAL
      INTEGER,PARAMETER :: F_NX=2048
      REAL,DIMENSION(F_NX) :: VECTOR

      INTEGER :: I
      DO I=1,F_NX,1
         VECTOR(I)=1.0
      ENDDO

      WRITE (*,*) ' BEFORE : First element was ', VECTOR(1)
      WRITE (*,*) ' BEFORE : Last element was ', VECTOR(F_NX)

      CALL COMPUTE(F_NX, VECTOR)

      WRITE (*,*) ' AFTER : First element is ', VECTOR(1)
      WRITE (*,*) ' AFTER : Last element is ', VECTOR(F_NX)

      END PROGRAM
