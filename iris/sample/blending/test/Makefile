#
# Iris sample
# Olivier/Omar for lab-console
# Jully 2002
#   

# Files ---------------------------------------------------------------------------
TARGET 	= blend_test_iris.elf

OBJS 	=	main.o 
INCLUDE_PATH = ../../../include
INCLUDE_IRIS = $(INCLUDE_PATH)/iris3d
INCLUDE_OPCODE = $(INCLUDE_PATH)/opcode
KOS_CFLAGS += -I/usr/local/include/stlport -I$(INCLUDE_IRIS)/ -I$(INCLUDE_OPCODE)/ -O2
KOS_LDFLAGS += -L../../../lib/
IRIS_LIBS =  -Wl,-\(,-liris3d,-lstdc++,-lpng,-lz,-lgl,-ldcplib,-lm,-\)
# Base ----------------------------------------------------------------------------
all: 		$(TARGET)
include 	$(KOS_BASE)/Makefile.rules

# Link ----------------------------------------------------------------------------
$(TARGET): 	$(OBJS) romdisk.o
			$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $(TARGET) $(KOS_START) $(OBJS) romdisk.o $(OBJEXTRA) $(IRIS_LIBS) $(KOS_LIBS)

# ROM Disk Creation ---------------------------------------------------------------
rd :			
			$(KOS_GENROMFS) -f romdisk.img -d romdisk -v

romdisk.img :
			$(KOS_GENROMFS) -f romdisk.img -d romdisk -v

romdisk.o :	romdisk.img
			$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk romdisk.o

# Upload program on Dreamcast using $KOS_LOADER -----------------------------------
run: 		$(TARGET)
			$(KOS_LOADER) $(TARGET)

# Miscellaneous -------------------------------------------------------------------
dist:
			rm -f $(OBJS)
			$(KOS_STRIP) $(TARGET)

clean:		clean_elf
			-rm -f $(OBJS)

clean_elf:
			-rm -f $(TARGET) romdisk.*

#----------------------------------------------------------------------------------
