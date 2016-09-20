/**********************************************************************************************/
/*  This program is part of the Barcelona OpenMP Tasks Suite                                  */
/*  Copyright (C) 2009 Barcelona Supercomputing Center - Centro Nacional de Supercomputacion  */
/*  Copyright (C) 2009 Universitat Politecnica de Catalunya                                   */
/*                                                                                            */
/**********************************************************************************************/
/*
 * Copyright (c) 1996 Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to use, copy, modify, and distribute the Software without
 * restriction, provided the Software, including any modified copies made
 * under this license, is not distributed for a fee, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE MASSACHUSETTS INSTITUTE OF TECHNOLOGY BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * /WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the Massachusetts
 * Institute of Technology shall not be used in advertising or otherwise
 * to promote the sale, use or other dealings in this Software without
 * prior written authorization from the Massachusetts Institute of
 * Technology.
 *
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "app-desc.h"
#include "bots.h"
#include "strassen.h"
/***********************************************************************
 * Naive sequential algorithm, for comparison purposes
 **********************************************************************/
#include "libxomp.h" 

void matrixmul(int n,REAL *A,int an,REAL *B,int bn,REAL *C,int cn)
{
  int i;
  int j;
  int k;
  REAL s;
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      s = (0.0);
      for (k = 0; k < n; ++k) 
        s += (A[(i * an) + k] * B[(k * bn) + j]);
      C[(i * cn) + j] = s;
    }
  }
}
/*****************************************************************************
**
** FastNaiveMatrixMultiply
**
** For small to medium sized matrices A, B, and C of size
** MatrixSize * MatrixSize this function performs the operation
** C = A x B efficiently.
**
** Note MatrixSize must be divisible by 8.
**
** INPUT:
**    C = (*C WRITE) Address of top left element of matrix C.
**    A = (*A IS READ ONLY) Address of top left element of matrix A.
**    B = (*B IS READ ONLY) Address of top left element of matrix B.
**    MatrixSize = Size of matrices (for n*n matrix, MatrixSize = n)
**    RowWidthA = Number of elements in memory between A[x,y] and A[x,y+1]
**    RowWidthB = Number of elements in memory between B[x,y] and B[x,y+1]
**    RowWidthC = Number of elements in memory between C[x,y] and C[x,y+1]
**
** OUTPUT:
**    C = (*C WRITE) Matrix C contains A x B. (Initial value of *C undefined.)
**
*****************************************************************************/

void FastNaiveMatrixMultiply(REAL *C,REAL *A,REAL *B,unsigned int MatrixSize,unsigned int RowWidthC,unsigned int RowWidthA,unsigned int RowWidthB)
{
/* Assumes size of real is 8 bytes */
  PTR RowWidthBInBytes = (RowWidthB << 3);
  PTR RowWidthAInBytes = (RowWidthA << 3);
  PTR MatrixWidthInBytes = (MatrixSize << 3);
  PTR RowIncrementC = ((RowWidthC - MatrixSize) << 3);
  unsigned int Horizontal;
  unsigned int Vertical;
  REAL *ARowStart = A;
  for (Vertical = (0); Vertical < MatrixSize; Vertical++) {
    for (Horizontal = (0); Horizontal < MatrixSize; Horizontal += (8)) {
      REAL *BColumnStart = (B + Horizontal);
      REAL FirstARowValue =  *(ARowStart++);
      REAL Sum0 = (FirstARowValue *  *BColumnStart);
      REAL Sum1 = (FirstARowValue * BColumnStart[1]);
      REAL Sum2 = (FirstARowValue * BColumnStart[2]);
      REAL Sum3 = (FirstARowValue * BColumnStart[3]);
      REAL Sum4 = (FirstARowValue * BColumnStart[4]);
      REAL Sum5 = (FirstARowValue * BColumnStart[5]);
      REAL Sum6 = (FirstARowValue * BColumnStart[6]);
      REAL Sum7 = (FirstARowValue * BColumnStart[7]);
      unsigned int Products;
      for (Products = (1); Products < MatrixSize; Products++) {
        REAL ARowValue =  *(ARowStart++);
        BColumnStart = ((REAL *)(((PTR )BColumnStart) + RowWidthBInBytes));
        Sum0 += (ARowValue *  *BColumnStart);
        Sum1 += (ARowValue * BColumnStart[1]);
        Sum2 += (ARowValue * BColumnStart[2]);
        Sum3 += (ARowValue * BColumnStart[3]);
        Sum4 += (ARowValue * BColumnStart[4]);
        Sum5 += (ARowValue * BColumnStart[5]);
        Sum6 += (ARowValue * BColumnStart[6]);
        Sum7 += (ARowValue * BColumnStart[7]);
      }
      ARowStart = ((REAL *)(((PTR )ARowStart) - MatrixWidthInBytes));
       *C = Sum0;
       *(C + 1) = Sum1;
       *(C + 2) = Sum2;
       *(C + 3) = Sum3;
       *(C + 4) = Sum4;
       *(C + 5) = Sum5;
       *(C + 6) = Sum6;
       *(C + 7) = Sum7;
      C += 8;
    }
    ARowStart = ((REAL *)(((PTR )ARowStart) + RowWidthAInBytes));
    C = ((REAL *)(((PTR )C) + RowIncrementC));
  }
}
/*****************************************************************************
**
** FastAdditiveNaiveMatrixMultiply
**
** For small to medium sized matrices A, B, and C of size
** MatrixSize * MatrixSize this function performs the operation
** C += A x B efficiently.
**
** Note MatrixSize must be divisible by 8.
**
** INPUT:
**    C = (*C READ/WRITE) Address of top left element of matrix C.
**    A = (*A IS READ ONLY) Address of top left element of matrix A.
**    B = (*B IS READ ONLY) Address of top left element of matrix B.
**    MatrixSize = Size of matrices (for n*n matrix, MatrixSize = n)
**    RowWidthA = Number of elements in memory between A[x,y] and A[x,y+1]
**    RowWidthB = Number of elements in memory between B[x,y] and B[x,y+1]
**    RowWidthC = Number of elements in memory between C[x,y] and C[x,y+1]
**
** OUTPUT:
**    C = (*C READ/WRITE) Matrix C contains C + A x B.
**
*****************************************************************************/

