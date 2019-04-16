TEMPLATE = app
TARGET = KeePit

load(ubuntu-click)

QT += qml quick

SOURCES += main.cpp \
    backend.cpp \
    compositekey.cpp \
    hashedblockstream.cpp \
    passwordentry.cpp \
    readxmlfile.cpp \
    writexmlfile.cpp \
    salsa20.cpp \
    sha256.cpp \
    tinyxml2.cpp \
    cryptopp/zlib.cpp \
    cryptopp/zinflate.cpp \
    cryptopp/zdeflate.cpp \
    cryptopp/xtrcrypt.cpp \
    cryptopp/xtr.cpp \
    cryptopp/winpipes.cpp \
    cryptopp/whrlpool.cpp \
    cryptopp/wake.cpp \
    cryptopp/wait.cpp \
    cryptopp/vmac.cpp \
    cryptopp/twofish.cpp \
    cryptopp/ttmac.cpp \
    cryptopp/trdlocal.cpp \
    cryptopp/tigertab.cpp \
    cryptopp/tiger.cpp \
    cryptopp/tftables.cpp \
    cryptopp/tea.cpp \
    cryptopp/strciphr.cpp \
    cryptopp/squaretb.cpp \
    cryptopp/square.cpp \
    cryptopp/sosemanuk.cpp \
    cryptopp/socketft.cpp \
    cryptopp/skipjack.cpp \
    cryptopp/simple.cpp \
    cryptopp/sharkbox.cpp \
    cryptopp/shark.cpp \
    cryptopp/shacal2.cpp \
    cryptopp/sha3.cpp \
    cryptopp/sha.cpp \
    cryptopp/serpent.cpp \
    cryptopp/seed.cpp \
    cryptopp/seal.cpp \
    cryptopp/salsa.cpp \
    cryptopp/safer.cpp \
    cryptopp/rw.cpp \
    cryptopp/rsa.cpp \
    cryptopp/rng.cpp \
    cryptopp/ripemd.cpp \
    cryptopp/rijndael.cpp \
    cryptopp/regtest.cpp \
    cryptopp/rdtables.cpp \
    cryptopp/rc6.cpp \
    cryptopp/rc5.cpp \
    cryptopp/rc2.cpp \
    cryptopp/randpool.cpp \
    cryptopp/rabin.cpp \
    cryptopp/queue.cpp \
    cryptopp/pubkey.cpp \
    cryptopp/pssr.cpp \
    cryptopp/polynomi.cpp \
    cryptopp/pkcspad.cpp \
    cryptopp/pch.cpp \
    cryptopp/panama.cpp \
    cryptopp/osrng.cpp \
    cryptopp/oaep.cpp \
    cryptopp/network.cpp \
    cryptopp/nbtheory.cpp \
    cryptopp/mqv.cpp \
    cryptopp/mqueue.cpp \
    cryptopp/modes.cpp \
    cryptopp/misc.cpp \
    cryptopp/md5.cpp \
    cryptopp/md4.cpp \
    cryptopp/md2.cpp \
    cryptopp/marss.cpp \
    cryptopp/mars.cpp \
    cryptopp/luc.cpp \
    cryptopp/iterhash.cpp \
    cryptopp/integer.cpp \
    cryptopp/idea.cpp \
    cryptopp/ida.cpp \
    cryptopp/hrtimer.cpp \
    cryptopp/hmac.cpp \
    cryptopp/hex.cpp \
    cryptopp/gzip.cpp \
    cryptopp/gost.cpp \
    cryptopp/gfpcrypt.cpp \
    cryptopp/gf256.cpp \
    cryptopp/gf2n.cpp \
    cryptopp/gf2_32.cpp \
    cryptopp/gcm.cpp \
    cryptopp/fipstest.cpp \
    cryptopp/fipsalgt.cpp \
    cryptopp/fips140.cpp \
    cryptopp/filters.cpp \
    cryptopp/files.cpp \
    cryptopp/esign.cpp \
    cryptopp/eprecomp.cpp \
    cryptopp/emsa2.cpp \
    cryptopp/elgamal.cpp \
    cryptopp/ecp.cpp \
    cryptopp/eccrypto.cpp \
    cryptopp/ec2n.cpp \
    cryptopp/eax.cpp \
    cryptopp/dsa.cpp \
    cryptopp/dlltest.cpp \
    cryptopp/dll.cpp \
    cryptopp/dh2.cpp \
    cryptopp/dh.cpp \
    cryptopp/dessp.cpp \
    cryptopp/des.cpp \
    cryptopp/default.cpp \
    cryptopp/cryptlib.cpp \
    cryptopp/cryptlib_bds.cpp \
    cryptopp/crc.cpp \
    cryptopp/cpu.cpp \
    cryptopp/cmac.cpp \
    cryptopp/channels.cpp \
    cryptopp/ccm.cpp \
    cryptopp/cbcmac.cpp \
    cryptopp/casts.cpp \
    cryptopp/cast.cpp \
    cryptopp/camellia.cpp \
    cryptopp/blumshub.cpp \
    cryptopp/blowfish.cpp \
    cryptopp/bfinit.cpp \
    cryptopp/basecode.cpp \
    cryptopp/base32.cpp \
    cryptopp/authenc.cpp \
    cryptopp/asn.cpp \
    cryptopp/arc4.cpp \
    cryptopp/algparam.cpp \
    cryptopp/algebra.cpp \
    cryptopp/adler32.cpp \
    cryptopp/3way.cpp \
    treenode.cpp \
    base64.cpp \
    readkeyfile.cpp \
    database.cpp \
    arrayextensions.cpp \
    aes.cpp \
    filehandler.cpp \
    bytestream.cpp \
    hexx.cpp \
    uuid.cpp

