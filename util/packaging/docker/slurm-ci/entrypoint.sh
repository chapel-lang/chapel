#!/usr/bin/env bash
set -euo pipefail

install -d -o munge -g munge -m 0755 /run/munge
install -d -o munge -g munge -m 0700 /etc/munge /var/lib/munge /var/log/munge
install -o munge -g munge -m 0400 /run/slurm-ci/munge.key /etc/munge/munge.key
runuser -u munge -- /usr/sbin/munged

case "${1:-}" in
  slurmctld)
    install -d -o slurm -g slurm -m 0755 /var/spool/slurmctld
    exec /usr/sbin/slurmctld -D
    ;;
  slurmd)
    install -d -m 0755 /var/spool/slurmd
    exec /usr/sbin/slurmd -D
    ;;
  *)
    echo "Expected slurmctld or slurmd" >&2
    exit 2
    ;;
esac
