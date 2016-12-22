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
            ', stridable ', A._value.stridable);
    writeln('----');
  }

  for i in A.domain.dim(A._value.mdParDim) do
    write(' ', A._value.mdInd2Chunk(i));
  writeln();

  for iChunk in 0..#A._value.mdNumChunks do
    write(if iChunk == 0 then ' ' else ', ', A._value.mData(iChunk).pdr);
  writeln();

  if verbose {
    write(' -->');
    for iChunk in 0..#A._value.mdNumChunks do
      write(' ',
            _computeBlock(A._value.mdRLen, A._value.mdNumChunks, iChunk,
                          (A._value.mdRHi - A._value.mdRLo) / A._value.mdRStr,
                          0, 0));
    writeln();
  }

  writeln('====================');
}
