/*  Initialize  */

for (i = 1; i < Ilen; i++)
 
   for (j = 1, j < Jlen; j++) {

      purge (SWtab[i, j]) ;  /* set all but row 0 and column 0 to empty */

   }

}

/*  Main Smith-Waterman scoring loop */

#pragma dynamic schedule

for (k = 2; k < numDiag; k++) {    /* loop over anit-diagonals */

   for (l = 1; l < lenDiag(k); l++) {   /* scan each anti-diagonal */

      i = calcI(l,k) ;   /* get coordinates in matrix */
      j = calcJ(l,k) ;

      neighborN = readff(SWtab[i-1, j] ;

      neighborE = readff(SWtab[i, j-1] ;  /* read if set to full */

      neighborNE = readff(SWtab[i-1, j-1] ;

      Max = SWCalc(i, j, neighborN, neighborE, neighborNE) ;
   
      writeef(SWtab[i,j], Max) ;   /* set to full after calculating */
   }

}




