# Arkouda — Container Build Framework

Container build framework for running [Arkouda](https://github.com/Bears-R-Us/arkouda)
on [Chapel](https://chapel-lang.org/) with HPE Cray EX (Slingshot/CXI) support,
built as two composable images:

1. **`containers/Containerfile.hpe-cray-ex-chapel-pic`** — a Chapel base image
   with dual runtimes (`hpe-cray-ex`/OFI+CXI and `linux64`/`CHPL_COMM=none`).
2. **`containers/Containerfile.arkouda`** — builds Arkouda on top of
   that Chapel base image.

See **[docs/README.md](docs/README.md)** for the full build and usage guide.

## Quick start

```bash
# 1. Build the Chapel base image
./scripts/build-chapel-dist-cxi-2.3.1-pic.sh

# 2. Build Arkouda on top of it
./scripts/build-arkouda.sh

# 3. Run it on a single workstation (standalone CHPL_COMM=none server)
docker run --rm -it --init \
  arkouda-2026.07.15-cxi:latest \
  arkouda_server
```

See [docs/README.md](docs/README.md#3-running-the-arkouda-container)
for workstation vs. distributed HPE Cray EX (`e4s-cl`) run commands.

## Repository layout

```
containers/
├── Containerfile.hpe-cray-ex-chapel-pic  # Chapel base image (target 1)
└── Containerfile.arkouda                 # Arkouda (target 2)
scripts/
├── build-chapel-dist-cxi-2.3.1-pic.sh    # builds the Chapel base image
├── build-arkouda.sh                      # builds Arkouda on the Chapel base image
├── convert-to-sif.sh                     # OCI image -> Apptainer .sif
├── setup-e4s-cl-profile.sh               # HPC library-forwarding helper (e4s-cl)
├── generate-e4s-cl-profile.sh            # detects HPC libs for e4s-cl profiles
└── chapel-start / chapel-test-compile / chapel-validate-hpe-ex
patches/                                  # patches applied during the Arkouda build
docs/                                     # full build/usage guide
```
