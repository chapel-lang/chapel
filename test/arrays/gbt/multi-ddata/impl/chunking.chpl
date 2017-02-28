config const verbose = false;

config var skipCount = 0;

config const N = defRectArrMultiDDataSizeThreshold * 3;

var A_1toN: [1..N] int = [i in 1..N] i;
reportChunking('A[1..N]', A_1toN);

var A_slice_2toNm1 => A_1toN[1+1..N-1];
reportChunking('A[...] => A[1+1..N-1]',
               A_slice_2toNm1);

var A_reindex_1toN: [1+5..N+5] => A_1toN;
reportChunking('A[1+5..N+5] => A[1..N]',
               A_reindex_1toN);

var A_1toNby2: [1..N by 2] int = [i in 1..N by 2] i;
reportChunking('A[1..N by 2]', A_1toNby2);

var A_1toNby3: [1..N by 3] int = [i in 1..N by 3] i;
reportChunking('A[1..N by 3]', A_1toNby3);

var A_alias_1toNby2 => A_1toN[1..N by 2];
reportChunking('A => A[1..N] sliced [1..N by 2]',
               A_alias_1toNby2);

var A_1toX_alias_1toNby2: [1..N/2] => A_1toN[1..N by 2];
reportChunking('A[1..N/2] => A[1..N] sliced [1..N by 2]',
               A_1toX_alias_1toNby2);

var A_5to6_alias_1toN_2N3dd2N3p2by2: [5..6] => A_1toN[2*N/3..2*N/3+2 by 2];
reportChunking('A[5..6] => A[1..N] sliced [2*N/3..2*N/3+2 by 2]',
               A_5to6_alias_1toN_2N3dd2N3p2by2);

var A_1toN_1toN: [1..N, 1..N] int;
for i in A_1toN_1toN.domain.dim(1) do
  for j in A_1toN_1toN.domain.dim(2) do
    A_1toN_1toN[i, j] = (i-1)*N+j;
reportChunking('A[1..N, 1..N]', A_1toN_1toN);
reportChunking('A[1..N, 1..N] sliced [1, 1..N]', A_1toN_1toN[1, 1..N]);
reportChunking('A[1..N, 1..N] sliced [N, 1..N]', A_1toN_1toN[N, 1..N]);


proc reportChunking(what, A) {
  skipCount -= 1;
  if skipCount >= 0 then return;

  writeln(what, ': ', A);

  if verbose {
    writeln('----');
    A.displayRepresentation();
    writeln(' dom.dsiDims() ', A._value.dom.dsiDims(),
            ', stridable ', chpl__getActualArray(A).stridable);
    writeln('----');
  }

  for i in A.domain.dim(chpl__getActualArray(A).mdParDim) do
    write(' ', chpl__getActualArray(A).mdInd2Chunk(i));
  writeln();

  for iChunk in 0..#chpl__getActualArray(A).mdNumChunks do
    write(if iChunk == 0 then ' ' else ', ', chpl__getActualArray(A).mData(iChunk).pdr);
  writeln();

  if verbose {
    write(' -->');
    for iChunk in 0..#chpl__getActualArray(A).mdNumChunks do
      write(' ',
            _computeBlock(chpl__getActualArray(A).mdRLen, chpl__getActualArray(A).mdNumChunks, iChunk,
                          (chpl__getActualArray(A).mdRHi - chpl__getActualArray(A).mdRLo) / chpl__getActualArray(A).mdRStr,
                          0, 0));
    writeln();
  }

  writeln('====================');
}