void FastAdditiveNaiveMatrixMultiply(REAL *C,REAL *A,REAL *B,unsigned int MatrixSize,unsigned int RowWidthC,unsigned int RowWidthA,unsigned int RowWidthB)
{
/* Assumes size of real is 8 bytes */
  PTR RowWidthBInBytes = (RowWidthB << 3);
  PTR RowWidthAInBytes = (RowWidthA << 3);
  PTR MatrixWidthInBytes = (MatrixSize << 3);
  PTR RowIncrementC = ((RowWidthC - MatrixSize) << 3);
  unsigned int Horizontal;
  unsigned int Vertical;
  REAL *ARowStart = A;
  for (Vertical = (0); Vertical < MatrixSize; Vertical++) {
    for (Horizontal = (0); Horizontal < MatrixSize; Horizontal += (8)) {
      REAL *BColumnStart = (B + Horizontal);
      REAL Sum0 =  *C;
      REAL Sum1 = C[1];
      REAL Sum2 = C[2];
      REAL Sum3 = C[3];
      REAL Sum4 = C[4];
      REAL Sum5 = C[5];
      REAL Sum6 = C[6];
      REAL Sum7 = C[7];
      unsigned int Products;
      for (Products = (0); Products < MatrixSize; Products++) {
        REAL ARowValue =  *(ARowStart++);
        Sum0 += (ARowValue *  *BColumnStart);
        Sum1 += (ARowValue * BColumnStart[1]);
        Sum2 += (ARowValue * BColumnStart[2]);
        Sum3 += (ARowValue * BColumnStart[3]);
        Sum4 += (ARowValue * BColumnStart[4]);
        Sum5 += (ARowValue * BColumnStart[5]);
        Sum6 += (ARowValue * BColumnStart[6]);
        Sum7 += (ARowValue * BColumnStart[7]);
        BColumnStart = ((REAL *)(((PTR )BColumnStart) + RowWidthBInBytes));
      }
      ARowStart = ((REAL *)(((PTR )ARowStart) - MatrixWidthInBytes));
       *C = Sum0;
       *(C + 1) = Sum1;
       *(C + 2) = Sum2;
       *(C + 3) = Sum3;
       *(C + 4) = Sum4;
       *(C + 5) = Sum5;
       *(C + 6) = Sum6;
       *(C + 7) = Sum7;
      C += 8;
    }
    ARowStart = ((REAL *)(((PTR )ARowStart) + RowWidthAInBytes));
    C = ((REAL *)(((PTR )C) + RowIncrementC));
  }
}
/*****************************************************************************
**
** MultiplyByDivideAndConquer
**
** For medium to medium-large (would you like fries with that) sized
** matrices A, B, and C of size MatrixSize * MatrixSize this function
** efficiently performs the operation
**    C  = A x B (if AdditiveMode == 0)
**    C += A x B (if AdditiveMode != 0)
**
** Note MatrixSize must be divisible by 16.
**
** INPUT:
**    C = (*C READ/WRITE) Address of top left element of matrix C.
**    A = (*A IS READ ONLY) Address of top left element of matrix A.
**    B = (*B IS READ ONLY) Address of top left element of matrix B.
**    MatrixSize = Size of matrices (for n*n matrix, MatrixSize = n)
**    RowWidthA = Number of elements in memory between A[x,y] and A[x,y+1]
**    RowWidthB = Number of elements in memory between B[x,y] and B[x,y+1]
**    RowWidthC = Number of elements in memory between C[x,y] and C[x,y+1]
**    AdditiveMode = 0 if we want C = A x B, otherwise we'll do C += A x B
**
** OUTPUT:
**    C (+)= A x B. (+ if AdditiveMode != 0)
**
*****************************************************************************/

void MultiplyByDivideAndConquer(REAL *C,REAL *A,REAL *B,unsigned int MatrixSize,unsigned int RowWidthC,unsigned int RowWidthA,unsigned int RowWidthB,int AdditiveMode)
{
  #define A00 A
  #define B00 B
  #define C00 C
  REAL *A01;
  REAL *A10;
  REAL *A11;
  REAL *B01;
  REAL *B10;
  REAL *B11;
  REAL *C01;
  REAL *C10;
  REAL *C11;
  unsigned int QuadrantSize = (MatrixSize >> 1);
/* partition the matrix */
  A01 = (A + QuadrantSize);
  A10 = (A + (RowWidthA * QuadrantSize));
  A11 = (A10 + QuadrantSize);
  B01 = (B + QuadrantSize);
  B10 = (B + (RowWidthB * QuadrantSize));
  B11 = (B10 + QuadrantSize);
  C01 = (C + QuadrantSize);
  C10 = (C + (RowWidthC * QuadrantSize));
  C11 = (C10 + QuadrantSize);
  if (QuadrantSize > (16)) {
    MultiplyByDivideAndConquer(C,A,B,QuadrantSize,RowWidthC,RowWidthA,RowWidthB,AdditiveMode);
    MultiplyByDivideAndConquer(C01,A,B01,QuadrantSize,RowWidthC,RowWidthA,RowWidthB,AdditiveMode);
    MultiplyByDivideAndConquer(C11,A10,B01,QuadrantSize,RowWidthC,RowWidthA,RowWidthB,AdditiveMode);
    MultiplyByDivideAndConquer(C10,A10,B,QuadrantSize,RowWidthC,RowWidthA,RowWidthB,AdditiveMode);
    MultiplyByDivideAndConquer(C,A01,B10,QuadrantSize,RowWidthC,RowWidthA,RowWidthB,1);
    MultiplyByDivideAndConquer(C01,A01,B11,QuadrantSize,RowWidthC,RowWidthA,RowWidthB,1);
    MultiplyByDivideAndConquer(C11,A11,B11,QuadrantSize,RowWidthC,RowWidthA,RowWidthB,1);
    MultiplyByDivideAndConquer(C10,A11,B10,QuadrantSize,RowWidthC,RowWidthA,RowWidthB,1);
  }
  else {
    if (AdditiveMode) {
      FastAdditiveNaiveMatrixMultiply(C,A,B,QuadrantSize,RowWidthC,RowWidthA,RowWidthB);
      FastAdditiveNaiveMatrixMultiply(C01,A,B01,QuadrantSize,RowWidthC,RowWidthA,RowWidthB);
      FastAdditiveNaiveMatrixMultiply(C11,A10,B01,QuadrantSize,RowWidthC,RowWidthA,RowWidthB);
      FastAdditiveNaiveMatrixMultiply(C10,A10,B,QuadrantSize,RowWidthC,RowWidthA,RowWidthB);
    }
    else {
      FastNaiveMatrixMultiply(C,A,B,QuadrantSize,RowWidthC,RowWidthA,RowWidthB);
      FastNaiveMatrixMultiply(C01,A,B01,QuadrantSize,RowWidthC,RowWidthA,RowWidthB);
      FastNaiveMatrixMultiply(C11,A10,B01,QuadrantSize,RowWidthC,RowWidthA,RowWidthB);
      FastNaiveMatrixMultiply(C10,A10,B,QuadrantSize,RowWidthC,RowWidthA,RowWidthB);
    }
    FastAdditiveNaiveMatrixMultiply(C,A01,B10,QuadrantSize,RowWidthC,RowWidthA,RowWidthB);
    FastAdditiveNaiveMatrixMultiply(C01,A01,B11,QuadrantSize,RowWidthC,RowWidthA,RowWidthB);
    FastAdditiveNaiveMatrixMultiply(C11,A11,B11,QuadrantSize,RowWidthC,RowWidthA,RowWidthB);
    FastAdditiveNaiveMatrixMultiply(C10,A11,B10,QuadrantSize,RowWidthC,RowWidthA,RowWidthB);
  }
}
/*****************************************************************************
**
** OptimizedStrassenMultiply
**
** For large matrices A, B, and C of size MatrixSize * MatrixSize this
** function performs the operation C = A x B efficiently.
**
** INPUT:
**    C = (*C WRITE) Address of top left element of matrix C.
**    A = (*A IS READ ONLY) Address of top left element of matrix A.
**    B = (*B IS READ ONLY) Address of top left element of matrix B.
**    MatrixSize = Size of matrices (for n*n matrix, MatrixSize = n)
**    RowWidthA = Number of elements in memory between A[x,y] and A[x,y+1]
**    RowWidthB = Number of elements in memory between B[x,y] and B[x,y+1]
**    RowWidthC = Number of elements in memory between C[x,y] and C[x,y+1]
**
** OUTPUT:
**    C = (*C WRITE) Matrix C contains A x B. (Initial value of *C undefined.)
**
*****************************************************************************/

