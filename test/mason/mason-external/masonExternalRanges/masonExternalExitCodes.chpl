use MasonTestHelpers;

checkExitStatus(['mason','external'],1);
checkExitStatus(['mason','external','-h'],0);
checkExitStatus(['mason','external','-V'],0);
checkExitStatus(['mason','external','--spec'],0);
