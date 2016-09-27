## _This directory is intended to document the images available on [Docker Hub](https://hub.docker.com/r/chapel/chapel/)._
   Please see [Docker Hub](https://hub.docker.com/r/chapel/chapel/) for the latest images.

---

# Supported tags and respective `Dockerfile` links

* [`1.13.1`, `latest` (_1.13.1/Dockerfile_)](https://github.com/chapel-lang/chapel/blob/96ea5aac571391ba5f210cbbd2cbfc7b3ab05b5f/util/dockerfiles/1.13.1/Dockerfile)

# What is Chapel?

[Chapel](http://chapel.cray.com/) is a parallel programming language designed for productivity at scale. It is developed as [open source on GitHub](https://github.com/chapel-lang/chapel/).

# How to use this image

## Compile your Chapel program

The `chapel/chapel:<version>` image can be used to compile your Chapel program inside the container and export a binary that can be executed outside the container.

In the following example, the `docker run` argument `-v` mounts the current directory in the container and `-w` sets the working directory to this mounted directory. The Chapel compiler, `chpl`, is invoked much like `gcc` with arguments `--static` to create a statically-linked executable and `-o` to specify the name of the output binary. The resultant binary can be run outside the container.

```
$ echo 'writeln("Hello, world!");' > hello.chpl
$ docker run --rm -v "$PWD":/usr/src/myapp -w /usr/src/myapp chapel/chapel chpl --static -o hello hello.chpl
$ ./hello
Hello, world!
```

# Image Variants

Currently, the `chapel` images come in only one flavor, although others (e.g., `onbuild`) are planned.

## `chapel/chapel:<version>`

This is the core image for Chapel. It provides the complete Chapel compiler and runtime.  It can be used to compile Chapel programs inside the container -- which may then be run inside the container or (as in the example above) exported via volumes and run outside the container (typically, if compiled with static linking) -- or as an image from which a Chapel-based image could be built.

# Documentation

Chapel's documentation is [available online](http://chapel.cray.com/docs/latest/). Documentation for a specific release is also available: [1.13](http://chapel.cray.com/docs/1.13/).

# License

View [license information](http://chapel.cray.com/license.html) for the software contained in this image.