void OptimizedStrassenMultiply_seq(REAL *C,REAL *A,REAL *B,unsigned int MatrixSize,unsigned int RowWidthC,unsigned int RowWidthA,unsigned int RowWidthB,int Depth)
{
/* MatixSize / 2 */
  unsigned int QuadrantSize = (MatrixSize >> 1);
  unsigned int QuadrantSizeInBytes = ((((sizeof(REAL )) * (QuadrantSize)) * (QuadrantSize)) + (32));
  unsigned int Column;
  unsigned int Row;
/************************************************************************
  ** For each matrix A, B, and C, we'll want pointers to each quandrant
  ** in the matrix. These quandrants will be addressed as follows:
  **  --        --
  **  | A11  A12 |
  **  |          |
  **  | A21  A22 |
  **  --        --
  ************************************************************************/
/* *A11, *B11, *C11, */
  REAL *A12;
  REAL *B12;
  REAL *C12;
  REAL *A21;
  REAL *B21;
  REAL *C21;
  REAL *A22;
  REAL *B22;
  REAL *C22;
  REAL *S1;
  REAL *S2;
  REAL *S3;
  REAL *S4;
  REAL *S5;
  REAL *S6;
  REAL *S7;
  REAL *S8;
  REAL *M2;
  REAL *M5;
  REAL *T1sMULT;
  #define T2sMULT C22
  #define NumberOfVariables 11
  PTR TempMatrixOffset = (0);
  PTR MatrixOffsetA = (0);
  PTR MatrixOffsetB = (0);
  char *Heap;
  void *StartHeap;
/* Distance between the end of a matrix row and the start of the next row */
  PTR RowIncrementA = ((RowWidthA - QuadrantSize) << 3);
  PTR RowIncrementB = ((RowWidthB - QuadrantSize) << 3);
  PTR RowIncrementC = ((RowWidthC - QuadrantSize) << 3);
  if (MatrixSize <= (unsigned)(bots_app_cutoff_value)) {
    MultiplyByDivideAndConquer(C,A,B,MatrixSize,RowWidthC,RowWidthA,RowWidthB,0);
    return ;
  }
/* Initialize quandrant matrices */
  #define A11 A
  #define B11 B
  #define C11 C
  A12 = (A + QuadrantSize);
  B12 = (B + QuadrantSize);
  C12 = (C + QuadrantSize);
  A21 = (A + (RowWidthA * QuadrantSize));
  B21 = (B + (RowWidthB * QuadrantSize));
  C21 = (C + (RowWidthC * QuadrantSize));
  A22 = (A21 + QuadrantSize);
  B22 = (B21 + QuadrantSize);
  C22 = (C21 + QuadrantSize);
/* Allocate Heap Space Here */
  StartHeap = ((Heap = ((char *)(malloc(((QuadrantSizeInBytes * (11))))))));
/* ensure that heap is on cache boundary */
  if ((((PTR )Heap) & (31))) 
    Heap = ((char *)((((PTR )Heap) + (32)) - (((PTR )Heap) & (31))));
/* Distribute the heap space over the variables */
  S1 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S2 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S3 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S4 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S5 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S6 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S7 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S8 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  M2 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  M5 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  T1sMULT = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
/***************************************************************************
  ** Step through all columns row by row (vertically)
  ** (jumps in memory by RowWidth => bad locality)
  ** (but we want the best locality on the innermost loop)
  ***************************************************************************/
  for (Row = (0); Row < QuadrantSize; Row++) {
/*************************************************************************
    ** Step through each row horizontally (addressing elements in each column)
    ** (jumps linearly througn memory => good locality)
    *************************************************************************/
    for (Column = (0); Column < QuadrantSize; Column++) {
/***********************************************************
      ** Within this loop, the following holds for MatrixOffset:
      ** MatrixOffset = (Row * RowWidth) + Column
      ** (note: that the unit of the offset is number of reals)
      ***********************************************************/
/* Element of Global Matrix, such as A, B, C */
      #define E(Matrix)   (* (REAL*) ( ((PTR) Matrix) + TempMatrixOffset ) )
      #define EA(Matrix)  (* (REAL*) ( ((PTR) Matrix) + MatrixOffsetA ) )
      #define EB(Matrix)  (* (REAL*) ( ((PTR) Matrix) + MatrixOffsetB ) )
/* FIXME - may pay to expand these out - got higher speed-ups below */
/* S4 = A12 - ( S2 = ( S1 = A21 + A22 ) - A11 ) */
       *((REAL *)(((PTR )S4) + TempMatrixOffset)) = ( *((REAL *)(((PTR )A12) + MatrixOffsetA)) - ( *((REAL *)(((PTR )S2) + TempMatrixOffset)) = (( *((REAL *)(((PTR )S1) + TempMatrixOffset)) = ( *((REAL *)(((PTR )A21) + MatrixOffsetA)) +  *((REAL *)(((PTR )A22) + MatrixOffsetA)))) -  *((REAL *)(((PTR )A) + MatrixOffsetA)))));
/* S8 = (S6 = B22 - ( S5 = B12 - B11 ) ) - B21 */
       *((REAL *)(((PTR )S8) + TempMatrixOffset)) = (( *((REAL *)(((PTR )S6) + TempMatrixOffset)) = ( *((REAL *)(((PTR )B22) + MatrixOffsetB)) - ( *((REAL *)(((PTR )S5) + TempMatrixOffset)) = ( *((REAL *)(((PTR )B12) + MatrixOffsetB)) -  *((REAL *)(((PTR )B) + MatrixOffsetB)))))) -  *((REAL *)(((PTR )B21) + MatrixOffsetB)));
/* S3 = A11 - A21 */
       *((REAL *)(((PTR )S3) + TempMatrixOffset)) = ( *((REAL *)(((PTR )A) + MatrixOffsetA)) -  *((REAL *)(((PTR )A21) + MatrixOffsetA)));
/* S7 = B22 - B12 */
       *((REAL *)(((PTR )S7) + TempMatrixOffset)) = ( *((REAL *)(((PTR )B22) + MatrixOffsetB)) -  *((REAL *)(((PTR )B12) + MatrixOffsetB)));
      TempMatrixOffset += (sizeof(REAL ));
      MatrixOffsetA += (sizeof(REAL ));
      MatrixOffsetB += (sizeof(REAL ));
/* end row loop*/
    }
    MatrixOffsetA += RowIncrementA;
    MatrixOffsetB += RowIncrementB;
/* end column loop */
  }
/* M2 = A11 x B11 */
  OptimizedStrassenMultiply_seq(M2,A,B,QuadrantSize,QuadrantSize,RowWidthA,RowWidthB,(Depth + 1));
/* M5 = S1 * S5 */
  OptimizedStrassenMultiply_seq(M5,S1,S5,QuadrantSize,QuadrantSize,QuadrantSize,QuadrantSize,(Depth + 1));
/* Step 1 of T1 = S2 x S6 + M2 */
  OptimizedStrassenMultiply_seq(T1sMULT,S2,S6,QuadrantSize,QuadrantSize,QuadrantSize,QuadrantSize,(Depth + 1));
/* Step 1 of T2 = T1 + S3 x S7 */
/*FIXME*/
  OptimizedStrassenMultiply_seq(C22,S3,S7,QuadrantSize,RowWidthC,QuadrantSize,QuadrantSize,(Depth + 1));
/* Step 1 of C11 = M2 + A12 * B21 */
  OptimizedStrassenMultiply_seq(C,A12,B21,QuadrantSize,RowWidthC,RowWidthA,RowWidthB,(Depth + 1));
/* Step 1 of C12 = S4 x B22 + T1 + M5 */
  OptimizedStrassenMultiply_seq(C12,S4,B22,QuadrantSize,RowWidthC,QuadrantSize,RowWidthB,(Depth + 1));
/* Step 1 of C21 = T2 - A22 * S8 */
  OptimizedStrassenMultiply_seq(C21,A22,S8,QuadrantSize,RowWidthC,RowWidthA,QuadrantSize,(Depth + 1));
/***************************************************************************
  ** Step through all columns row by row (vertically)
  ** (jumps in memory by RowWidth => bad locality)
  ** (but we want the best locality on the innermost loop)
  ***************************************************************************/
  for (Row = (0); Row < QuadrantSize; Row++) {
/*************************************************************************
    ** Step through each row horizontally (addressing elements in each column)
    ** (jumps linearly througn memory => good locality)
    *************************************************************************/
    for (Column = (0); Column < QuadrantSize; Column += (4)) {
      REAL LocalM5_0 =  *M5;
      REAL LocalM5_1 = M5[1];
      REAL LocalM5_2 = M5[2];
      REAL LocalM5_3 = M5[3];
      REAL LocalM2_0 =  *M2;
      REAL LocalM2_1 = M2[1];
      REAL LocalM2_2 = M2[2];
      REAL LocalM2_3 = M2[3];
      REAL T1_0 = ( *T1sMULT + LocalM2_0);
      REAL T1_1 = (T1sMULT[1] + LocalM2_1);
      REAL T1_2 = (T1sMULT[2] + LocalM2_2);
      REAL T1_3 = (T1sMULT[3] + LocalM2_3);
      REAL T2_0 = ( *C22 + T1_0);
      REAL T2_1 = (C22[1] + T1_1);
      REAL T2_2 = (C22[2] + T1_2);
      REAL T2_3 = (C22[3] + T1_3);
       *C += LocalM2_0;
       *(C + 1) += LocalM2_1;
       *(C + 2) += LocalM2_2;
       *(C + 3) += LocalM2_3;
       *C12 += (LocalM5_0 + T1_0);
       *(C12 + 1) += (LocalM5_1 + T1_1);
       *(C12 + 2) += (LocalM5_2 + T1_2);
       *(C12 + 3) += (LocalM5_3 + T1_3);
       *C22 = (LocalM5_0 + T2_0);
       *(C22 + 1) = (LocalM5_1 + T2_1);
       *(C22 + 2) = (LocalM5_2 + T2_2);
       *(C22 + 3) = (LocalM5_3 + T2_3);
       *C21 = (-( *C21) + T2_0);
       *(C21 + 1) = (-C21[1] + T2_1);
       *(C21 + 2) = (-C21[2] + T2_2);
       *(C21 + 3) = (-C21[3] + T2_3);
      M5 += 4;
      M2 += 4;
      T1sMULT += 4;
      C += 4;
      C12 += 4;
      C21 += 4;
      C22 += 4;
    }
    C = ((REAL *)(((PTR )C) + RowIncrementC));
    C12 = ((REAL *)(((PTR )C12) + RowIncrementC));
    C21 = ((REAL *)(((PTR )C21) + RowIncrementC));
    C22 = ((REAL *)(((PTR )C22) + RowIncrementC));
  }
  free(StartHeap);
}
#if defined(IF_CUTOFF)
/* MatixSize / 2 */
/************************************************************************
  ** For each matrix A, B, and C, we'll want pointers to each quandrant
  ** in the matrix. These quandrants will be addressed as follows:
  **  --        --
  **  | A11  A12 |
  **  |          |
  **  | A21  A22 |
  **  --        --
  ************************************************************************/
