include config.mk

DIRS=lib src plugins
DOCDIRS=man
DISTDIRS=man
DISTFILES= \
	apps/ \
	cmake/ \
	deps/ \
	examples/ \
	include/ \
	installer/ \
	lib/ \
	logo/ \
	man/ \
	misc/ \
	plugins/ \
	security/ \
	service/ \
	snap/ \
	src/ \
	test/ \
	\
	CMakeLists.txt \
	CONTRIBUTING.md \
	ChangeLog.txt \
	LICENSE.txt \
	Makefile \
	about.html \
	aclfile.example \
	config.h \
	config.mk \
	edl-v10 \
	epl-v20 \
	libmosquitto.pc.in \
	libmosquittopp.pc.in \
	mosquitto.conf \
	NOTICE.md \
	pskfile.example \
	pwfile.example \
	README-compiling.md \
	README-letsencrypt.md \
	README-windows.txt \
	README.md

.PHONY : all mosquitto api docs binary check clean reallyclean test install uninstall dist sign copy localdocker

all : $(MAKE_ALL)

api :
	mkdir -p api p
	naturaldocs -o HTML api -i lib -p p
	rm -rf p

docs :
	set -e; for d in ${DOCDIRS}; do $(MAKE) -C $${d}; done

binary : mosquitto

mosquitto :
ifeq ($(UNAME),Darwin)
	$(error Please compile using CMake on Mac OS X)
endif

	set -e; for d in ${DIRS}; do $(MAKE) -C $${d}; done

clean :
	set -e; for d in ${DIRS}; do $(MAKE) -C $${d} clean; done
	set -e; for d in ${DOCDIRS}; do $(MAKE) -C $${d} clean; done
	$(MAKE) -C test clean

reallyclean :
	set -e; for d in ${DIRS}; do $(MAKE) -C $${d} reallyclean; done
	set -e; for d in ${DOCDIRS}; do $(MAKE) -C $${d} reallyclean; done
	$(MAKE) -C test reallyclean
	-rm -f *.orig

check : test

test : mosquitto
	$(MAKE) -C test test

ptest : mosquitto
	$(MAKE) -C test ptest

utest : mosquitto
	$(MAKE) -C test utest

install : all
	set -e; for d in ${DIRS}; do $(MAKE) -C $${d} install; done
ifeq ($(WITH_DOCS),yes)
	set -e; for d in ${DOCDIRS}; do $(MAKE) -C $${d} install; done
endif

uninstall :
	set -e; for d in ${DIRS}; do $(MAKE) -C $${d} uninstall; done

dist : reallyclean
	set -e; for d in ${DISTDIRS}; do $(MAKE) -C $${d} dist; done
	mkdir -p dist/mosquitto-${VERSION}
	cp -r ${DISTFILES} dist/mosquitto-${VERSION}/
	cd dist; tar -zcf mosquitto-${VERSION}.tar.gz mosquitto-${VERSION}/

sign : dist
	cd dist; gpg --detach-sign -a mosquitto-${VERSION}.tar.gz

copy : sign
	cd dist; scp mosquitto-${VERSION}.tar.gz mosquitto-${VERSION}.tar.gz.asc mosquitto:site/mosquitto.org/files/source/
	scp ChangeLog.txt mosquitto:site/mosquitto.org/

coverage :
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory out

localdocker : reallyclean
	set -e; for d in ${DISTDIRS}; do $(MAKE) -C $${d} dist; done
	rm -rf dockertmp/
	mkdir -p dockertmp/mosquitto-${VERSION}
	cp -r ${DISTFILES} dockertmp/mosquitto-${VERSION}/
	cd dockertmp/; tar -zcf mosq.tar.gz mosquitto-${VERSION}/
	cp dockertmp/mosq.tar.gz docker/local
	rm -rf dockertmp/
	cd docker/local && docker build . -t eclipse-mosquitto:local
