#!/usr/bin/env perl

$memoryInBytes = @ARGV[0];

$memoryTarget = $memoryInBytes / 4;
$hplMemoryTarget = $memoryInBytes / 2;

$streamProbSize = int($memoryTarget / (3*8));
$raProbSize = int($memoryTarget / (1*8));
$fftProbSize = int($memoryTarget / (2*16));
$hplProbSize = int($hplMemoryTarget / (1*8));

printf "For default settings:\n";
printf "  Stream = $streamProbSize\n";
printf "  RA     = $raProbSize\n";
printf "  FFT    = $fftProbSize\n";
printf "  HPL    = $hplProbSize\n";
printf "\n";

$streamPow2ProbSize = 0;
$raPow2ProbSize = 0;
$fftPow2ProbSize = 0;
$hplPow2ProbSize = 0;
$pow2ProbSize = 1;
while (($streamPow2ProbSize == 0) || ($raPow2ProbSize == 0) || ($fftPow2ProbSize == 0) || ($hplPow2ProbSize == 0)) {
    $pow2ProbSize *= 2;
    if (($pow2ProbSize*3*8 > $memoryTarget) && ($streamPow2ProbSize == 0)) {
        $streamPow2ProbSize = $pow2ProbSize;
    }
    if (($pow2ProbSize*1*8 > $memoryTarget) && ($raPow2ProbSize == 0)) {
        $raPow2ProbSize = $pow2ProbSize;
    }
    if (($pow2ProbSize*2*16 > $memoryTarget) && ($fftPow2ProbSize == 0)) {
        $fftPow2ProbSize = $pow2ProbSize;
    }
    if (($pow2ProbSize*1*8 > $hplMemoryTarget) && ($hplPow2ProbSize == 0)) {
        $hplPow2ProbSize = $pow2ProbSize;
    }
}

printf "For 2**k case:\n";
printf "  Stream = $streamPow2ProbSize\n";
printf "  RA     = $raPow2ProbSize\n";
printf "  FFT    = $fftPow2ProbSize\n";
printf "  HPL    = $hplPow2ProbSize\n";

