
# Maintainer: Gabriel Brown <gabriel.h.brown@gmail.com>

#_gituser="chapel-lang"
_gituser="mppf" #temp: until PR #19913 is merged
_gitname="chapel"

pkgname=${_gitname}-git
_basepkgver=1.26
pkgver=1.26.cc0241ef9c
pkgrel=1
pkgdesc="Programming language designed for productive parallel computing at scale"
url="https://chapel-lang.org/"
arch=('x86_64' 'arm')
license=('Apache')
depends=('python' 'perl' 'llvm' 'clang')
makedepends=('git' 'cmake')
source=("git+https://github.com/${_gituser}/${_gitname}.git")
sha256sums=('SKIP') # source is not static

pkgver() {
  cd "${srcdir}/${_gitname}"
  git describe --long --tags --always | sed 's/\-/\./' | echo "${_basepkgver}.$(cat -)"
}

build() {
        cd "${srcdir}/${_gitname}"
        git checkout less-la #temp: until PR #19913 is merged
        ./configure --prefix=/usr
        make
}

check() {
        cd "${srcdir}/${_gitname}"
        export PATH="${srcdir}/${_gitname}/bin/linux64-x86_64:${PATH}"
        make check
}

package() {
        cd "${_gitname}"
        make DESTDIR="${pkgdir}" install
}
