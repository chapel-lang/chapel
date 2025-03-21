# Github CI Docker Container

This directory contains the `Dockerfile` for the Chapel GitHub CI container.
Running the container will replicate the environment used by the CI. After
launching the container, you can run parts of the CI manually. See
`$CHPL_HOME/.github/workflows/CI.yml` for the steps the CI runs.

## Running the container

`docker` or `podman` should both work:

These commands should be run from this directory (the one containing the
`Dockerfile`).

```bash
docker build . -t chapel-lang/chapel-github-ci:latest
# -w working directory
# -v volume mount
# -it interactive with tty
# --rm remove container at exit
docker run -w="$CHPL_HOME" -v"$CHPL_HOME":"$CHPL_HOME":z -it --rm chapel-lang/chapel-github-ci:latest
# NOTE: this mounts `build` and any `third-party/**/{build,install}` into the container, so be aware
```
