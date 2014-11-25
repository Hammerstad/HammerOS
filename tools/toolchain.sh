BINUTILS_VERSION="2.23.1"
BINUTILS_RELEASE=""
GCC_VERSION="4.8.1"

BASEDIR="`pwd`"
SRCDIR="$(readlink -f $(dirname "$0"))"
BINUTILS="binutils-${BINUTILS_VERSION}${BINUTILS_RELEASE}.tar.bz2"
GCC="gcc-${GCC_VERSION}.tar.bz2"

DEPENDENCIES="gcc make bison flex libgmp3-dev libmpfr-dev libmpc-dev texinfo qemu-system-i386 g++"

check_error() {
    if [ "$1" -ne "0" ]; then
        echo
        echo "Script failed: $2"
        
        exit 1
    fi
}

check_md5() {
    FILE="$1"
    SUM="$2"
    
    COMPUTED="`md5sum "${FILE}" | cut -d' ' -f1`"
    if [ "${SUM}" != "${COMPUTED}" ] ; then
        echo
        echo "Checksum of ${FILE} does not match."
        
        exit 2
    fi
}

show_usage() {
    echo "Cross-compiler toolchain build script"
    echo
    echo "Syntax:"
    echo " $0 <platform>"
    echo
    echo "Possible target platforms are:"
    echo " ia32       IA-32 (x86, i386)"
    echo " install    installs dependencies for cross compiler"
    echo " all        build all targets"
    echo " cleandl    cleans the downloaded files"
    echo " cleanwork  cleans the work directory"
    echo " clean      cleans the work directory and downloaded files"
    echo
    echo "The toolchain is installed into directory specified by the"
    echo "CROSS_PREFIX environment variable. If the variable is not"
    echo "defined, /usr/local/cross/ is used as default."
    echo
    
    exit 3
}

change_title() {
    echo -en "\e]0;$1\a"
}

show_dependencies() {
    echo "IMPORTANT NOTICE:"
    echo
    echo "For a successful compilation and use of the cross-compiler"
    echo "toolchain you need at least the following dependencies."
    echo
    echo "Please make sure that the dependencies are present in your"
    echo "system. Otherwise the compilation process might fail after"
    echo "a few seconds or minutes."
    echo
    echo " - GCC"
    echo " - GNU Make"
    echo " - GNU Bison"
    echo " - Flex"
    echo " - GNU GMP"
    echo " - GNU MPFR"
    echo " - GNU MPC"
    echo " - Tar"
    echo " - Texinfo"
    echo " - Qemu-system-i386"
    echo
}

install(){
    PACKAGE=$1
    sudo apt-get install -y $1 > /dev/null
    if [[ $? > 0 ]]
    then
        echo "Unable to install $PACKAGE. Exiting."
        exit 1
    else
        echo "$PACKAGE installed successfully."
    fi
}

install_dependencies(){
    for PACKAGE in $DEPENDENCIES
    do
        PACKAGE_INSTALLED=$(dpkg-query -Wf '${db:Status-abbrev}' "$PACKAGE" 2>/dev/null | grep '^ii')
        if [[ "" == $PACKAGE_INSTALLED ]]
        then
            echo -e " >>> $PACKAGE not found. Install? (y/n) \c"
            read
            if [[ "$REPLY" == "y" ]]
            then
                install $PACKAGE
            fi
        else
            echo ">>> $PACKAGE already installed. Moving on..."
        fi
    done
    echo
}

download_fetch() {
    SOURCE="$1"
    FILE="$2"
    CHECKSUM="$3"
    
    if [ ! -f "${FILE}" ]; then
        change_title "Downloading ${FILE}"
        wget -c "${SOURCE}${FILE}"
        check_error $? "Error downloading ${FILE}."
    fi
    
    check_md5 "${FILE}" "${CHECKSUM}"
}

source_check() {
    FILE="$1"
    
    if [ ! -f "${FILE}" ]; then
        echo
        echo "File ${FILE} not found."
        
        exit 4
    fi
}

cleanup_dir() {
    DIR="$1"
    
    if [ -d "${DIR}" ]; then
        change_title "Removing ${DIR}"
        echo " >>> Removing ${DIR}"
        rm -rf "${DIR}"
    fi
}

create_dir() {
    DIR="$1"
    DESC="$2"
    
    change_title "Creating ${DESC}"
    echo ">>> Creating ${DESC}"
    
    mkdir -p "${DIR}"
    test -d "${DIR}"
    check_error $? "Unable to create ${DIR}."
}

check_dirs() {
    OUTSIDE="$1"
    BASE="$2"
    ORIGINAL="`pwd`"
    
    cd "${OUTSIDE}"
    check_error $? "Unable to change directory to ${OUTSIDE}."
    ABS_OUTSIDE="`pwd`"
    
    cd "${BASE}"
    check_error $? "Unable to change directory to ${BASE}."
    ABS_BASE="`pwd`"
    
    cd "${ORIGINAL}"
    check_error $? "Unable to change directory to ${ORIGINAL}."
    
    BASE_LEN="${#ABS_BASE}"
    OUTSIDE_TRIM="${ABS_OUTSIDE:0:${BASE_LEN}}"
    
    if [ "${OUTSIDE_TRIM}" == "${ABS_BASE}" ] ; then
        echo
        echo "CROSS_PREFIX cannot reside within the working directory."
        
        exit 5
    fi
}

