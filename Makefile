#############################################################################
# Makefile for building: CHANGE-XF
# Generated by qmake (2.01a) (Qt 4.7.2) on: ?? 11? 28 21:16:32 2011
# Project:  CHANGE-XF.pro
# Template: app
# Command: /usr/bin/qmake-qt4 -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug -o Makefile CHANGE-XF.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -IE:\STM32һſ\I\IԴ\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x -IE:\STM32һſ\I\IԴ\Utilities\STM32_EVAL -IE:\STM32һſ\I\IԴ\Libraries\STM32F10x_StdPeriph_Driver\inc -IE:\Keil\ARM\INC\ST\STM32F10x -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.c \
		stm32f10x_it.c \
		EEPROM/eeprom.c \
		GPRS/gprs.c \
		GPS/gps.c \
		TIMERS/times.c \
		USART/usart.c \
		CHIPSET/chipset.c 
OBJECTS       = main.o \
		stm32f10x_it.o \
		eeprom.o \
		gprs.o \
		gps.o \
		times.o \
		usart.o \
		chipset.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		CHANGE-XF.pro
QMAKE_TARGET  = CHANGE-XF
DESTDIR       = 
TARGET        = CHANGE-XF

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: CHANGE-XF.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/libQtGui.prl \
		/usr/lib/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug -o Makefile CHANGE-XF.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/libQtGui.prl:
/usr/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ CONFIG+=debug -o Makefile CHANGE-XF.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/CHANGE-XF1.0.0 || $(MKDIR) .tmp/CHANGE-XF1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/CHANGE-XF1.0.0/ && $(COPY_FILE) --parents stm32f10x_conf.h stm32f10x_it.h EEPROM/eeprom.h GPRS/gprs.h GPS/gps.h TIMERS/times.h USART/usart.h CHIPSET/chipset.h .tmp/CHANGE-XF1.0.0/ && $(COPY_FILE) --parents main.c stm32f10x_it.c EEPROM/eeprom.c GPRS/gprs.c GPS/gps.c TIMERS/times.c USART/usart.c CHIPSET/chipset.c .tmp/CHANGE-XF1.0.0/ && (cd `dirname .tmp/CHANGE-XF1.0.0` && $(TAR) CHANGE-XF1.0.0.tar CHANGE-XF1.0.0 && $(COMPRESS) CHANGE-XF1.0.0.tar) && $(MOVE) `dirname .tmp/CHANGE-XF1.0.0`/CHANGE-XF1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/CHANGE-XF1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

main.o: main.c USART/usart.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o main.o main.c

stm32f10x_it.o: stm32f10x_it.c stm32f10x_it.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o stm32f10x_it.o stm32f10x_it.c

eeprom.o: EEPROM/eeprom.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o eeprom.o EEPROM/eeprom.c

gprs.o: GPRS/gprs.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o gprs.o GPRS/gprs.c

gps.o: GPS/gps.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o gps.o GPS/gps.c

times.o: TIMERS/times.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o times.o TIMERS/times.c

usart.o: USART/usart.c USART/usart.h \
		stm32f10x_conf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o usart.o USART/usart.c

chipset.o: CHIPSET/chipset.c USART/usart.h \
		stm32f10x_conf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o chipset.o CHIPSET/chipset.c

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

