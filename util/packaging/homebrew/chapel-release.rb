class Chapel < Formula
  desc "Programming language for productive parallel computing at scale"
  homepage "https://chapel-lang.org/"
  url "https://github.com/chapel-lang/chapel/releases/download/2.1.0/chapel-2.1.0.tar.gz"
  sha256 "72593c037505dd76e8b5989358b7580a3fdb213051a406adb26a487d26c68c60"
  license "Apache-2.0"
  head "https://github.com/chapel-lang/chapel.git", branch: "main"

  bottle do
    sha256 arm64_sonoma:   "6270062d9c3472bc58f50509aa6277f94bbc74a8f0281ec63086a4d9d4730a1c"
    sha256 arm64_ventura:  "e18e72fcd4fb23504c73b837b3e8f8024c461823b368b65a6948722113f85b9d"
    sha256 arm64_monterey: "190788f344f7a5233f0916ff82e058b138a83bbc12ddcd2aa209b1bb7201d0e2"
    sha256 sonoma:         "0cfba2be4769bedc8f9860a41fadeb7f96230498c64d4412b59a2aa074d6fa1b"
    sha256 ventura:        "e23058da8ea803f51ca1279d8b80adf8f32dffa698740678341a95fcdc5fe30e"
    sha256 monterey:       "f7eddcf9f7a47b6e03c977e799aaf61f5f2bbfef76c8553b1eff4e6853928af9"
    sha256 x86_64_linux:   "1b16e11553762c1dcc82c59dc56e48f0a5ed6f644f0b3a1fdaab5570d5edffc4"
  end

  depends_on "cmake"
  depends_on "gmp"
  depends_on "hwloc"
  depends_on "jemalloc"
  depends_on "llvm"
  depends_on "pkg-config"
  depends_on "python@3.12"

  # LLVM is built with gcc11 and we will fail on linux with gcc version 5.xx
  fails_with gcc: "5"

  def llvm
    deps.map(&:to_formula).find { |f| f.name.match? "^llvm" }
  end

  # This fixes an issue when using jemalloc and hwloc from the system (homebrew)
  # provided installation. Remove in Chapel 2.2 release, after
  # https://github.com/chapel-lang/chapel/pull/25354 is merged
  patch :DATA

  def install
    # Always detect Python used as dependency rather than needing aliased Python formula
    python = "python3.12"
    # It should be noted that this will expand to: 'for cmd in python3.12 python3 python python2; do'
    # in our find-python.sh script.
    inreplace "util/config/find-python.sh", /^(for cmd in )(python3 )/, "\\1#{python} \\2"

    libexec.install Dir["*"]
    # Chapel uses this ENV to work out where to install.
    ENV["CHPL_HOME"] = libexec
    ENV["CHPL_GMP"] = "system"
    # This ENV avoids a problem where cmake cache is invalidated by subsequent make calls
    ENV["CHPL_CMAKE_USE_CC_CXX"] = "1"

    # don't try to set CHPL_LLVM_GCC_PREFIX since the llvm
    # package should be configured to use a reasonable GCC
    (libexec/"chplconfig").write <<~EOS
      CHPL_RE2=bundled
      CHPL_GMP=system
      CHPL_MEM=jemalloc
      CHPL_TARGET_JEMALLOC=system
      CHPL_HWLOC=system
      CHPL_LLVM_CONFIG=#{llvm.opt_bin}/llvm-config
      CHPL_LLVM_GCC_PREFIX=none
    EOS

    # Must be built from within CHPL_HOME to prevent build bugs.
    # https://github.com/Homebrew/legacy-homebrew/pull/35166
    cd libexec do
      system "./util/printchplenv", "--all"
      with_env(CHPL_LLVM: "none") do
        system "make"
      end
      with_env(CHPL_LLVM: "system") do
        system "make"
      end
      with_env(CHPL_PIP_FROM_SOURCE: "1") do
        system "make", "chpldoc"
        system "make", "chplcheck"
        system "make", "chpl-language-server"
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
        system "util/test/checkChplDoc"
      end
      with_env(CHPL_LLVM: "none") do
        system "util/test/checkChplInstall"
        system "util/test/checkChplDoc"
      end
    end
    system bin/"chpl", "--print-passes", "--print-commands", libexec/"examples/hello.chpl"
    system bin/"chpldoc", "--version"
    system bin/"mason", "--version"

    # Test chplcheck, if it works CLS probably does too.
    # chpl-language-server will hang indefinitely waiting for a LSP client
    system bin/"chplcheck", "--list-rules"
    system bin/"chplcheck", libexec/"examples/hello.chpl"
  end
end

