

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
  DOCKER_BUILDKIT=1 docker buildx build \
    --platform linux/amd64,linux/arm64 \
    --output=type=local,dest=../build/$os-$package_name-$chapel_version-$package_version \
    --target=artifact \
    --build-arg "CHAPEL_VERSION=$chapel_version" \
    --build-arg "PACKAGE_VERSION=$package_version" \
    --build-arg "PARALLEL=$para" \
    -t $__docker_tag \
    -f Dockerfile ../..
  popd
}
