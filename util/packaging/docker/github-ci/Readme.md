`docker` or `podman` should both work:

```
docker build -f util/dockerfiles/github-ci/Dockerfile -t chapel-lang/chapel-github-ci:latest
# -w working directory
# -v volume mount
# -it interactive with tty
# --rm remove container at exit
docker run -w="$(pwd)" -v"$(pwd)":"$(pwd)":z -it --rm chapel-lang/chapel-github-ci:latest
# NOTE: this mounts `build` and any `third-party/**/{build,install}` into the container, so be aware
```
