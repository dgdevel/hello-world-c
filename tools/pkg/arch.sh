#!/bin/sh
# arch.sh - build the .pkg.tar.zst with makepkg; runs inside an
# archlinux:base-devel container as root (tools/package.sh invokes it) or
# natively as any non-root user with gcc/make/makepkg installed.
#
# proper pacman package (pacman -U), not a plain tarball: has .PKGINFO,
# .MTREE, license and docs. for AUR, publish a PKGBUILD pointing at the
# release tarball instead.
set -eu
VER=${VER:?}
cd "$(dirname "$0")/../.."

if [ "$(id -u)" = 0 ]; then
    pacman -Sy --needed --noconfirm gcc make ca-certificates >/dev/null
fi

rm -f hello-world-c test/selfcheck   # no make clean: must not wipe dist/
make "VERSION=$VER"
./hello-world-c version | grep -q "$VER" || { echo "arch.sh: version stamp not picked up" >&2; exit 1; }

# makepkg refuses to run as root; in a container build as a throwaway user
build=/tmp/pkgbuild
rm -rf "$build"
mkdir -p "$build"
cat > "$build/PKGBUILD" <<EOF
pkgname=hello-world-c
pkgver=$VER
pkgrel=1
pkgdesc='template c project with utilities'
arch=(x86_64)
url='https://github.com/YOURNAME/hello-world-c'
license=('EUPL-1.2')
options=(!debug)

package() {
    install -Dm755 "$PWD/hello-world-c" "\$pkgdir/usr/bin/hello-world-c"
    install -Dm644 "$PWD/LICENSE.md" "\$pkgdir/usr/share/licenses/hello-world-c/LICENSE.md"
    install -Dm644 "$PWD/README.md" "\$pkgdir/usr/share/doc/hello-world-c/README.md"
}
EOF

if [ "$(id -u)" = 0 ]; then
    useradd -m builder 2>/dev/null || true
    chown -R builder "$build"
    chmod -R a+rX .
    su builder -c "cd '$build' && PKGEXT='.pkg.tar.zst' makepkg -f"
    chown -R "$(stat -c '%u:%g' Makefile)" "$build"
else
    (cd "$build" && PKGEXT='.pkg.tar.zst' makepkg -f)
fi
mkdir -p dist
mv "$build"/hello-world-c-*.pkg.tar.zst dist/
echo "arch: $(ls dist/hello-world-c-*.pkg.tar.zst)"
