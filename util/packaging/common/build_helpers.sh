

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

__build_all_packages() {
  __build_packages $1 $2 $3 $4 $5 $6 $7 '--platform=linux/amd64,linux/arm64' $8
}
__build_x8664_package() {
  __build_packages $1 $2 $3 $4 $5 $6 $7 '--platform=linux/amd64' $8
}
__build_arm64_package() {
  __build_packages $1 $2 $3 $4 $5 $6 $7 '--platform=linux/arm64' $8
}
__build_native_package() {
  __build_packages $1 $2 $3 $4 $5 $6 $7 '' $8
}

__build_packages() {
  local pkg_type=$1
  local os=$2
  local package_name=$3
  local chapel_version=$4
  local package_version=$5
  local docker_dir_name=$6
  local docker_image_base=$7
  local architecture_string=$8

  # default to 1 core
  local para=${9:-1}

  __wget_chpl_release $chapel_version

  local package_dir="${CHPL_HOME}/util/packaging/${pkg_type}/${docker_dir_name}"
  local fill_script="${CHPL_HOME}/util/packaging/${pkg_type}/common/fill_docker_template.py"
  __get_docker_tag $os $package_name $chapel_version $package_version

  pushd ${package_dir}

  # Try to pull the latest version of this image, as a best effort.
  docker pull $docker_image_base || true
  # Whether we pulled successfully or not, use the image SHA256 digest to
  # identify the copy of it we have locally, so build can proceed with an old
  # version.
  export DOCKER_IMAGE_NAME_FULL="$(docker inspect --format='{{index .RepoDigests 0}}' $docker_image_base)"
  echo "Using image digest ${DOCKER_IMAGE_NAME_FULL} for $docker_image_base"

  # if there is a template file, use it to generate the Dockerfile
  if [ -f Dockerfile.template ]; then
    ${fill_script} Dockerfile.template
  fi
  set -x

  DOCKER_BUILDKIT=1 docker buildx build \
    $architecture_string \
    --output=type=local,dest=../build/$os-$package_name-$chapel_version-$package_version \
    --target=artifact \
    --build-arg "BASENAME=$package_name" \
    --build-arg "CHAPEL_VERSION=$chapel_version" \
    --build-arg "PACKAGE_VERSION=$package_version" \
    --build-arg "OS_NAME=$os" \
    --build-arg "DOCKER_DIR_NAME=$docker_dir_name" \
    --build-arg "DOCKER_IMAGE_NAME_FULL=$DOCKER_IMAGE_NAME_FULL" \
    --build-arg "PARALLEL=$para" \
    -t $__docker_tag \
    -f Dockerfile ../..
  popd
}

__build_image() {
  # use this to build a container image for local testing
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
  local fill_script="${CHPL_HOME}/util/packaging/${pkg_type}/common/fill_docker_template.py"
  __get_docker_tag $os $package_name $chapel_version $package_version

  pushd ${package_dir}

  # if there is a template file, use it to generate the Dockerfile
  if [ -f Dockerfile.template ]; then
    ${fill_script} Dockerfile.template
  fi

  DOCKER_BUILDKIT=1 docker buildx build \
    --load \
    --target=build \
    --build-arg "BASENAME=$package_name" \
    --build-arg "CHAPEL_VERSION=$chapel_version" \
    --build-arg "PACKAGE_VERSION=$package_version" \
    --build-arg "OS_NAME=$os" \
    --build-arg "DOCKER_DIR_NAME=$docker_dir_name" \
    --build-arg "PARALLEL=$para" \
    -t $__docker_tag \
    -f Dockerfile ../..
  popd
}
__run_container() {
  local os=$1
  local package_name=$2
  local chapel_version=$3
  local package_version=$4

  __get_docker_tag $os $package_name $chapel_version $package_version

  docker run -it --rm $__docker_tag
}

__test_package() {
  python3 $CHPL_HOME/util/packaging/common/test_package.py $@
}
__test_all_packages() {
  for deb in $(set +e && find $CHPL_HOME/util/packaging/apt/build -name '*.deb'); do
    __test_package $deb
  done
  for rpm in $(set +e && find $CHPL_HOME/util/packaging/rpm/build -name '*.rpm'); do
    __test_package $rpm
  done
}
