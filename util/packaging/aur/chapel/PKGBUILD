
#    Maintainer: J. Emiliano Deustua <edeustua@gmail.com>
# Co-maintainer: Gabriel Brown <gabriel.h.brown@gmail.com>

pkgname=chapel
pkgver=1.26.0
pkgrel=1
pkgdesc="Programming language designed for productive parallel computing at scale"
url="https://chapel-lang.org/"
arch=('x86_64' 'arm')
license=('Apache')
depends=('python' 'perl' 'llvm' 'clang' 'gmp')
makedepends=('git' 'cmake' 'libtool')
source=("https://github.com/chapel-lang/chapel/releases/download/${pkgver}/chapel-${pkgver}.tar.gz")
sha256sums=('ba396b581f0a17f8da3f365a3f8b079b8d2e229a393fbd1756966b0019931ece')
            

build() {
        cd "$srcdir/${pkgname}-${pkgver}"
        export CHPL_LIB_PIC=pic # remove on next release a la
        # https://github.com/chapel-lang/chapel/pull/19785
        ./configure --prefix=/usr
        make
}

check() {
        cd "$srcdir/${pkgname}-${pkgver}"
        export PATH="$srcdir/${pkgname}-${pkgver}/bin/linux64-x86_64:$PATH"
        make check
}

package() {
        cd "$srcdir/${pkgname}-${pkgver}"
        make DESTDIR="${pkgdir}" install
}