/* *A11, *B11, *C11, */
  #define T2sMULT C22
  #define NumberOfVariables 11
/* Distance between the end of a matrix row and the start of the next row */
/* Initialize quandrant matrices */
  #define A11 A
  #define B11 B
  #define C11 C
/* Allocate Heap Space Here */
/* ensure that heap is on cache boundary */
/* Distribute the heap space over the variables */
/***************************************************************************
  ** Step through all columns row by row (vertically)
  ** (jumps in memory by RowWidth => bad locality)
  ** (but we want the best locality on the innermost loop)
  ***************************************************************************/
/*************************************************************************
    ** Step through each row horizontally (addressing elements in each column)
    ** (jumps linearly througn memory => good locality)
    *************************************************************************/
/***********************************************************
      ** Within this loop, the following holds for MatrixOffset:
      ** MatrixOffset = (Row * RowWidth) + Column
      ** (note: that the unit of the offset is number of reals)
      ***********************************************************/
/* Element of Global Matrix, such as A, B, C */
      #define E(Matrix)   (* (REAL*) ( ((PTR) Matrix) + TempMatrixOffset ) )
      #define EA(Matrix)  (* (REAL*) ( ((PTR) Matrix) + MatrixOffsetA ) )
      #define EB(Matrix)  (* (REAL*) ( ((PTR) Matrix) + MatrixOffsetB ) )
/* FIXME - may pay to expand these out - got higher speed-ups below */
/* S4 = A12 - ( S2 = ( S1 = A21 + A22 ) - A11 ) */
/* S8 = (S6 = B22 - ( S5 = B12 - B11 ) ) - B21 */
/* S3 = A11 - A21 */
/* S7 = B22 - B12 */
/* end row loop*/
/* end column loop */
/* M2 = A11 x B11 */
/* M5 = S1 * S5 */
/* Step 1 of T1 = S2 x S6 + M2 */
/* Step 1 of T2 = T1 + S3 x S7 */
/*FIXME*/
/* Step 1 of C11 = M2 + A12 * B21 */
/* Step 1 of C12 = S4 x B22 + T1 + M5 */
/* Step 1 of C21 = T2 - A22 * S8 */
/**********************************************
  ** Synchronization Point
  **********************************************/
/***************************************************************************
  ** Step through all columns row by row (vertically)
  ** (jumps in memory by RowWidth => bad locality)
  ** (but we want the best locality on the innermost loop)
  ***************************************************************************/
/*************************************************************************
    ** Step through each row horizontally (addressing elements in each column)
    ** (jumps linearly througn memory => good locality)
    *************************************************************************/
#elif defined(MANUAL_CUTOFF)

struct OUT__3__1527___data 
{
  unsigned int RowWidthC;
  unsigned int RowWidthA;
  int Depth;
  unsigned int QuadrantSize;
  REAL *C21;
  REAL *A22;
  REAL *S8;
}
;
static void OUT__3__1527__(void *__out_argv);

struct OUT__4__1527___data 
{
  unsigned int RowWidthC;
  unsigned int RowWidthB;
  int Depth;
  unsigned int QuadrantSize;
  REAL *C12;
  REAL *B22;
  REAL *S4;
}
;
static void OUT__4__1527__(void *__out_argv);

struct OUT__5__1527___data 
{
  REAL *C;
  unsigned int RowWidthC;
  unsigned int RowWidthA;
  unsigned int RowWidthB;
  int Depth;
  unsigned int QuadrantSize;
  REAL *A12;
  REAL *B21;
}
;
static void OUT__5__1527__(void *__out_argv);

struct OUT__6__1527___data 
{
  unsigned int RowWidthC;
  int Depth;
  unsigned int QuadrantSize;
  REAL *C22;
  REAL *S3;
  REAL *S7;
}
;
static void OUT__6__1527__(void *__out_argv);

struct OUT__7__1527___data 
{
  int Depth;
  unsigned int QuadrantSize;
  REAL *S2;
  REAL *S6;
  REAL *T1sMULT;
}
;
static void OUT__7__1527__(void *__out_argv);

struct OUT__8__1527___data 
{
  int Depth;
  unsigned int QuadrantSize;
  REAL *S1;
  REAL *S5;
  REAL *M5;
}
;
static void OUT__8__1527__(void *__out_argv);

struct OUT__9__1527___data 
{
  REAL *A;
  REAL *B;
  unsigned int RowWidthA;
  unsigned int RowWidthB;
  int Depth;
  unsigned int QuadrantSize;
  REAL *M2;
}
;
static void OUT__9__1527__(void *__out_argv);

