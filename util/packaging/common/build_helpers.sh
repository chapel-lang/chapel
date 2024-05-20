

# required to enable BuildKit
export DOCKER_BUILDKIT=1

__get_docker_tag() {
  local os=$1
  local package_name=$2
  local chapel_version=$3
  local package_version=$4

  __docker_tag="chapel/${package_name}-${chapel_version}-${os}:${package_version}"
}

__wget_chpl_release() {
  local chapel_version=$1
  local release_dir="${CHPL_HOME}/util/packaging/tarballs"
  local release_file="${release_dir}/chapel-${chapel_version}.tar.gz"
  local url="https://github.com/chapel-lang/chapel/releases/download/${chapel_version}/chapel-${chapel_version}.tar.gz"

  if [ ! -f $release_file ]; then
    mkdir -p $release_dir
    wget -O $release_file $url
  fi
}

__build_packages() {
  # use this to build the package for linux/amd64 and linux/arm64
  local pkg_type=$1
  local os=$2
  local package_name=$3
  local chapel_version=$4
  local package_version=$5
  local docker_dir_name=$6

  # default to 1 core
  local para=${7:-1}

  __wget_chpl_release $chapel_version

  local package_dir="${CHPL_HOME}/util/packaging/${pkg_type}/${docker_dir_name}"
  __get_docker_tag $os $package_name $chapel_version $package_version


  pushd ${package_dir}
  docker buildx build \
    --platform linux/amd64,linux/arm64 \
    --output=type=docker \
    --build-arg "CHAPEL_VERSION=$chapel_version" \
    --build-arg "PACKAGE_VERSION=$package_version" \
    --build-arg "PARALLEL=$para" \
    -t $__docker_tag \
    -f Dockerfile ../..
  popd
}

__get_packages() {
  # extract the rpm packages from the docker image to the build directory
  local pkg_type=$1
  local os=$2
  local package_name=$3
  local chapel_version=$4
  local package_version=$5

  __get_docker_tag $os $package_name $chapel_version $package_version

  local pkg_dir="${CHPL_HOME}/util/packaging/${pkg_type}/"
  local out_dir="${pkg_dir}/build"
  mkdir -p ${out_dir}

  pushd ${pkg_dir}

  if [ "$pkg_type" = "rpm"]; then

    local name=`python3 ../common/package_name.py $package_name $chapel_version $package_version $os x86_64`
    docker create --platform linux/amd64 --name image-builder $__docker_tag && \
      docker cp image-builder:/home/user/rpmbuild/RPMS/x86_64/$name.rpm $out_dir && \
      docker rm image-builder

    local name=`python3 ../common/package_name.py $package_name $chapel_version $package_version $os aarch64`
    docker create --platform linux/arm64 --name image-builder $__docker_tag && \
      docker cp image-builder:/home/user/rpmbuild/RPMS/aarch64/$name.rpm $out_dir && \
      docker rm image-builder
  else if [ "$pkg_type" = "apt"]; then

    local name=`python3 ../common/package_name.py $package_name $chapel_version $package_version $os amd64`
    docker create --platform linux/amd64 --name image-builder $__docker_tag && \
      docker cp image-builder:/home/user/$name.deb $out_dir && \
      docker rm image-builder

    local name=`python3 ../common/package_name.py $package_name $chapel_version $package_version $os arm64`
    docker create --platform linux/arm64 --name image-builder $__docker_tag && \
      docker cp image-builder:/home/user/$name.deb $out_dir && \
      docker rm image-builder
  else
    echo "Invalid package type"
  fi
  fi

  popd
}


#
# native only, useful for testing
#

__build_package_native() {
  # use this to build the package for the host native architecture
  # this is significantly faster than building for all architectures
  # this is essentially the same as __build_package but with no `--platform`

  local pkg_type=$1
  local os=$2
  local package_name=$3
  local chapel_version=$4
  local package_version=$5
  local docker_dir_name=$6

  # default to 1 core
  local para=${7:-1}

  __wget_chpl_release $chapel_version

  local package_dir="${CHPL_HOME}/util/packaging/${pkg_type}/${docker_dir_name}"
  __get_docker_tag $os $package_name $chapel_version $package_version


  pushd ${package_dir}
  docker buildx build \
    --output=type=docker \
    --build-arg "CHAPEL_VERSION=$chapel_version" \
    --build-arg "PACKAGE_VERSION=$package_version" \
    --build-arg "PARALLEL=$para" \
    -t $__docker_tag \
    -f Dockerfile ../..
  popd
}


__get_package_native() {
  # extract the packages from the docker image to the build directory
  # this is essentially the same as __get_packages but with no `--platform`

  local pkg_type=$1
  local os=$2
  local package_name=$3
  local chapel_version=$4
  local package_version=$5

  __get_docker_tag $os $package_name $chapel_version $package_version

  local pkg_dir="${CHPL_HOME}/util/packaging/${pkg_type}/"
  local out_dir="${pkg_dir}/build"
  mkdir -p ${out_dir}

  pushd ${pkg_dir}

  local arch=`uname -m`
  if [ "$pkg_type" = "rpm" ]; then
    # change arm64 to aarch64
    if [ "$arch" = "arm64" ]; then
      arch="aarch64"
    fi
    # change amd64 to x86_64
    if [ "$arch" = "amd64" ]; then
      arch="x86_64"
    fi
  fi

  local name=`python3 ../common/package_name.py $package_name $chapel_version $package_version $os $arch`

  if [ "$pkg_type" = "rpm" ]; then
    docker create --name image-builder $__docker_tag && \
      docker cp image-builder:/home/user/rpmbuild/RPMS/$arch/$name.rpm $out_dir && \
      docker rm image-builder
  else if [ "$pkg_type" = "apt" ]; then
    docker create --name image-builder $__docker_tag && \
      docker cp image-builder:/home/user/$name.deb $out_dir && \
      docker rm image-builder
  else
    echo "Invalid package type"
  fi
  fi


  popd
}
