XBE_TITLE = "EEasyXBChangeAV"
GEN_XISO = $(XBE_TITLE).iso
NXDK_CXX = y
NXDK_SDL = y
DEBUG = y
CFLAGS="-D_XBOX"
CXXFLAGS="-D_XBOX"

#Store the path to the EEasyXB Source directory
#This var is also used by the EEasyXB Makefile, and must
#be declared.
EEASYXB_SOURCE = $(CURDIR)/../../Source

#Include the EEasyXB Makefile to makebuild dependencies
include $(EEASYXB_SOURCE)/Makefile

SRCS += $(CURDIR)/main.cpp

include $(NXDK_DIR)/Makefile