void OptimizedStrassenMultiply_par(REAL *C,REAL *A,REAL *B,unsigned int MatrixSize,unsigned int RowWidthC,unsigned int RowWidthA,unsigned int RowWidthB,int Depth)
{
/* MatixSize / 2 */
  unsigned int QuadrantSize = (MatrixSize >> 1);
  unsigned int QuadrantSizeInBytes = ((((sizeof(REAL )) * (QuadrantSize)) * (QuadrantSize)) + (32));
  unsigned int Column;
  unsigned int Row;
/************************************************************************
  ** For each matrix A, B, and C, we'll want pointers to each quandrant
  ** in the matrix. These quandrants will be addressed as follows:
  **  --        --
  **  | A11  A12 |
  **  |          |
  **  | A21  A22 |
  **  --        --
  ************************************************************************/
/* *A11, *B11, *C11, */
  REAL *A12;
  REAL *B12;
  REAL *C12;
  REAL *A21;
  REAL *B21;
  REAL *C21;
  REAL *A22;
  REAL *B22;
  REAL *C22;
  REAL *S1;
  REAL *S2;
  REAL *S3;
  REAL *S4;
  REAL *S5;
  REAL *S6;
  REAL *S7;
  REAL *S8;
  REAL *M2;
  REAL *M5;
  REAL *T1sMULT;
  #define T2sMULT C22
  #define NumberOfVariables 11
  PTR TempMatrixOffset = (0);
  PTR MatrixOffsetA = (0);
  PTR MatrixOffsetB = (0);
  char *Heap;
  void *StartHeap;
/* Distance between the end of a matrix row and the start of the next row */
  PTR RowIncrementA = ((RowWidthA - QuadrantSize) << 3);
  PTR RowIncrementB = ((RowWidthB - QuadrantSize) << 3);
  PTR RowIncrementC = ((RowWidthC - QuadrantSize) << 3);
  if (MatrixSize <= (unsigned)(bots_app_cutoff_value)) {
    MultiplyByDivideAndConquer(C,A,B,MatrixSize,RowWidthC,RowWidthA,RowWidthB,0);
    return ;
  }
/* Initialize quandrant matrices */
  #define A11 A
  #define B11 B
  #define C11 C
  A12 = (A + QuadrantSize);
  B12 = (B + QuadrantSize);
  C12 = (C + QuadrantSize);
  A21 = (A + (RowWidthA * QuadrantSize));
  B21 = (B + (RowWidthB * QuadrantSize));
  C21 = (C + (RowWidthC * QuadrantSize));
  A22 = (A21 + QuadrantSize);
  B22 = (B21 + QuadrantSize);
  C22 = (C21 + QuadrantSize);
/* Allocate Heap Space Here */
  StartHeap = ((Heap = ((char *)(malloc(((QuadrantSizeInBytes * (11))))))));
/* ensure that heap is on cache boundary */
  if ((((PTR )Heap) & (31))) 
    Heap = ((char *)((((PTR )Heap) + (32)) - (((PTR )Heap) & (31))));
/* Distribute the heap space over the variables */
  S1 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S2 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S3 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S4 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S5 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S6 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S7 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  S8 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  M2 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  M5 = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
  T1sMULT = ((REAL *)Heap);
  Heap += QuadrantSizeInBytes;
/***************************************************************************
  ** Step through all columns row by row (vertically)
  ** (jumps in memory by RowWidth => bad locality)
  ** (but we want the best locality on the innermost loop)
  ***************************************************************************/
  for (Row = (0); Row < QuadrantSize; Row++) {
/*************************************************************************
    ** Step through each row horizontally (addressing elements in each column)
    ** (jumps linearly througn memory => good locality)
    *************************************************************************/
    for (Column = (0); Column < QuadrantSize; Column++) {
/***********************************************************
      ** Within this loop, the following holds for MatrixOffset:
      ** MatrixOffset = (Row * RowWidth) + Column
      ** (note: that the unit of the offset is number of reals)
      ***********************************************************/
/* Element of Global Matrix, such as A, B, C */
      #define E(Matrix)   (* (REAL*) ( ((PTR) Matrix) + TempMatrixOffset ) )
      #define EA(Matrix)  (* (REAL*) ( ((PTR) Matrix) + MatrixOffsetA ) )
      #define EB(Matrix)  (* (REAL*) ( ((PTR) Matrix) + MatrixOffsetB ) )
/* FIXME - may pay to expand these out - got higher speed-ups below */
/* S4 = A12 - ( S2 = ( S1 = A21 + A22 ) - A11 ) */
       *((REAL *)(((PTR )S4) + TempMatrixOffset)) = ( *((REAL *)(((PTR )A12) + MatrixOffsetA)) - ( *((REAL *)(((PTR )S2) + TempMatrixOffset)) = (( *((REAL *)(((PTR )S1) + TempMatrixOffset)) = ( *((REAL *)(((PTR )A21) + MatrixOffsetA)) +  *((REAL *)(((PTR )A22) + MatrixOffsetA)))) -  *((REAL *)(((PTR )A) + MatrixOffsetA)))));
/* S8 = (S6 = B22 - ( S5 = B12 - B11 ) ) - B21 */
       *((REAL *)(((PTR )S8) + TempMatrixOffset)) = (( *((REAL *)(((PTR )S6) + TempMatrixOffset)) = ( *((REAL *)(((PTR )B22) + MatrixOffsetB)) - ( *((REAL *)(((PTR )S5) + TempMatrixOffset)) = ( *((REAL *)(((PTR )B12) + MatrixOffsetB)) -  *((REAL *)(((PTR )B) + MatrixOffsetB)))))) -  *((REAL *)(((PTR )B21) + MatrixOffsetB)));
/* S3 = A11 - A21 */
       *((REAL *)(((PTR )S3) + TempMatrixOffset)) = ( *((REAL *)(((PTR )A) + MatrixOffsetA)) -  *((REAL *)(((PTR )A21) + MatrixOffsetA)));
/* S7 = B22 - B12 */
       *((REAL *)(((PTR )S7) + TempMatrixOffset)) = ( *((REAL *)(((PTR )B22) + MatrixOffsetB)) -  *((REAL *)(((PTR )B12) + MatrixOffsetB)));
      TempMatrixOffset += (sizeof(REAL ));
      MatrixOffsetA += (sizeof(REAL ));
      MatrixOffsetB += (sizeof(REAL ));
/* end row loop*/
    }
    MatrixOffsetA += RowIncrementA;
    MatrixOffsetB += RowIncrementB;
/* end column loop */
  }
  if (Depth < bots_cutoff_value) {
    struct OUT__9__1527___data __out_argv9__1527__;
    __out_argv9__1527__.OUT__9__1527___data::M2 = M2;
    __out_argv9__1527__.OUT__9__1527___data::QuadrantSize = QuadrantSize;
    __out_argv9__1527__.OUT__9__1527___data::Depth = Depth;
    __out_argv9__1527__.OUT__9__1527___data::RowWidthB = RowWidthB;
    __out_argv9__1527__.OUT__9__1527___data::RowWidthA = RowWidthA;
    __out_argv9__1527__.OUT__9__1527___data::B = B;
    __out_argv9__1527__.OUT__9__1527___data::A = A;
/* M2 = A11 x B11 */
    XOMP_task(OUT__9__1527__,&__out_argv9__1527__,0,sizeof(struct OUT__9__1527___data ),4,1,1);
    struct OUT__8__1527___data __out_argv8__1527__;
    __out_argv8__1527__.OUT__8__1527___data::M5 = M5;
    __out_argv8__1527__.OUT__8__1527___data::S5 = S5;
    __out_argv8__1527__.OUT__8__1527___data::S1 = S1;
    __out_argv8__1527__.OUT__8__1527___data::QuadrantSize = QuadrantSize;
    __out_argv8__1527__.OUT__8__1527___data::Depth = Depth;
/* M5 = S1 * S5 */
    XOMP_task(OUT__8__1527__,&__out_argv8__1527__,0,sizeof(struct OUT__8__1527___data ),4,1,1);
    struct OUT__7__1527___data __out_argv7__1527__;
    __out_argv7__1527__.OUT__7__1527___data::T1sMULT = T1sMULT;
    __out_argv7__1527__.OUT__7__1527___data::S6 = S6;
    __out_argv7__1527__.OUT__7__1527___data::S2 = S2;
    __out_argv7__1527__.OUT__7__1527___data::QuadrantSize = QuadrantSize;
    __out_argv7__1527__.OUT__7__1527___data::Depth = Depth;
/* Step 1 of T1 = S2 x S6 + M2 */
    XOMP_task(OUT__7__1527__,&__out_argv7__1527__,0,sizeof(struct OUT__7__1527___data ),4,1,1);
    struct OUT__6__1527___data __out_argv6__1527__;
    __out_argv6__1527__.OUT__6__1527___data::S7 = S7;
    __out_argv6__1527__.OUT__6__1527___data::S3 = S3;
    __out_argv6__1527__.OUT__6__1527___data::C22 = C22;
    __out_argv6__1527__.OUT__6__1527___data::QuadrantSize = QuadrantSize;
    __out_argv6__1527__.OUT__6__1527___data::Depth = Depth;
    __out_argv6__1527__.OUT__6__1527___data::RowWidthC = RowWidthC;
/* Step 1 of T2 = T1 + S3 x S7 */
    XOMP_task(OUT__6__1527__,&__out_argv6__1527__,0,sizeof(struct OUT__6__1527___data ),4,1,1);
    struct OUT__5__1527___data __out_argv5__1527__;
    __out_argv5__1527__.OUT__5__1527___data::B21 = B21;
    __out_argv5__1527__.OUT__5__1527___data::A12 = A12;
    __out_argv5__1527__.OUT__5__1527___data::QuadrantSize = QuadrantSize;
    __out_argv5__1527__.OUT__5__1527___data::Depth = Depth;
    __out_argv5__1527__.OUT__5__1527___data::RowWidthB = RowWidthB;
    __out_argv5__1527__.OUT__5__1527___data::RowWidthA = RowWidthA;
    __out_argv5__1527__.OUT__5__1527___data::RowWidthC = RowWidthC;
    __out_argv5__1527__.OUT__5__1527___data::C = C;
/* Step 1 of C11 = M2 + A12 * B21 */
    XOMP_task(OUT__5__1527__,&__out_argv5__1527__,0,sizeof(struct OUT__5__1527___data ),4,1,1);
    struct OUT__4__1527___data __out_argv4__1527__;
    __out_argv4__1527__.OUT__4__1527___data::S4 = S4;
    __out_argv4__1527__.OUT__4__1527___data::B22 = B22;
    __out_argv4__1527__.OUT__4__1527___data::C12 = C12;
    __out_argv4__1527__.OUT__4__1527___data::QuadrantSize = QuadrantSize;
    __out_argv4__1527__.OUT__4__1527___data::Depth = Depth;
    __out_argv4__1527__.OUT__4__1527___data::RowWidthB = RowWidthB;
    __out_argv4__1527__.OUT__4__1527___data::RowWidthC = RowWidthC;
/* Step 1 of C12 = S4 x B22 + T1 + M5 */
    XOMP_task(OUT__4__1527__,&__out_argv4__1527__,0,sizeof(struct OUT__4__1527___data ),4,1,1);
    struct OUT__3__1527___data __out_argv3__1527__;
    __out_argv3__1527__.OUT__3__1527___data::S8 = S8;
    __out_argv3__1527__.OUT__3__1527___data::A22 = A22;
    __out_argv3__1527__.OUT__3__1527___data::C21 = C21;
    __out_argv3__1527__.OUT__3__1527___data::QuadrantSize = QuadrantSize;
    __out_argv3__1527__.OUT__3__1527___data::Depth = Depth;
    __out_argv3__1527__.OUT__3__1527___data::RowWidthA = RowWidthA;
    __out_argv3__1527__.OUT__3__1527___data::RowWidthC = RowWidthC;
/* Step 1 of C21 = T2 - A22 * S8 */
    XOMP_task(OUT__3__1527__,&__out_argv3__1527__,0,sizeof(struct OUT__3__1527___data ),4,1,1);
/**********************************************
    ** Synchronization Point
    **********************************************/
    XOMP_taskwait();
  }
  else {
/* M2 = A11 x B11 */
    OptimizedStrassenMultiply_par(M2,A,B,QuadrantSize,QuadrantSize,RowWidthA,RowWidthB,(Depth + 1));
/* M5 = S1 * S5 */
    OptimizedStrassenMultiply_par(M5,S1,S5,QuadrantSize,QuadrantSize,QuadrantSize,QuadrantSize,(Depth + 1));
/* Step 1 of T1 = S2 x S6 + M2 */
    OptimizedStrassenMultiply_par(T1sMULT,S2,S6,QuadrantSize,QuadrantSize,QuadrantSize,QuadrantSize,(Depth + 1));
/* Step 1 of T2 = T1 + S3 x S7 */
/*FIXME*/
    OptimizedStrassenMultiply_par(C22,S3,S7,QuadrantSize,RowWidthC,QuadrantSize,QuadrantSize,(Depth + 1));
/* Step 1 of C11 = M2 + A12 * B21 */
    OptimizedStrassenMultiply_par(C,A12,B21,QuadrantSize,RowWidthC,RowWidthA,RowWidthB,(Depth + 1));
/* Step 1 of C12 = S4 x B22 + T1 + M5 */
    OptimizedStrassenMultiply_par(C12,S4,B22,QuadrantSize,RowWidthC,QuadrantSize,RowWidthB,(Depth + 1));
/* Step 1 of C21 = T2 - A22 * S8 */
    OptimizedStrassenMultiply_par(C21,A22,S8,QuadrantSize,RowWidthC,RowWidthA,QuadrantSize,(Depth + 1));
  }
/***************************************************************************
  ** Step through all columns row by row (vertically)
  ** (jumps in memory by RowWidth => bad locality)
  ** (but we want the best locality on the innermost loop)
  ***************************************************************************/
  for (Row = (0); Row < QuadrantSize; Row++) {
/*************************************************************************
    ** Step through each row horizontally (addressing elements in each column)
    ** (jumps linearly througn memory => good locality)
    *************************************************************************/
    for (Column = (0); Column < QuadrantSize; Column += (4)) {
      REAL LocalM5_0 =  *M5;
      REAL LocalM5_1 = M5[1];
      REAL LocalM5_2 = M5[2];
      REAL LocalM5_3 = M5[3];
      REAL LocalM2_0 =  *M2;
      REAL LocalM2_1 = M2[1];
      REAL LocalM2_2 = M2[2];
      REAL LocalM2_3 = M2[3];
      REAL T1_0 = ( *T1sMULT + LocalM2_0);
      REAL T1_1 = (T1sMULT[1] + LocalM2_1);
      REAL T1_2 = (T1sMULT[2] + LocalM2_2);
      REAL T1_3 = (T1sMULT[3] + LocalM2_3);
      REAL T2_0 = ( *C22 + T1_0);
      REAL T2_1 = (C22[1] + T1_1);
      REAL T2_2 = (C22[2] + T1_2);
      REAL T2_3 = (C22[3] + T1_3);
       *C += LocalM2_0;
       *(C + 1) += LocalM2_1;
       *(C + 2) += LocalM2_2;
       *(C + 3) += LocalM2_3;
       *C12 += (LocalM5_0 + T1_0);
       *(C12 + 1) += (LocalM5_1 + T1_1);
       *(C12 + 2) += (LocalM5_2 + T1_2);
       *(C12 + 3) += (LocalM5_3 + T1_3);
       *C22 = (LocalM5_0 + T2_0);
       *(C22 + 1) = (LocalM5_1 + T2_1);
       *(C22 + 2) = (LocalM5_2 + T2_2);
       *(C22 + 3) = (LocalM5_3 + T2_3);
       *C21 = (-( *C21) + T2_0);
       *(C21 + 1) = (-C21[1] + T2_1);
       *(C21 + 2) = (-C21[2] + T2_2);
       *(C21 + 3) = (-C21[3] + T2_3);
      M5 += 4;
      M2 += 4;
      T1sMULT += 4;
      C += 4;
      C12 += 4;
      C21 += 4;
      C22 += 4;
    }
    C = ((REAL *)(((PTR )C) + RowIncrementC));
    C12 = ((REAL *)(((PTR )C12) + RowIncrementC));
    C21 = ((REAL *)(((PTR )C21) + RowIncrementC));
    C22 = ((REAL *)(((PTR )C22) + RowIncrementC));
  }
  free(StartHeap);
}
#else
/* MatixSize / 2 */
/************************************************************************
  ** For each matrix A, B, and C, we'll want pointers to each quandrant
  ** in the matrix. These quandrants will be addressed as follows:
  **  --        --
  **  | A11  A12 |
  **  |          |
  **  | A21  A22 |
  **  --        --
  ************************************************************************/