__END__
diff --git a/third-party/jemalloc/Makefile.target.include b/third-party/jemalloc/Makefile.target.include
deleted file mode 100644
index 217a500dfb..0000000000
--- a/third-party/jemalloc/Makefile.target.include
+++ /dev/null
@@ -1,12 +0,0 @@
-JEMALLOC_DIR=$(THIRD_PARTY_DIR)/jemalloc
-JEMALLOC_SUBDIR = $(JEMALLOC_DIR)/jemalloc-src
-JEMALLOC_BUILD_SUBDIR=build/$(CHPL_MAKE_TARGET_JEMALLOC_UNIQ_CFG_PATH)
-JEMALLOC_BUILD_DIR=$(JEMALLOC_DIR)/$(JEMALLOC_BUILD_SUBDIR)
-JEMALLOC_INSTALL_SUBDIR=install/$(CHPL_MAKE_TARGET_JEMALLOC_UNIQ_CFG_PATH)
-JEMALLOC_INSTALL_DIR=$(JEMALLOC_DIR)/$(JEMALLOC_INSTALL_SUBDIR)
-JEMALLOC_INCLUDE_DIR = $(JEMALLOC_INSTALL_DIR)/include
-JEMALLOC_LIB_DIR = $(JEMALLOC_INSTALL_DIR)/lib
-JEMALLOC_BIN_DIR = $(JEMALLOC_INSTALL_DIR)/bin
-JEMALLOC_TARGET = --target
-
-CHPL_JEMALLOC_PREFIX=$(CHPL_JEMALLOC_TARGET_PREFIX)
diff --git a/make/Makefile.base b/make/Makefile.base
index bcbda0a9cf..1120057d47 100644
--- a/make/Makefile.base
+++ b/make/Makefile.base
@@ -194,7 +194,7 @@ include $(THIRD_PARTY_DIR)/jemalloc/Makefile.common.include
 ifeq ($(strip $(CHPL_MAKE_HOST_TARGET)),--host)
 include $(THIRD_PARTY_DIR)/jemalloc/Makefile.host.include-$(CHPL_MAKE_HOST_JEMALLOC)
 else
-include $(THIRD_PARTY_DIR)/jemalloc/Makefile.target.include
+include $(THIRD_PARTY_DIR)/jemalloc/Makefile.target.include-$(CHPL_MAKE_TARGET_JEMALLOC)
 endif
 include $(THIRD_PARTY_DIR)/gmp/Makefile.include
 include $(THIRD_PARTY_DIR)/hwloc/Makefile.include
diff --git a/third-party/jemalloc/Makefile.target.include-bundled b/third-party/jemalloc/Makefile.target.include-bundled
new file mode 100644
index 0000000000..217a500dfb
--- /dev/null
+++ b/third-party/jemalloc/Makefile.target.include-bundled
@@ -0,0 +1,12 @@
+JEMALLOC_DIR=$(THIRD_PARTY_DIR)/jemalloc
+JEMALLOC_SUBDIR = $(JEMALLOC_DIR)/jemalloc-src
+JEMALLOC_BUILD_SUBDIR=build/$(CHPL_MAKE_TARGET_JEMALLOC_UNIQ_CFG_PATH)
+JEMALLOC_BUILD_DIR=$(JEMALLOC_DIR)/$(JEMALLOC_BUILD_SUBDIR)
+JEMALLOC_INSTALL_SUBDIR=install/$(CHPL_MAKE_TARGET_JEMALLOC_UNIQ_CFG_PATH)
+JEMALLOC_INSTALL_DIR=$(JEMALLOC_DIR)/$(JEMALLOC_INSTALL_SUBDIR)
+JEMALLOC_INCLUDE_DIR = $(JEMALLOC_INSTALL_DIR)/include
+JEMALLOC_LIB_DIR = $(JEMALLOC_INSTALL_DIR)/lib
+JEMALLOC_BIN_DIR = $(JEMALLOC_INSTALL_DIR)/bin
+JEMALLOC_TARGET = --target
+
+CHPL_JEMALLOC_PREFIX=$(CHPL_JEMALLOC_TARGET_PREFIX)
diff --git a/third-party/jemalloc/Makefile.target.include-none b/third-party/jemalloc/Makefile.target.include-none
new file mode 100644
index 0000000000..e69de29bb2
--- /dev/null
+++ b/third-party/jemalloc/Makefile.target.include-none
@@ -0,0 +1 @@
+
diff --git a/third-party/jemalloc/Makefile.target.include-system b/third-party/jemalloc/Makefile.target.include-system
new file mode 100644
index 0000000000..e69de29bb2
--- /dev/null
+++ b/third-party/jemalloc/Makefile.target.include-system
@@ -0,0 +1 @@
+
diff --git a/util/chplenv/chpl_hwloc.py b/util/chplenv/chpl_hwloc.py
index cda5ed6bc0..9dc7f0355d 100755
--- a/util/chplenv/chpl_hwloc.py
+++ b/util/chplenv/chpl_hwloc.py
@@ -61,7 +61,13 @@ def get_link_args():
             if exists and retcode != 0:
                 error("CHPL_HWLOC=system requires hwloc >= 2.1", ValueError)

-            return third_party_utils.pkgconfig_get_system_link_args('hwloc')
+            _, pclibs = third_party_utils.pkgconfig_get_system_link_args('hwloc', static=False)
+            libs = []
+            for pcl in pclibs:
+                libs.append(pcl)
+                if pcl.startswith('-L'):
+                    libs.append(pcl.replace('-L', '-Wl,-rpath,', 1))
+            return ([ ], libs)
         else:
             third_party_utils.could_not_find_pkgconfig_pkg("hwloc", "CHPL_HWLOC")

diff --git a/util/chplenv/chpl_jemalloc.py b/util/chplenv/chpl_jemalloc.py
index 3d665fa56b..78761c1c6e 100644
--- a/util/chplenv/chpl_jemalloc.py
+++ b/util/chplenv/chpl_jemalloc.py
@@ -129,7 +129,13 @@ def get_link_args(flag):
         # try pkg-config
         args = third_party_utils.pkgconfig_get_system_link_args('jemalloc')
         if args != (None, None):
-            return args
+            pclibs = args[1]
+            libs = []
+            for pcl in pclibs:
+                libs.append(pcl)
+                if pcl.startswith('-L'):
+                    libs.append(pcl.replace('-L', '-Wl,-rpath,', 1))
+            return (args[0], libs)
         else:
             envname = "CHPL_TARGET_JEMALLOC" if flag == "target" else "CHPL_HOST_JEMALLOC"
             third_party_utils.could_not_find_pkgconfig_pkg("jemalloc", envname)