unpack_tarball() {
    FILE="$1"
    DESC="$2"
    
    change_title "Unpacking ${DESC}"
    echo " >>> Unpacking ${DESC}"
    
    tar -xjf "${FILE}"
    check_error $? "Error unpacking ${DESC}."
}

prepare() {
    show_dependencies
    install_dependencies
    
    BINUTILS_SOURCE="ftp://ftp.gnu.org/gnu/binutils/"
    GCC_SOURCE="ftp://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/"
    
    download_fetch "${BINUTILS_SOURCE}" "${BINUTILS}" "33adb18c3048d057ac58d07a3f1adb38"
    download_fetch "${GCC_SOURCE}" "${GCC}" "3b2386c114cd74185aa3754b58a79304"
}

set_target_from_platform() {
    case "$1" in
        "ia32")
            TARGET="i686-elf"
            ;;
        *)
            check_error 1 "No target known for $1."
            ;;
    esac
}

build_binutils(){
    echo ">>> Processing binutils (${PLATFORM})"
    create_dir $BINUTILSBUILDDIR "Binutils build directory"
    cd $BINUTILSBUILDDIR
    check_error $? "Change directory failed."
    
    change_title "binutils: configure (${PLATFORM})"
    ${BINUTILSDIR}/configure --target=${TARGET} --prefix=${PREFIX} --disable-nls --disable-werror --with-sysroot
    check_error $? "Error configuring binutils."
    
    change_title "binutils: make (${PLATFORM})"
    make
    check_error $? "Error compiling binutils."
    
    change_title "binutils: install (${PLATFORM})"
    make install
    check_error $? "Error installing binutils."
}

build_gcc(){
    echo ">>> Processing GCC (${PLATFORM})"
    create_dir "${GCCBUILDDIR}" "GCC build directory"
    cd "${GCCBUILDDIR}"
    check_error $? "Change directory failed."
    
    change_title "GCC: configure (${PLATFORM})"
    ${GCCDIR}/configure --target=${TARGET} --prefix=${PREFIX} --disable-nls --enable-languages=c,c++ --without-headers
    check_error $? "Error configuring GCC."
    
    change_title "GCC: make all-gcc (${PLATFORM})"
    make all-gcc
    check_error $? "Error compiling GCC."
    
    change_title "GCC: make all-target-libgcc (${PLATFORM})"
    make all-target-libgcc
    check_error $? "Error compiling LIBGCC."
    
    change_title "GCC: install-gcc (${PLATFORM})"
    make install-gcc
    check_error $? "Error installing GCC."
    
    change_title "GCC: install-target-libgcc (${PLATFORM})"
    make install-target-libgcc
    check_error $? "Error installing LIBGCC."
}

build_target() {
    PLATFORM="$1"
    set_target_from_platform "$PLATFORM"
    
    WORKDIR="${BASEDIR}/work/${PLATFORM}"
    BINUTILSDIR="${WORKDIR}/binutils-${BINUTILS_VERSION}"
    BINUTILSBUILDDIR="${WORKDIR}/build-binutils"
    GCCDIR="${WORKDIR}/gcc-${GCC_VERSION}"
    GCCBUILDDIR="${WORKDIR}/build-gcc"
    
    if [ -z "${CROSS_PREFIX}" ] ; then
        CROSS_PREFIX="/usr/local/cross"
    fi
    
    PREFIX="${CROSS_PREFIX}/${PLATFORM}"
        
    echo ">>> Downloading tarballs"
    source_check "${BASEDIR}/${BINUTILS}"
    source_check "${BASEDIR}/${GCC}"
    
    create_dir "${PREFIX}" "destination directory"
    create_dir "${WORKDIR}" "working directory"
    
    check_dirs "${PREFIX}" "${WORKDIR}"
    
    echo ">>> Unpacking tarballs"
    cd "${WORKDIR}"
    check_error $? "Change directory failed."
    
    unpack_tarball "${BASEDIR}/${BINUTILS}" "binutils"
    unpack_tarball "${BASEDIR}/${GCC}" "GCC"

    build_binutils
    build_gcc
    
    
    cd "${BASEDIR}"
    check_error $? "Change directory failed."
    
    echo
    echo ">>> Cross-compiler for ${TARGET} installed."
}

#Do stuff

while [ "$#" -gt 1 ]; do
    case "$1" in
        *)
            show_usage
            ;;
    esac
done

if [ "$#" -lt "1" ]; then
    show_usage
fi

case "$1" in
    "install")
        install_dependencies
        ;;
    ia32)
        prepare
        build_target "$1"
        ;;
    "all")
        prepare
        build_target "ia32"
        ;;
    "cleandl")
        #rm "${BASEDIR}/${BINUTILS}"
        #rm "${BASEDIR}/${GCC}"
        ;;
    "cleanwork")
        rm -rf "work"
        ;;
    "clean")
        #rm "${BASEDIR}/${BINUTILS}"
        #rm "${BASEDIR}/${GCC}"
        rm -rf "work"
        ;;
    *)
        show_usage
        ;;
esac