/* *A11, *B11, *C11, */
  #define T2sMULT C22
  #define NumberOfVariables 11
/* Distance between the end of a matrix row and the start of the next row */
/* Initialize quandrant matrices */
  #define A11 A
  #define B11 B
  #define C11 C
/* Allocate Heap Space Here */
/* ensure that heap is on cache boundary */
/* Distribute the heap space over the variables */
/***************************************************************************
  ** Step through all columns row by row (vertically)
  ** (jumps in memory by RowWidth => bad locality)
  ** (but we want the best locality on the innermost loop)
  ***************************************************************************/
/*************************************************************************
    ** Step through each row horizontally (addressing elements in each column)
    ** (jumps linearly througn memory => good locality)
    *************************************************************************/
/***********************************************************
      ** Within this loop, the following holds for MatrixOffset:
      ** MatrixOffset = (Row * RowWidth) + Column
      ** (note: that the unit of the offset is number of reals)
      ***********************************************************/
/* Element of Global Matrix, such as A, B, C */
      #define E(Matrix)   (* (REAL*) ( ((PTR) Matrix) + TempMatrixOffset ) )
      #define EA(Matrix)  (* (REAL*) ( ((PTR) Matrix) + MatrixOffsetA ) )
      #define EB(Matrix)  (* (REAL*) ( ((PTR) Matrix) + MatrixOffsetB ) )
