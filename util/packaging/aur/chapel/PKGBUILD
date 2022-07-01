
#    Maintainer: J. Emiliano Deustua <edeustua@gmail.com>
# Co-maintainer: Gabriel Brown <gabriel.h.brown@gmail.com>

pkgname=chapel
pkgver=1.27.0
pkgrel=1
pkgdesc="Programming language designed for productive parallel computing at scale"
url="https://chapel-lang.org/"
arch=('x86_64' 'arm')
license=('Apache')
depends=('python' 'perl' 'llvm' 'clang')
makedepends=('git' 'cmake')
source=("https://github.com/chapel-lang/chapel/releases/download/${pkgver}/chapel-${pkgver}.tar.gz")
sha256sums=('558b1376fb7757a5e1f254c717953f598a3e89850c8edd1936b8d09c464f3e8b')
            

build() {
        cd "$srcdir/${pkgname}-${pkgver}"
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
