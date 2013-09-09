#############################################################################
# Makefile for building: Paizhaoyi_Project
# Generated by qmake (2.01a) (Qt 4.8.2) on: Mon Sep 9 10:55:52 2013
# Project:  Paizhaoyi_Project.pro
# Template: app
# Command: /usr/bin/qmake -o Makefile Paizhaoyi_Project.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -IPaizhaoyi_CommonFuncs -IPaizhaoyi_Log -IPaizhaoyi_Model -IPaizhaoyi_DBProtocal -IPaizhaoyi_Network -IPaizhaoyi_NetProtocal -I/usr/include/mysql++ -I/usr/include/mysql -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu -lmysqlclient -lmysqlpp -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
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

SOURCES       = Paizhaoyi_CommonFuncs/CommonFuncs.cpp \
		Paizhaoyi_Log/Log.cpp \
		Paizhaoyi_Model/DataPool.cpp \
		Paizhaoyi_Model/DB.cpp \
		Paizhaoyi_Model/mysql_connection_yk.cpp \
		Paizhaoyi_Model/mysql_recordset_yk.cpp \
		Paizhaoyi_Network/Network.cpp \
		Paizhaoyi_Project/main.cpp 
OBJECTS       = CommonFuncs.o \
		Log.o \
		DataPool.o \
		DB.o \
		mysql_connection_yk.o \
		mysql_recordset_yk.o \
		Network.o \
		main.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		Paizhaoyi_Project.pro
QMAKE_TARGET  = Paizhaoyi_Project
DESTDIR       = ./bin 
TARGET        = PPaizhaoyi_Project

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

Makefile: Paizhaoyi_Project.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf
	$(QMAKE) -o Makefile Paizhaoyi_Project.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
qmake:  FORCE
	@$(QMAKE) -o Makefile Paizhaoyi_Project.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/Paizhaoyi_Project1.0.0 || $(MKDIR) .tmp/Paizhaoyi_Project1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/Paizhaoyi_Project1.0.0/ && $(COPY_FILE) --parents Paizhaoyi_CommonFuncs/CommonFuncs.h Paizhaoyi_DBProtocal/DBProtocal.h Paizhaoyi_Log/Log.h Paizhaoyi_Model/DataPool.h Paizhaoyi_Model/DB.h Paizhaoyi_Model/mysql_connection_yk.h Paizhaoyi_Model/mysql_recordset_yk.h Paizhaoyi_NetProtocal/NetProtocal.h Paizhaoyi_Network/Network.h .tmp/Paizhaoyi_Project1.0.0/ && $(COPY_FILE) --parents Paizhaoyi_CommonFuncs/CommonFuncs.cpp Paizhaoyi_Log/Log.cpp Paizhaoyi_Model/DataPool.cpp Paizhaoyi_Model/DB.cpp Paizhaoyi_Model/mysql_connection_yk.cpp Paizhaoyi_Model/mysql_recordset_yk.cpp Paizhaoyi_Network/Network.cpp Paizhaoyi_Project/main.cpp .tmp/Paizhaoyi_Project1.0.0/ && (cd `dirname .tmp/Paizhaoyi_Project1.0.0` && $(TAR) Paizhaoyi_Project1.0.0.tar Paizhaoyi_Project1.0.0 && $(COMPRESS) Paizhaoyi_Project1.0.0.tar) && $(MOVE) `dirname .tmp/Paizhaoyi_Project1.0.0`/Paizhaoyi_Project1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/Paizhaoyi_Project1.0.0


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

CommonFuncs.o: Paizhaoyi_CommonFuncs/CommonFuncs.cpp Paizhaoyi_CommonFuncs/CommonFuncs.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CommonFuncs.o Paizhaoyi_CommonFuncs/CommonFuncs.cpp

Log.o: Paizhaoyi_Log/Log.cpp Paizhaoyi_Log/Log.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Log.o Paizhaoyi_Log/Log.cpp

DataPool.o: Paizhaoyi_Model/DataPool.cpp Paizhaoyi_Model/DataPool.h \
		Paizhaoyi_DBProtocal/DBProtocal.h \
		Paizhaoyi_Log/Log.h \
		Paizhaoyi_CommonFuncs/CommonFuncs.h \
		Paizhaoyi_Network/Network.h \
		Paizhaoyi_NetProtocal/NetProtocal.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o DataPool.o Paizhaoyi_Model/DataPool.cpp

DB.o: Paizhaoyi_Model/DB.cpp Paizhaoyi_Model/DB.h \
		Paizhaoyi_DBProtocal/DBProtocal.h \
		Paizhaoyi_Model/DataPool.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o DB.o Paizhaoyi_Model/DB.cpp

mysql_connection_yk.o: Paizhaoyi_Model/mysql_connection_yk.cpp Paizhaoyi_Model/mysql_connection_yk.h \
		Paizhaoyi_Model/mysql_recordset_yk.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mysql_connection_yk.o Paizhaoyi_Model/mysql_connection_yk.cpp

mysql_recordset_yk.o: Paizhaoyi_Model/mysql_recordset_yk.cpp Paizhaoyi_Model/mysql_recordset_yk.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mysql_recordset_yk.o Paizhaoyi_Model/mysql_recordset_yk.cpp

Network.o: Paizhaoyi_Network/Network.cpp Paizhaoyi_Network/Network.h \
		Paizhaoyi_Log/Log.h \
		Paizhaoyi_NetProtocal/NetProtocal.h \
		Paizhaoyi_DBProtocal/DBProtocal.h \
		Paizhaoyi_Model/DB.h \
		Paizhaoyi_Model/DataPool.h \
		Paizhaoyi_CommonFuncs/CommonFuncs.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Network.o Paizhaoyi_Network/Network.cpp

main.o: Paizhaoyi_Project/main.cpp Paizhaoyi_Log/Log.h \
		Paizhaoyi_Network/Network.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o Paizhaoyi_Project/main.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

