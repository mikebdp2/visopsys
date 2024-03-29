##
##  Visopsys
##  Copyright (C) 1998-2023 J. Andrew McLaughlin
##
##  Makefile
##

ROOT = ../../..
BUILDDIR = build
OUTPUTDIR = ${ROOT}/${BUILDDIR}/system/libraries
INCDIR = ../../include

include ${ROOT}/Makefile.include

STDDEPS = ${ROOT}/Makefile.include Makefile ${INCDIR}/*.h ${INCDIR}/sys/*.h \
	${INCDIR}/arch/${ARCH}/*.h
INCLUDE = -nostdinc -I${INCDIR}
CFLAGS = ${OPT} ${ARCHFLAGS} ${CCODEGEN} ${CWARN} ${INCLUDE} ${DEBUG}
LFLAGS = -nostdlib

ifdef DEBUG
	override DEBUG = -DDEBUG
else
	STRIP = strip -s ${SHAREDLIB}
endif

NAMES = \
	windowArchiveList \
	windowBannerDialog \
	windowCancelDialog \
	windowCenterDialog \
	windowChoiceDialog \
	windowColorDialog \
	windowFileDialog \
	windowFileList \
	windowKeyboard \
	windowLanguageDialog \
	windowMain \
	windowNumberDialog \
	windowOkDialog \
	windowPixelEditor \
	windowProgressDialog \
	windowPromptDialog \
	windowQueryDialog \
	windowRadioDialog \
	windowThumbImage

OBJDIR = obj
OBJS = $(addprefix ${OBJDIR}/, $(addsuffix .o, ${NAMES}))
PICOBJDIR = picobj
PICOBJS = $(addprefix ${PICOBJDIR}/, $(addsuffix .o, ${NAMES}))
LIBRARY = ${OUTPUTDIR}/libwindow.a
SONAME = libwindow.so
SHAREDLIB = ${OUTPUTDIR}/${SONAME}

# Targets

all: target-dirs ${LIBRARY} ${SHAREDLIB} strip
	${MAKE} -C de BUILDDIR=${BUILDDIR}
	${MAKE} -C es BUILDDIR=${BUILDDIR}
	${MAKE} -C ru BUILDDIR=${BUILDDIR}
	${MAKE} -C tr BUILDDIR=${BUILDDIR}

target-dirs:
	mkdir -p ${OBJDIR} ${PICOBJDIR} ${OUTPUTDIR}

${LIBRARY}: ${OBJS}
	${AR} -rs $@ ${OBJS}

${SHAREDLIB}: ${PICOBJS}
	${CC} ${ARCHFLAGS} -nostdlib -Wl,-shared -Wl,--soname=${SONAME} \
		-Wl,-z,norelro ${PICOBJS} -o $@

strip: ${SHAREDLIB}
	${STRIP}

${OBJDIR}/%.o: %.c ${STDDEPS}
	${CC} ${CFLAGS} ${LFLAGS} -c $< -o $@

${PICOBJDIR}/%.o: %.c ${STDDEPS}
	${CC} ${CFLAGS} ${LFLAGS} -fpic -c $< -o $@

clean:
	${MAKE} -C de BUILDDIR=${BUILDDIR} clean
	${MAKE} -C es BUILDDIR=${BUILDDIR} clean
	${MAKE} -C ru BUILDDIR=${BUILDDIR} clean
	${MAKE} -C tr BUILDDIR=${BUILDDIR} clean
	rm -Rf *~ *.o core ${OBJDIR} ${PICOBJDIR} ${LIBRARY} ${SHAREDLIB}

