TEMPLATE = lib
TARGET = KeePassbackend
QT += qml quick
CONFIG += qt plugin

load(ubuntu-click)

TARGET = $$qtLibraryTarget($$TARGET)

# Input
SOURCES += \
    backend.cpp \
    filesystem.cpp \
    ../../libs/3way.cpp \
    ../../libs/adler32.cpp \
    ../../libs/algebra.cpp \
    ../../libs/algparam.cpp \
    ../../libs/arc4.cpp \
    ../../libs/asn.cpp \
    ../../libs/authenc.cpp \
    ../../libs/base32.cpp \
    ../../libs/base64.cpp \
    ../../libs/basecode.cpp \
    ../../libs/bench.cpp \
    ../../libs/bench2.cpp \
    ../../libs/bfinit.cpp \
    ../../libs/blowfish.cpp \
    ../../libs/blumshub.cpp \
    ../../libs/camellia.cpp \
    ../../libs/cast.cpp \
    ../../libs/casts.cpp \
    ../../libs/cbcmac.cpp \
    ../../libs/ccm.cpp \
    ../../libs/channels.cpp \
    ../../libs/cmac.cpp \
    ../../libs/cpu.cpp \
    ../../libs/crc.cpp \
    ../../libs/cryptlib.cpp \
    ../../libs/cryptlib_bds.cpp \
    ../../libs/datatest.cpp \
    ../../libs/default.cpp \
    ../../libs/des.cpp \
    ../../libs/dessp.cpp \
    ../../libs/dh.cpp \
    ../../libs/dh2.cpp \
    ../../libs/dll.cpp \
    ../../libs/dlltest.cpp \
    ../../libs/dsa.cpp \
    ../../libs/eax.cpp \
    ../../libs/ec2n.cpp \
    ../../libs/eccrypto.cpp \
    ../../libs/ecp.cpp \
    ../../libs/elgamal.cpp \
    ../../libs/emsa2.cpp \
    ../../libs/eprecomp.cpp \
    ../../libs/esign.cpp \
    ../../libs/files.cpp \
    ../../libs/filters.cpp \
    ../../libs/fips140.cpp \
    ../../libs/fipsalgt.cpp \
    ../../libs/fipstest.cpp \
    ../../libs/gcm.cpp \
    ../../libs/gf256.cpp \
    ../../libs/gf2_32.cpp \
    ../../libs/gf2n.cpp \
    ../../libs/gfpcrypt.cpp \
    ../../libs/gost.cpp \
    ../../libs/gzip.cpp \
    ../../libs/hex.cpp \
    ../../libs/hmac.cpp \
    ../../libs/hrtimer.cpp \
    ../../libs/ida.cpp \
    ../../libs/idea.cpp \
    ../../libs/integer.cpp \
    ../../libs/iterhash.cpp \
    ../../libs/luc.cpp \
    ../../libs/mars.cpp \
    ../../libs/marss.cpp \
    ../../libs/md2.cpp \
    ../../libs/md4.cpp \
    ../../libs/md5.cpp \
    ../../libs/misc.cpp \
    ../../libs/modes.cpp \
    ../../libs/mqueue.cpp \
    ../../libs/mqv.cpp \
    ../../libs/nbtheory.cpp \
    ../../libs/network.cpp \
    ../../libs/oaep.cpp \
    ../../libs/osrng.cpp \
    ../../libs/panama.cpp \
    ../../libs/pch.cpp \
    ../../libs/pkcspad.cpp \
    ../../libs/polynomi.cpp \
    ../../libs/pssr.cpp \
    ../../libs/pubkey.cpp \
    ../../libs/queue.cpp \
    ../../libs/rabin.cpp \
    ../../libs/randpool.cpp \
    ../../libs/rc2.cpp \
    ../../libs/rc5.cpp \
    ../../libs/rc6.cpp \
    ../../libs/rdtables.cpp \
    ../../libs/regtest.cpp \
    ../../libs/rijndael.cpp \
    ../../libs/ripemd.cpp \
    ../../libs/rng.cpp \
    ../../libs/rsa.cpp \
    ../../libs/rw.cpp \
    ../../libs/safer.cpp \
    ../../libs/salsa.cpp \
    ../../libs/seal.cpp \
    ../../libs/seed.cpp \
    ../../libs/serpent.cpp \
    ../../libs/sha.cpp \
    ../../libs/sha3.cpp \
    ../../libs/shacal2.cpp \
    ../../libs/shark.cpp \
    ../../libs/sharkbox.cpp \
    ../../libs/simple.cpp \
    ../../libs/skipjack.cpp \
    ../../libs/socketft.cpp \
    ../../libs/sosemanuk.cpp \
    ../../libs/square.cpp \
    ../../libs/squaretb.cpp \
    ../../libs/strciphr.cpp \
    ../../libs/tea.cpp \
    ../../libs/tftables.cpp \
    ../../libs/tiger.cpp \
    ../../libs/tigertab.cpp \
    ../../libs/trdlocal.cpp \
    ../../libs/ttmac.cpp \
    ../../libs/twofish.cpp \
    ../../libs/validat1.cpp \
    ../../libs/validat2.cpp \
    ../../libs/validat3.cpp \
    ../../libs/vmac.cpp \
    ../../libs/wait.cpp \
    ../../libs/wake.cpp \
    ../../libs/whrlpool.cpp \
    ../../libs/winpipes.cpp \
    ../../libs/xtr.cpp \
    ../../libs/xtrcrypt.cpp \
    ../../libs/zdeflate.cpp \
    ../../libs/zinflate.cpp \
    ../../libs/zlib.cpp \
    compositekey.cpp \
    sha256.cpp \
    salsa20.cpp \
    hashedblockstream.cpp \
    ../../libs/xml/tinyxml2.cpp \
    readxmlfile.cpp \
    passwordentry.cpp \
    ../../KeePass/main.cpp

