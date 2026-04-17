{
  description = "Chapel Dependencies in Nix";

  nixConfig = {
    extra-experimental-features = "nix-command flakes";
  };

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    #nix-filter.url = "github:numtide/nix-filter";
    #flake-compat = {
    #  url = "github:edolstra/flake-compat";
    #  flake = false;
    #};
  };

  outputs = inputs: inputs.flake-utils.lib.eachDefaultSystem (system:
    with builtins;
    let
      pkgs = import inputs.nixpkgs { inherit system; };
    in
    {
      devShells.default = (pkgs.mkShell.override { stdenv = pkgs.llvmPackages_14.stdenv; }) {
        nativeBuildInputs = with pkgs; [
          automake
          bash
          cmake
          file
          gmp
          gnum4
          gnumake
          gnused
          libunwind
          llvmPackages_14.clang
          llvmPackages_14.libclang.dev
          llvmPackages_14.llvm
          llvmPackages_14.llvm.dev
          makeWrapper
          perl
          pkg-config
          python39
          which
        ];
        shellHook = with pkgs; ''
          export CHPL_LLVM=system
          export CHPL_LLVM_CONFIG=${llvmPackages_14.llvm.dev}/bin/llvm-config
          export CHPL_HOST_COMPILER=llvm
          export CHPL_HOST_CC=${llvmPackages_14.clang}/bin/clang
          export CHPL_HOST_CXX=${llvmPackages_14.clang}/bin/clang++
          export CHPL_TARGET_CC=${llvmPackages_14.clang}/bin/clang
          export CHPL_TARGET_CXX=${llvmPackages_14.clang}/bin/clang++
          export CHPL_CLANG_INCLUDES="-I ${llvmPackages_15.bintools.libc.dev}/include -I ${llvmPackages_15.clang-unwrapped.lib}/lib/clang/${llvmPackages_15.clang.version}/include"
        '';
      };
    });
}
