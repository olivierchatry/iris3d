#
# libiris3d
# Makefile (c)2002 Dan Potter
#   

OBJS =	animation/IrisSkeletal.o\
		animation/IrisSkeletalManager.o\
		model/IrisObjectManager.o \
		model/IrisImportObject.o \
		model/IrisObject.o \
		model/IrisManualObject.o \
		model/imd.o \
		model/imd2.o \
		IrisMaterial.o \
		IrisMaterialManager.o \
		IrisContext.o \
		IrisFrustrum.o \
		IrisFaceClipper.o \
		IrisLightingPipeline.o \
		helper/IrisHelper.o\
		collision/ColModel.o \
		collision/ColModelModel.o \
		collision/ColSphereModel.o \
		collision/ColRayModel.o
SUBDIRS =

LWIPDIR := $(KOS_BASE)/addons/lwip/src
LWIPARCH := kos

INCLUDE_PATH = ../../include
INCLUDE_IRIS = $(INCLUDE_PATH)/iris3d
INCLUDE_OPCODE = $(INCLUDE_PATH)/opcode

KOS_CFLAGS +=	-I/usr/local/include/stlport -I$(INCLUDE_IRIS)/ -I$(INCLUDE_OPCODE)/ -O2 \
				-I$(LWIPDIR)/include -I$(LWIPDIR)/arch/$(LWIPARCH)/include \
				-I$(LWIPDIR)/include/ipv4/ -I$(LWIPDIR)/.. -DLWIP_DEBUG -DIRIS_HELPER

myall: $(OBJS)
	rm -f ../../lib/libiris3d.a
	$(KOS_AR) rcs ../../lib/libiris3d.a $(OBJS)

include $(KOS_BASE)/Makefile.prefab


