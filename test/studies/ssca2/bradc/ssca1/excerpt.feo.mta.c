void cb_sw_fw_score(swtab, dbllen, dbslen, eg, eog, smax)
  int *swtab, dbllen, dbslen, eg, eog, *smax;
{
  int i, j, l, k, *MAX2, *gaph, *gapv;

  *smax   = 0;
  dbllen += 1;
  dbslen += 1;

/* allocate memory for temporary storage */
  MAX2 = (int *) malloc(dbllen * sizeof(int));
  gaph = (int *) malloc(dbllen * dbslen * sizeof(int));
  gapv = (int *) malloc(dbllen * dbslen * sizeof(int));

/* set all but row 0 and column 0 to empty */
#pragma mta assert no dependence
  for (i = 1; i < dbllen; i++)
  for (j = 1; j < dbslen; j++) purge(swtab + i * dbslen + j);

/* initialize max and gap valued */
  for (i = 0; i < dbllen; i++) MAX2[i] = - eog;
  for (i = 0; i < dbslen; i++) gapv[i] = - eog - (i - 1) * eg;

#pragma mta assert no dependence
  for (i = 0; i < dbllen; i++) gaph[i * dbslen] = -eog - (i - 1) * eg;

#pragma mta assert parallel
#pragma mta use 100 streams
#pragma mta dynamic schedule
  for (k = 2; k < dbllen + dbslen; k++) {

      if (k < dbllen) {

         for (l = 1; l <= min(k, dbslen - 1); l++) {
             int i = k - l;
             int j = l;
             if (i != 0) diag(i,j,dbslen,eg,eog,smax,gaph,gapv,swtab,MAX2);
         }

      } else {

         for (l = dbllen; l > max(k - dbslen + 1, 0); l--) {
             int i = l - 1;
             int j = k - l + 1;
             if (i != 0) diag(i,j,dbslen,eg,eog,smax,gaph,gapv,swtab,MAX2);
  }   }  }


  free(MAX2); free(gaph); free(gapv);
}