HEADERS += \
    backend.h \
    filesystem.h \
    ../../libs/3way.h \
    ../../libs/adler32.h \
    ../../libs/aes.h \
    ../../libs/algebra.h \
    ../../libs/algparam.h \
    ../../libs/arc4.h \
    ../../libs/argnames.h \
    ../../libs/asn.h \
    ../../libs/authenc.h \
    ../../libs/base32.h \
    ../../libs/base64.h \
    ../../libs/basecode.h \
    ../../libs/bench.h \
    ../../libs/blowfish.h \
    ../../libs/blumshub.h \
    ../../libs/camellia.h \
    ../../libs/cast.h \
    ../../libs/cbcmac.h \
    ../../libs/ccm.h \
    ../../libs/channels.h \
    ../../libs/cmac.h \
    ../../libs/config.h \
    ../../libs/cpu.h \
    ../../libs/crc.h \
    ../../libs/cryptlib.h \
    ../../libs/default.h \
    ../../libs/des.h \
    ../../libs/dh.h \
    ../../libs/dh2.h \
    ../../libs/dll.h \
    ../../libs/dmac.h \
    ../../libs/dsa.h \
    ../../libs/eax.h \
    ../../libs/ec2n.h \
    ../../libs/eccrypto.h \
    ../../libs/ecp.h \
    ../../libs/elgamal.h \
    ../../libs/emsa2.h \
    ../../libs/eprecomp.h \
    ../../libs/esign.h \
    ../../libs/factory.h \
    ../../libs/files.h \
    ../../libs/filters.h \
    ../../libs/fips140.h \
    ../../libs/fltrimpl.h \
    ../../libs/gcm.h \
    ../../libs/gf256.h \
    ../../libs/gf2_32.h \
    ../../libs/gf2n.h \
    ../../libs/gfpcrypt.h \
    ../../libs/gost.h \
    ../../libs/gzip.h \
    ../../libs/hex.h \
    ../../libs/hmac.h \
    ../../libs/hrtimer.h \
    ../../libs/ida.h \
    ../../libs/idea.h \
    ../../libs/integer.h \
    ../../libs/iterhash.h \
    ../../libs/lubyrack.h \
    ../../libs/luc.h \
    ../../libs/mars.h \
    ../../libs/md2.h \
    ../../libs/md4.h \
    ../../libs/md5.h \
    ../../libs/mdc.h \
    ../../libs/misc.h \
    ../../libs/modarith.h \
    ../../libs/modes.h \
    ../../libs/modexppc.h \
    ../../libs/mqueue.h \
    ../../libs/mqv.h \
    ../../libs/nbtheory.h \
    ../../libs/network.h \
    ../../libs/nr.h \
    ../../libs/oaep.h \
    ../../libs/oids.h \
    ../../libs/osrng.h \
    ../../libs/panama.h \
    ../../libs/pch.h \
    ../../libs/pkcspad.h \
    ../../libs/polynomi.h \
    ../../libs/pssr.h \
    ../../libs/pubkey.h \
    ../../libs/pwdbased.h \
    ../../libs/queue.h \
    ../../libs/rabin.h \
    ../../libs/randpool.h \
    ../../libs/rc2.h \
    ../../libs/rc5.h \
    ../../libs/rc6.h \
    ../../libs/resource.h \
    ../../libs/rijndael.h \
    ../../libs/ripemd.h \
    ../../libs/rng.h \
    ../../libs/rsa.h \
    ../../libs/rw.h \
    ../../libs/safer.h \
    ../../libs/salsa.h \
    ../../libs/seal.h \
    ../../libs/secblock.h \
    ../../libs/seckey.h \
    ../../libs/seed.h \
    ../../libs/serpent.h \
    ../../libs/serpentp.h \
    ../../libs/sha.h \
    ../../libs/sha3.h \
    ../../libs/shacal2.h \
    ../../libs/shark.h \
    ../../libs/simple.h \
    ../../libs/skipjack.h \
    ../../libs/smartptr.h \
    ../../libs/socketft.h \
    ../../libs/sosemanuk.h \
    ../../libs/square.h \
    ../../libs/stdcpp.h \
    ../../libs/strciphr.h \
    ../../libs/tea.h \
    ../../libs/tiger.h \
    ../../libs/trdlocal.h \
    ../../libs/trunhash.h \
    ../../libs/ttmac.h \
    ../../libs/twofish.h \
    ../../libs/validate.h \
    ../../libs/vmac.h \
    ../../libs/wait.h \
    ../../libs/wake.h \
    ../../libs/whrlpool.h \
    ../../libs/winpipes.h \
    ../../libs/words.h \
    ../../libs/xtr.h \
    ../../libs/xtrcrypt.h \
    ../../libs/zdeflate.h \
    ../../libs/zinflate.h \
    ../../libs/zlib.h \
    compositekey.h \
    sha256.h \
    salsa20.h \
    hashedblockstream.h \
    ../../libs/xml/tinyxml2.h \
    readxmlfile.h \
    passwordentry.h

OTHER_FILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
installPath = $${UBUNTU_CLICK_PLUGIN_PATH}/KeePass
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir


