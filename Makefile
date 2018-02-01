COMMON_MAKEFILE_DIR = ../CommonMakefile
include $(COMMON_MAKEFILE_DIR)/FrontBase.mk

XMLPATH = ../TinyXml
SYSLIBPATH = ../SysLib
TOOLPATH = ../Tools
#APPLIBPATH = ../AppLib
#COMMLIBPATH = ../CommLib
PROBASELPATH = ../ProtocolBase
DEVLIBPATH = ../DeviceLib
LOADDEVICEPATH = ../LoadDevLib
#PROTOCOLPARAMETER = ../ProtocolParameter
#PROTOCOLDUAL = ../ProtocolDualRedundancy

INCS = -I$(XMLPATH) -I$(SYSLIBPATH) -I$(TOOLPATH) -I$(PROBASELPATH)\
	-I$(DEVLIBPATH) -I$(LOADDEVICEPATH)

OUTPUT = $(LIB_DIR)/RdbLib

EXTRA_FLAGS = -Wfatal-errors

#****************************************************************************
# objects 
#****************************************************************************
#OBJS = RdbLibVer.o SharedMemKey.o SharedMemOper.o StatRdbOper.o 
#ParameterMemOper.o

include $(COMMON_MAKEFILE_DIR)/BackLib.mk