/* FIXME - may pay to expand these out - got higher speed-ups below */
/* S4 = A12 - ( S2 = ( S1 = A21 + A22 ) - A11 ) */
/* S8 = (S6 = B22 - ( S5 = B12 - B11 ) ) - B21 */
/* S3 = A11 - A21 */
/* S7 = B22 - B12 */
/* end row loop*/
/* end column loop */
/* M2 = A11 x B11 */
/* M5 = S1 * S5 */
/* Step 1 of T1 = S2 x S6 + M2 */
/* Step 1 of T2 = T1 + S3 x S7 */
/*FIXME*/
/* Step 1 of C11 = M2 + A12 * B21 */
/* Step 1 of C12 = S4 x B22 + T1 + M5 */
/* Step 1 of C21 = T2 - A22 * S8 */
/**********************************************
  ** Synchronization Point
  **********************************************/
/***************************************************************************
  ** Step through all columns row by row (vertically)
  ** (jumps in memory by RowWidth => bad locality)
  ** (but we want the best locality on the innermost loop)
  ***************************************************************************/
/*************************************************************************
    ** Step through each row horizontally (addressing elements in each column)
    ** (jumps linearly througn memory => good locality)
    *************************************************************************/
#endif
/*
 * Set an n by n matrix A to random values.  The distance between
 * rows is an
 */

void init_matrix(int n,REAL *A,int an)
{
  int i;
  int j;
  for (i = 0; i < n; ++i) 
    for (j = 0; j < n; ++j) 
      A[(i * an) + j] = (((double )(rand())) / ((double )2147483647));
}
/*
 * Compare two matrices.  Print an error message if they differ by
 * more than EPSILON.
 */

int compare_matrix(int n,REAL *A,int an,REAL *B,int bn)
{
  int i;
  int j;
  REAL c;
  for (i = 0; i < n; ++i) 
    for (j = 0; j < n; ++j) {
/* compute the relative error c */
      c = (A[(i * an) + j] - B[(i * bn) + j]);
      if (c < 0.0) 
        c = -c;
      c = (c / A[(i * an) + j]);
      if (c > 1.0E-6) {{
          if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
            fprintf(stdout,"Strassen: Wrong answer!\n");
          }
        }
        return 2;
      }
    }
  return 1;
}
/*
 * Allocate a matrix of side n (therefore n^2 elements)
 */

REAL *alloc_matrix(int n)
{
  return (REAL *)(malloc(((((n * n)) * (sizeof(REAL ))))));
}

struct OUT__1__1527___data 
{
  void *A_p;
  void *B_p;
  void *C_p;
  void *n_p;
}
;
static void OUT__1__1527__(void *__out_argv);

struct OUT__2__1527___data 
{
  void *A_p;
  void *B_p;
  void *C_p;
  void *n_p;
}
;
static void OUT__2__1527__(void *__out_argv);

void strassen_main_par(REAL *A,REAL *B,REAL *C,int n)
{
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Computing parallel Strassen algorithm (n=%d) ",n);
    }
  }
  struct OUT__2__1527___data __out_argv2__1527__;
  __out_argv2__1527__.OUT__2__1527___data::n_p = ((void *)(&n));
  __out_argv2__1527__.OUT__2__1527___data::C_p = ((void *)(&C));
  __out_argv2__1527__.OUT__2__1527___data::B_p = ((void *)(&B));
  __out_argv2__1527__.OUT__2__1527___data::A_p = ((void *)(&A));
  XOMP_parallel_start(OUT__2__1527__,&__out_argv2__1527__,1,0,"OUT__2__1527__");
  XOMP_parallel_end();
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout," completed!\n");
    }
  }
}

void strassen_main_seq(REAL *A,REAL *B,REAL *C,int n)
{
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Computing sequential Strassen algorithm (n=%d) ",n);
    }
  }
  OptimizedStrassenMultiply_seq(C,A,B,(n),(n),(n),(n),1);
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout," completed!\n");
    }
  }
}

static void OUT__1__1527__(void *__out_argv)
{
  REAL **A = (REAL **)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::A_p);
  REAL **B = (REAL **)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::B_p);
  REAL **C = (REAL **)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::C_p);
  int *n = (int *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::n_p);
  OptimizedStrassenMultiply_par( *C, *A, *B,(( *n)),(( *n)),(( *n)),(( *n)),1);
}

