ARG CHAPEL_CI_IMAGE=ghcr.io/chapel-lang/chapel-github-ci:latest
FROM ${CHAPEL_CI_IMAGE}

RUN printf '%s\n' \
      'Acquire::Retries "5";' \
      'Acquire::http::Timeout "30";' \
      'Acquire::https::Timeout "30";' \
      'Acquire::ForceIPv4 "true";' \
      > /etc/apt/apt.conf.d/99-ci-network \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
      libpmi2-0-dev munge slurm-client slurmctld slurmd \
    && rm -rf /var/lib/apt/lists/*

COPY slurm.conf /etc/slurm/slurm.conf
COPY entrypoint.sh /usr/local/bin/slurm-ci-entrypoint
# These containers do not run systemd, so Slurm cannot create cgroup scopes
RUN printf '%s\n' 'CgroupPlugin=disabled' > /etc/slurm/cgroup.conf \
    && chmod 0755 /usr/local/bin/slurm-ci-entrypoint

ENTRYPOINT ["/usr/local/bin/slurm-ci-entrypoint"]
