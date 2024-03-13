class Chapel < Formula
  desc "Programming language for productive parallel computing at scale"
  homepage "https://chapel-lang.org/"
  url "https://github.com/chapel-lang/chapel/releases/download/1.33.0/chapel-1.33.0.tar.gz"
  sha256 "9dfd9bbab3eb1acf10242db909ccf17c1b07634452ca6ba8b238e69788d82883"
  license "Apache-2.0"
  head "https://github.com/chapel-lang/chapel.git", branch: "main"

  bottle do
    sha256 arm64_sonoma:   "bf48565f37d29f78919cdc53c05e23e5e610e915e82ede08573b06d4a9746d40"
    sha256 arm64_ventura:  "ac622257bbef56945f8241d87a63982dae2311fa803f3233788c4a2d85af6b0b"
    sha256 arm64_monterey: "03757fe09bfdbc1651aaa6b6ffbc54b14022ff13e19d62ca3abfe778de11371a"
    sha256 sonoma:         "1a16b40a4a13bca828f12a497c521cbbd6d0add7c819a47583c4273769e921b8"
    sha256 ventura:        "ad445bd4da02eca77f983c75d098716251df6af33e992627800bc4167b8b5947"
    sha256 monterey:       "984f5634eb6875a9c4b311044305d6fea01a4c04e768d2e3e20dc617e7d91d27"
    sha256 x86_64_linux:   "2e9f4c854f3bf0b2aa571c489e62ab7f398b1dfcb7ce87c12dc48930d94e6fac"
  end

  depends_on "cmake"
  depends_on "gmp"
  depends_on "llvm@17"
  depends_on "python@3.11"

  # LLVM is built with gcc11 and we will fail on linux with gcc version 5.xx
  fails_with gcc: "5"

  def llvm
    deps.map(&:to_formula).find { |f| f.name.match? "^llvm" }
  end

  def install
    # Always detect Python used as dependency rather than needing aliased Python formula
    python = "python3.11"
    # It should be noted that this will expand to: 'for cmd in python3.11 python3 python python2; do'
    # in our find-python.sh script.
    inreplace "util/config/find-python.sh", /^(for cmd in )(python3 )/, "\\1#{python} \\2"

    libexec.install Dir["*"]
    # Chapel uses this ENV to work out where to install.
    ENV["CHPL_HOME"] = libexec
    ENV["CHPL_GMP"] = "system"
    # don't try to set CHPL_LLVM_GCC_PREFIX since the llvm
    # package should be configured to use a reasonable GCC
    (libexec/"chplconfig").write <<~EOS
      CHPL_RE2=bundled
      CHPL_GMP=system
      CHPL_MEM=cstdlib
      CHPL_TASKS=fifo
      CHPL_LLVM_CONFIG=#{llvm.opt_bin}/llvm-config
      CHPL_LLVM_GCC_PREFIX=none
    EOS
    # CHPL_MEM and CHPL_TASKS are currently being set this way due
    # to this PR: https://github.com/chapel-lang/chapel/pull/23415.
    # This is a workaround for an issue where the bundled jemalloc
    # makefile is being run and failing when we are supposed to use
    # the system jemalloc.

    # Must be built from within CHPL_HOME to prevent build bugs.
    # https://github.com/Homebrew/legacy-homebrew/pull/35166
    cd libexec do
      system "./util/printchplenv", "--all"
      with_env(CHPL_PIP_FROM_SOURCE: "1") do
        system "make", "test-venv"
      end
      with_env(CHPL_LLVM: "none") do
        system "make"
      end
      with_env(CHPL_LLVM: "system") do
        system "make"
      end
      with_env(CHPL_PIP_FROM_SOURCE: "1") do
        system "make", "chpldoc"
      end
      system "make", "mason"
      system "make", "cleanall"

      rm_rf("third-party/llvm/llvm-src/")
      rm_rf("third-party/gasnet/gasnet-src")
      rm_rf("third-party/libfabric/libfabric-src")
      rm_rf("third-party/fltk/fltk-1.3.5-source.tar.gz")
      rm_rf("third-party/libunwind/libunwind-1.1.tar.gz")
      rm_rf("third-party/gmp/gmp-src/")
      rm_rf("third-party/qthread/qthread-src/installed")
    end

    # Install chpl and other binaries (e.g. chpldoc) into bin/ as exec scripts.
    platform = if OS.linux? && Hardware::CPU.is_64_bit?
      "linux64-#{Hardware::CPU.arch}"
    else
      "#{OS.kernel_name.downcase}-#{Hardware::CPU.arch}"
    end

    bin.install libexec.glob("bin/#{platform}/*")
    bin.env_script_all_files libexec/"bin"/platform, CHPL_HOME: libexec
    man1.install_symlink libexec.glob("man/man1/*.1")
  end

  test do
    ENV["CHPL_HOME"] = libexec
    ENV["CHPL_INCLUDE_PATH"] = HOMEBREW_PREFIX/"include"
    ENV["CHPL_LIB_PATH"] = HOMEBREW_PREFIX/"lib"
    cd libexec do
      with_env(CHPL_LLVM: "system") do
        system "util/test/checkChplInstall"
      end
      with_env(CHPL_LLVM: "none") do
        system "util/test/checkChplInstall"
      end
    end
    system bin/"chpl", "--print-passes", "--print-commands", libexec/"examples/hello.chpl"
  end
end
