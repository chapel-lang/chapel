

# required to enable BuildKit
export DOCKER_BUILDKIT=1

__get_docker_tag() {
  local os=$1
  local package_name=$2
  local chapel_version=$3
  local package_version=$4

  __docker_tag="chapel/${package_name}-${chapel_version}-${os}:${package_version}"
}

__build_rpm_package() {
  # use this to build the rpm package for linux/amd64 and linux/arm64
  local os=$1
  local package_name=$2
  local chapel_version=$3
  local package_version=$4

  # default to 1 core
  local para=${5:-1}

  local package_dir="${CHPL_HOME}/util/packaging/rpm/${os}"
  __get_docker_tag $os $package_name $chapel_version $package_version


  pushd ${package_dir}
  docker buildx build \
    --platform linux/amd64,linux/arm64 \
    --build-arg "CHAPEL_VERSION=$chapel_version" \
    --build-arg "PACKAGE_VERSION=$package_version" \
    --build-arg "PARALLEL=$para" \
    -t $__docker_tag \
    -f Dockerfile ..
  popd
}

__get_rpm_packages() {
  # extract the rpm packages from the docker image to the build directory
  local os=$1
  local package_name=$2
  local chapel_version=$3
  local package_version=$4

  __get_docker_tag $os $package_name $chapel_version $package_version

  local rpm_dir="${CHPL_HOME}/util/packaging/rpm/"
  local out_dir="${CHPL_HOME}/util/packaging/rpm/build"
  mkdir -p ${out_dir}

  pushd ${rpm_dir}

  local name=`python3 common/package_name.py $package_name $chapel_version $package_version $os x86_64`
  docker create --platform linux/amd64 --name image-builder $__docker_tag && \
    docker cp image-builder:/home/user/rpmbuild/RPMS/x86_64/$name.rpm $out_dir && \
    docker rm image-builder

  local name=`python3 common/package_name.py $package_name $chapel_version $package_version $os aarch64`
  docker create --platform linux/arm64 --name image-builder $__docker_tag && \
    docker cp image-builder:/home/user/rpmbuild/RPMS/aarch64/$name.rpm $out_dir && \
    docker rm image-builder

  popd
}



#
# native only, useful for testing
#

__build_rpm_package_native() {
  # use this to build the rpm package for the host native architecture
  # this is significantly faster than building for all architectures
  # this is essentially the same as __build_rpm_package but with no `--platform`

  local os=$1
  local package_name=$2
  local chapel_version=$3
  local package_version=$4

  # default to 1 core
  local para=${5:-1}

  local package_dir="${CHPL_HOME}/util/packaging/rpm/${os}"
  __get_docker_tag $os $package_name $chapel_version $package_version


  pushd ${package_dir}
  docker buildx build \
    --build-arg "CHAPEL_VERSION=$chapel_version" \
    --build-arg "PACKAGE_VERSION=$package_version" \
    --build-arg "PARALLEL=$para" \
    -t $__docker_tag \
    -f Dockerfile ..
  popd
}


__get_rpm_package_native() {
  # extract the rpm packages from the docker image to the build directory
  # this is essentially the same as __get_rpm_packages but with no `--platform`

  local os=$1
  local package_name=$2
  local chapel_version=$3
  local package_version=$4

  __get_docker_tag $os $package_name $chapel_version $package_version

  local rpm_dir="${CHPL_HOME}/util/packaging/rpm/"
  local out_dir="${CHPL_HOME}/util/packaging/rpm/build"
  mkdir -p ${out_dir}

  pushd ${rpm_dir}

  local arch=`uname -m`
  # change arm64 to aarch64
  if [ "$arch" = "arm64" ]; then
    arch="aarch64"
  fi
  # change amd64 to x86_64
  if [ "$arch" = "amd64" ]; then
    arch="x86_64"
  fi
  local name=`python3 common/package_name.py $package_name $chapel_version $package_version $os $arch`
  docker create --name image-builder $__docker_tag && \
    docker cp image-builder:/home/user/rpmbuild/RPMS/$arch/$name.rpm $out_dir && \
    docker rm image-builder

  popd
}