RESOURCES += KeePit.qrc

CONF_FILES +=  KeePit.apparmor \
               KeePit.png \
               KeePit.contenthub

#specify where the config files are installed to
config_files.path = /KeePit
config_files.files += $${CONF_FILES}
INSTALLS+=config_files

#install the desktop file, a translated version is
#automatically created in the build directory
desktop_file.path = /KeePit
desktop_file.files = $$OUT_PWD/KeePit.desktop
desktop_file.CONFIG += no_check_exist
INSTALLS+=desktop_file

# Default rules for deployment.
target.path = $${UBUNTU_CLICK_BINARY_PATH}
INSTALLS+=target

HEADERS += \
    backend.h \
    compositekey.h \
    hashedblockstream.h \
    passwordentry.h \
    readxmlfile.h \
    writexmlfile.h \
    salsa20.h \
    sha256.h \
    tinyxml2.h \
    cryptopp/zlib.h \
    cryptopp/zinflate.h \
    cryptopp/zdeflate.h \
    cryptopp/xtrcrypt.h \
    cryptopp/xtr.h \
    cryptopp/words.h \
    cryptopp/winpipes.h \
    cryptopp/whrlpool.h \
    cryptopp/wake.h \
    cryptopp/wait.h \
    cryptopp/vmac.h \
    cryptopp/validate.h \
    cryptopp/twofish.h \
    cryptopp/ttmac.h \
    cryptopp/trunhash.h \
    cryptopp/trdlocal.h \
    cryptopp/tiger.h \
    cryptopp/tea.h \
    cryptopp/strciphr.h \
    cryptopp/stdcpp.h \
    cryptopp/square.h \
    cryptopp/sosemanuk.h \
    cryptopp/socketft.h \
    cryptopp/smartptr.h \
    cryptopp/skipjack.h \
    cryptopp/simple.h \
    cryptopp/shark.h \
    cryptopp/shacal2.h \
    cryptopp/sha3.h \
    cryptopp/sha.h \
    cryptopp/serpentp.h \
    cryptopp/serpent.h \
    cryptopp/seed.h \
    cryptopp/seckey.h \
    cryptopp/secblock.h \
    cryptopp/seal.h \
    cryptopp/salsa.h \
    cryptopp/safer.h \
    cryptopp/rw.h \
    cryptopp/rsa.h \
    cryptopp/rng.h \
    cryptopp/ripemd.h \
    cryptopp/rijndael.h \
    cryptopp/resource.h \
    cryptopp/rc6.h \
    cryptopp/rc5.h \
    cryptopp/rc2.h \
    cryptopp/randpool.h \
    cryptopp/rabin.h \
    cryptopp/queue.h \
    cryptopp/pwdbased.h \
    cryptopp/pubkey.h \
    cryptopp/pssr.h \
    cryptopp/polynomi.h \
    cryptopp/pkcspad.h \
    cryptopp/pch.h \
    cryptopp/panama.h \
    cryptopp/osrng.h \
    cryptopp/oids.h \
    cryptopp/oaep.h \
    cryptopp/nr.h \
    cryptopp/network.h \
    cryptopp/nbtheory.h \
    cryptopp/mqv.h \
    cryptopp/mqueue.h \
    cryptopp/modexppc.h \
    cryptopp/modes.h \
    cryptopp/modarith.h \
    cryptopp/misc.h \
    cryptopp/mdc.h \
    cryptopp/md5.h \
    cryptopp/md4.h \
    cryptopp/md2.h \
    cryptopp/mars.h \
    cryptopp/luc.h \
    cryptopp/lubyrack.h \
    cryptopp/iterhash.h \
    cryptopp/integer.h \
    cryptopp/idea.h \
    cryptopp/ida.h \
    cryptopp/hrtimer.h \
    cryptopp/hmac.h \
    cryptopp/hex.h \
    cryptopp/gzip.h \
    cryptopp/gost.h \
    cryptopp/gfpcrypt.h \
    cryptopp/gf256.h \
    cryptopp/gf2n.h \
    cryptopp/gf2_32.h \
    cryptopp/gcm.h \
    cryptopp/fltrimpl.h \
    cryptopp/fips140.h \
    cryptopp/filters.h \
    cryptopp/files.h \
    cryptopp/factory.h \
    cryptopp/esign.h \
    cryptopp/eprecomp.h \
    cryptopp/emsa2.h \
    cryptopp/elgamal.h \
    cryptopp/ecp.h \
    cryptopp/eccrypto.h \
    cryptopp/ec2n.h \
    cryptopp/eax.h \
    cryptopp/dsa.h \
    cryptopp/dmac.h \
    cryptopp/dll.h \
    cryptopp/dh2.h \
    cryptopp/dh.h \
    cryptopp/des.h \
    cryptopp/default.h \
    cryptopp/cryptopp.rc \
    cryptopp/cryptlib.h \
    cryptopp/crc.h \
    cryptopp/cpu.h \
    cryptopp/config.h \
    cryptopp/cmac.h \
    cryptopp/channels.h \
    cryptopp/ccm.h \
    cryptopp/cbcmac.h \
    cryptopp/cast.h \
    cryptopp/camellia.h \
    cryptopp/blumshub.h \
    cryptopp/blowfish.h \
    cryptopp/bench.h \
    cryptopp/basecode.h \
    cryptopp/base32.h \
    cryptopp/authenc.h \
    cryptopp/asn.h \
    cryptopp/argnames.h \
    cryptopp/arc4.h \
    cryptopp/algparam.h \
    cryptopp/algebra.h \
    cryptopp/aes.h \
    cryptopp/adler32.h \
    cryptopp/adhoc.cpp.proto \
    cryptopp/3way.h \
    treenode.h \
    base64.h \
    readkeyfile.h \
    database.h \
    arrayextensions.h \
    aes.h \
    filehandler.h \
    bytestream.h \
    stdafx.h \
    hexx.h \
    uuid.h

LIBS += -L$$OUT_PWD/../ziplib/ -lziplib

INCLUDEPATH += $$PWD/../ziplib
DEPENDPATH += $$PWD/../ziplib

#PRE_TARGETDEPS += $$OUT_PWD/../ziplib/libziplib.a