static void OUT__2__1527__(void *__out_argv)
{
  REAL **A = (REAL **)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::A_p);
  REAL **B = (REAL **)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::B_p);
  REAL **C = (REAL **)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::C_p);
  int *n = (int *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::n_p);
  if (XOMP_single()) {
    struct OUT__1__1527___data __out_argv1__1527__;
    __out_argv1__1527__.OUT__1__1527___data::n_p = ((void *)(&( *n)));
    __out_argv1__1527__.OUT__1__1527___data::C_p = ((void *)(&( *C)));
    __out_argv1__1527__.OUT__1__1527___data::B_p = ((void *)(&( *B)));
    __out_argv1__1527__.OUT__1__1527___data::A_p = ((void *)(&( *A)));
    XOMP_task(OUT__1__1527__,&__out_argv1__1527__,0,sizeof(struct OUT__1__1527___data ),4,1,1);
  }
  XOMP_barrier();
}

static void OUT__3__1527__(void *__out_argv)
{
  unsigned int RowWidthC = (unsigned int )(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::RowWidthC);
  unsigned int RowWidthA = (unsigned int )(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::RowWidthA);
  int Depth = (int )(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::Depth);
  unsigned int QuadrantSize = (unsigned int )(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::QuadrantSize);
  REAL *C21 = (REAL *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::C21);
  REAL *A22 = (REAL *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::A22);
  REAL *S8 = (REAL *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::S8);
  unsigned int _p_RowWidthC = RowWidthC;
  unsigned int _p_RowWidthA = RowWidthA;
  int _p_Depth = Depth;
  unsigned int _p_QuadrantSize = QuadrantSize;
  REAL *_p_C21 = C21;
  REAL *_p_A22 = A22;
  REAL *_p_S8 = S8;
  OptimizedStrassenMultiply_par(_p_C21,_p_A22,_p_S8,_p_QuadrantSize,_p_RowWidthC,_p_RowWidthA,_p_QuadrantSize,(_p_Depth + 1));
}

static void OUT__4__1527__(void *__out_argv)
{
  unsigned int RowWidthC = (unsigned int )(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::RowWidthC);
  unsigned int RowWidthB = (unsigned int )(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::RowWidthB);
  int Depth = (int )(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::Depth);
  unsigned int QuadrantSize = (unsigned int )(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::QuadrantSize);
  REAL *C12 = (REAL *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::C12);
  REAL *B22 = (REAL *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::B22);
  REAL *S4 = (REAL *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::S4);
  unsigned int _p_RowWidthC = RowWidthC;
  unsigned int _p_RowWidthB = RowWidthB;
  int _p_Depth = Depth;
  unsigned int _p_QuadrantSize = QuadrantSize;
  REAL *_p_C12 = C12;
  REAL *_p_B22 = B22;
  REAL *_p_S4 = S4;
  OptimizedStrassenMultiply_par(_p_C12,_p_S4,_p_B22,_p_QuadrantSize,_p_RowWidthC,_p_QuadrantSize,_p_RowWidthB,(_p_Depth + 1));
}

static void OUT__5__1527__(void *__out_argv)
{
  REAL *C = (REAL *)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::C);
  unsigned int RowWidthC = (unsigned int )(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::RowWidthC);
  unsigned int RowWidthA = (unsigned int )(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::RowWidthA);
  unsigned int RowWidthB = (unsigned int )(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::RowWidthB);
  int Depth = (int )(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::Depth);
  unsigned int QuadrantSize = (unsigned int )(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::QuadrantSize);
  REAL *A12 = (REAL *)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::A12);
  REAL *B21 = (REAL *)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::B21);
  REAL *_p_C = C;
  unsigned int _p_RowWidthC = RowWidthC;
  unsigned int _p_RowWidthA = RowWidthA;
  unsigned int _p_RowWidthB = RowWidthB;
  int _p_Depth = Depth;
  unsigned int _p_QuadrantSize = QuadrantSize;
  REAL *_p_A12 = A12;
  REAL *_p_B21 = B21;
  OptimizedStrassenMultiply_par(_p_C,_p_A12,_p_B21,_p_QuadrantSize,_p_RowWidthC,_p_RowWidthA,_p_RowWidthB,(_p_Depth + 1));
}

static void OUT__6__1527__(void *__out_argv)
{
  unsigned int RowWidthC = (unsigned int )(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::RowWidthC);
  int Depth = (int )(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::Depth);
  unsigned int QuadrantSize = (unsigned int )(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::QuadrantSize);
  REAL *C22 = (REAL *)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::C22);
  REAL *S3 = (REAL *)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::S3);
  REAL *S7 = (REAL *)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::S7);
  unsigned int _p_RowWidthC = RowWidthC;
  int _p_Depth = Depth;
  unsigned int _p_QuadrantSize = QuadrantSize;
  REAL *_p_C22 = C22;
  REAL *_p_S3 = S3;
  REAL *_p_S7 = S7;
  OptimizedStrassenMultiply_par(_p_C22,_p_S3,_p_S7,_p_QuadrantSize,_p_RowWidthC,_p_QuadrantSize,_p_QuadrantSize,(_p_Depth + 1));
}

static void OUT__7__1527__(void *__out_argv)
{
  int Depth = (int )(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::Depth);
  unsigned int QuadrantSize = (unsigned int )(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::QuadrantSize);
  REAL *S2 = (REAL *)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::S2);
  REAL *S6 = (REAL *)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::S6);
  REAL *T1sMULT = (REAL *)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::T1sMULT);
  int _p_Depth = Depth;
  unsigned int _p_QuadrantSize = QuadrantSize;
  REAL *_p_S2 = S2;
  REAL *_p_S6 = S6;
  REAL *_p_T1sMULT = T1sMULT;
  OptimizedStrassenMultiply_par(_p_T1sMULT,_p_S2,_p_S6,_p_QuadrantSize,_p_QuadrantSize,_p_QuadrantSize,_p_QuadrantSize,(_p_Depth + 1));
}

static void OUT__8__1527__(void *__out_argv)
{
  int Depth = (int )(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::Depth);
  unsigned int QuadrantSize = (unsigned int )(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::QuadrantSize);
  REAL *S1 = (REAL *)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::S1);
  REAL *S5 = (REAL *)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::S5);
  REAL *M5 = (REAL *)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::M5);
  int _p_Depth = Depth;
  unsigned int _p_QuadrantSize = QuadrantSize;
  REAL *_p_S1 = S1;
  REAL *_p_S5 = S5;
  REAL *_p_M5 = M5;
  OptimizedStrassenMultiply_par(_p_M5,_p_S1,_p_S5,_p_QuadrantSize,_p_QuadrantSize,_p_QuadrantSize,_p_QuadrantSize,(_p_Depth + 1));
}

static void OUT__9__1527__(void *__out_argv)
{
  REAL *A = (REAL *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::A);
  REAL *B = (REAL *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::B);
  unsigned int RowWidthA = (unsigned int )(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::RowWidthA);
  unsigned int RowWidthB = (unsigned int )(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::RowWidthB);
  int Depth = (int )(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::Depth);
  unsigned int QuadrantSize = (unsigned int )(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::QuadrantSize);
  REAL *M2 = (REAL *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::M2);
  REAL *_p_A = A;
  REAL *_p_B = B;
  unsigned int _p_RowWidthA = RowWidthA;
  unsigned int _p_RowWidthB = RowWidthB;
  int _p_Depth = Depth;
  unsigned int _p_QuadrantSize = QuadrantSize;
  REAL *_p_M2 = M2;
  OptimizedStrassenMultiply_par(_p_M2,_p_A,_p_B,_p_QuadrantSize,_p_QuadrantSize,_p_RowWidthA,_p_RowWidthB,(_p_Depth + 1));
}
