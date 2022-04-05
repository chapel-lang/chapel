Notes

%post commands run as root
%runscript commands run as regular user

singularity build --fakeroot singularity.sif singularity.def

singularity run singularity.sif

singularity shell singularity.sif

By default environment included in run commands
