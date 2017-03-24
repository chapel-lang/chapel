#  This directory contains the Dockerfiles for Chapel images available on [Docker Hub](https://hub.docker.com/r/chapel/).
   Please see [Docker Hub](https://hub.docker.com/r/chapel/) to obtain the images.

---

# What is Chapel?

[Chapel](http://chapel.cray.com/) is a parallel programming language designed for productivity at scale. It is developed as [open source on GitHub](https://github.com/chapel-lang/chapel/).

# Docker Images

## `chapel/chapel:<version>`
Supported Chapel versions:
* [`1.14.0`, `latest` (_1.14.0/Dockerfile_)](https://github.com/chapel-lang/chapel/blob/master/util/dockerfiles/1.14.0/Dockerfile/)
* [`1.13.1` (_1.13.1/Dockerfile_)](https://github.com/chapel-lang/chapel/blob/master/util/dockerfiles/1.13.1/Dockerfile)

This is the core image for Chapel. It provides the complete Chapel compiler and runtime.  It can be used to compile and run Chapel programs inside the Docker container. On 64-bit Linux hosts, the compiled Chapel program binary can sometimes be executed outside the container (your mileage may vary). Other Chapel-based Docker images can be created from this image.

## [`chapel/chapel-gasnet:<version>`](https://hub.docker.com/r/chapel/chapel-gasnet/)

* [`1.14.0`, `latest` (_1.14.0/gasnet/Dockerfile_)](https://github.com/chapel-lang/chapel/blob/master/util/dockerfiles/1.14.0/gasnet/Dockerfile/)

The Chapel core image (above), rebuilt with `CHPL_COMM=gasnet` and `GASNET_SPAWNFN=L`. Simulates a multilocale Chapel platform within the Docker container.

Multilocale Chapel brings additional requirements, unrelated to Docker. The `chpl` compilation produces two binary files (e.g. `hello_real` as well as `hello`). When you run the binary, you need another command line parameter, `-nl #`, to specify the number of locales. Please see [Multilocale Chapel Execution](http://chapel.cray.com/docs/latest/usingchapel/multilocale.html) for details.

# How to use the image

## Compile your Chapel program

The image can be used to compile and run your Chapel program inside the container.

In the following example, you start a shell in the container and use the interactive session to compile and run your Chapel program, all within the container. The `-v` argument mounts the current directory in the container, `-w` sets the working directory to this mounted directory, `-i` gives you STDIN, and `-t` allocates a psuedo-TTY, giving you STDOUT and STDERR. 

The Chapel compiler, `chpl`, is invoked much like `gcc`, with argument `-o` to specify the name of the output binary. Then the binary file, `hello`, is run in the usual way.

```
$ echo 'writeln("Hello, world!");' > hello.chpl

$ docker run --rm -it -v "$PWD":/usr/src/myapp -w /usr/src/myapp chapel/chapel /bin/bash
root@xxxxxxxxx:/usr/src/myapp# chpl -o hello hello.chpl
root@xxxxxxxxx:/usr/src/myapp# ./hello
Hello, world!
```
&nbsp;

Alternatively, you could compile your program in one instance of the container, and run it in another, for the same end result. 
```
$ docker run --rm -v "$PWD":/usr/src/myapp -w /usr/src/myapp chapel/chapel chpl -o hello hello.chpl

$ docker run --rm -v "$PWD":/usr/src/myapp -w /usr/src/myapp chapel/chapel ./hello
Hello, world!
```
&nbsp;

On 64-bit Linux systems, the resultant binary can sometimes be run outside the container, if it was compiled with the `--static` option:
```
$ docker run --rm -v "$PWD":/usr/src/myapp -w /usr/src/myapp chapel/chapel chpl --static -o hello hello.chpl

$ ./hello
Hello, world!
```
&nbsp;

# Documentation

Chapel's documentation is [available online](http://chapel.cray.com/docs/latest/). Documentation for a specific release is also available: [1.14](http://chapel.cray.com/docs/1.14/), [1.13](http://chapel.cray.com/docs/1.13/).

# License

View [license information](http://chapel.cray.com/license.html) for the software contained in this image.

# User Feedback

If you have any questions about or problems with this image, please write to the [Chapel developers mailing list](https://lists.sourceforge.net/lists/listinfo/chapel-developers). To post a message to all the list members, send email to [chapel-developers@lists.sourceforge.net](mailto:chapel-developers@lists.sourceforge.net). You may also be interested in other [Chapel resources for developers](http://chapel.cray.com/developers.html), including the `#chapel-developers` IRC channel on Freenode.
