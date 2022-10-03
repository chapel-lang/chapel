# Chapel Docker Image

## What is Chapel?

[Chapel](https://chapel-lang.org/) is a parallel programming language designed
for productivity at scale. It is developed as
[open source on GitHub](https://github.com/chapel-lang/chapel/).


## Quickstart
Chapel hello world in Docker:
```
$ echo 'writeln("Hello, world!");' > hello.chpl

$ docker run --rm -it -v "$PWD":/myapp -w /myapp chapel/chapel /bin/bash
root@xxxxxxxxx:/myapp# chpl -o hello hello.chpl
root@xxxxxxxxx:/myapp# ./hello
Hello, world!
```

Compile and run a Chapel program using the Docker container as a
one-off, so you can develop outside of the container:
```
$ echo 'writeln("Hello, world!");' > hello.chpl
$ docker run --rm -v "$PWD":/myapp -w /myapp chapel/chapel chpl -o hello hello.chpl
$ docker run --rm -v "$PWD":/myapp -w /myapp chapel/chapel ./hello
Hello, world!
```


## Docker Image Tags

### [`chapel/chapel:<version>`](https://hub.docker.com/r/chapel/chapel)
Supported Chapel versions:
* [`1.28.0`, `latest`](https://github.com/chapel-lang/chapel/releases/tag/1.28.0)
* [`1.27.0`](https://github.com/chapel-lang/chapel/releases/tag/1.27.0)
* [`1.26.0`](https://github.com/chapel-lang/chapel/releases/tag/1.26.0)
* [`1.25.1`](https://github.com/chapel-lang/chapel/releases/tag/1.25.1)
* [`1.25.0`](https://github.com/chapel-lang/chapel/releases/tag/1.25.0)


This is the core image for Chapel. It provides the complete Chapel compiler and
runtime.  It can be used to compile and run Chapel programs inside the Docker
container. On 64-bit Linux hosts, the compiled Chapel program binary can
sometimes be executed outside the container (your mileage may vary).
Other Chapel-based Docker images can be created from this image.

### [`chapel/chapel-gasnet:<version>`](https://hub.docker.com/r/chapel/chapel-gasnet/)
* `1.27.0`, `latest`
* `1.26.0`
* `1.25.1`
* `1.25.0`

The Chapel core image (above), rebuilt with `CHPL_COMM=gasnet` and
`GASNET_SPAWNFN=L`. Simulates a multilocale Chapel platform within the Docker
container. Multiple locales in Chapel programs are used to take advantage of
distributed memory parallelism between nodes in a cluster. Since programs
compiled with Chapel using the above would live within a single Docker
container, they would not actually experience parallel computation, but the
simulation of it could help with local debugging.

Multilocale Chapel brings additional requirements, unrelated to Docker. The
`chpl` compilation produces two binary files (e.g. `hello_real` as well as
`hello`). When you run the binary, you need another command line parameter,
`-nl #`, to specify the number of locales. Please see
[Multilocale Chapel Execution](https://chapel-lang.org/docs/usingchapel/multilocale.html)
for details.


## How to use the image

### Compile your Chapel program

The image can be used to compile and run your Chapel program inside the
container.

In the following example, you start a shell in the container and use the
interactive session to compile and run your Chapel program, all within the
container. The `-v` argument mounts the current directory in the container,
`-w` sets the working directory to this mounted directory, `-i` gives you STDIN,
and `-t` allocates a psuedo-TTY, giving you STDOUT and STDERR.

The Chapel compiler, `chpl`, is invoked much like `gcc`, with argument `-o` to
specify the name of the output binary. Then the binary file, `hello`, is run in
the usual way. The following is the hello world example from above.

```
$ echo 'writeln("Hello, world!");' > hello.chpl

$ docker run --rm -it -v "$PWD":/myapp -w /myapp chapel/chapel /bin/bash
root@xxxxxxxxx:/myapp# chpl -o hello hello.chpl
root@xxxxxxxxx:/myapp# ./hello
Hello, world!
```
&nbsp;

Alternatively, you could compile your program in one instance of the container,
and run it in another, for the same end result. This is the one-off example
above.
```
$ docker run --rm -v "$PWD":/myapp -w /myapp chapel/chapel chpl -o hello hello.chpl

$ docker run --rm -v "$PWD":/myapp -w /myapp chapel/chapel ./hello
Hello, world!
```
&nbsp;

On 64-bit Linux systems, the resultant binary can sometimes be run outside the
container, if it was compiled with the `--static` option:
```
$ docker run --rm -v "$PWD":/myapp -w /myapp chapel/chapel chpl --static -o hello hello.chpl

$ ./hello
Hello, world!
```
&nbsp;

### Develop Chapel itself with Docker

It is possible to do development work on Chapel itself with Docker; the below
workflow example performs editing outside of Docker, followed by compilation and
execution within Docker.

```
$ cd [Chapel source tree root]                  # acquire Chapel sources
$ vim [Chapel source files]                     # hack on Chapel code
$ docker build -t chapel-dev .                  # build a new image locally from the modified code
$ docker run --rm -it chapel-dev make test      # test your modified code as desired
$ [...]                                         # repeat edit-compile-run cycle as desired...
```

If you have a fast computer you can speed up the building step by modifying the
Dockerfile to use concurrent jobs when building (i.e., `make -jN` for N cores).


## Documentation

Chapel's documentation is [available online](https://chapel-lang.org/docs/).
Documentation for a specific release is also available: [1.27](https://chapel-lang.org/docs/1.27/).


## License

View [license information](https://chapel-lang.org/license.html) for the
software contained in this image.


## For More Information

Please visit Chapel's [Home Page](https://chapel-lang.org/)
