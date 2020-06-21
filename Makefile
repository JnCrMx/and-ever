ccflags-y += -I$(src) -Wno-error=implicit-int -Wno-int-conversion
obj-m += andever_linux.o
andever_linux-y := andever.o

KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build
VZ=	$(shell uname -r | grep vz)
ifneq ($(VZ),)
ccflags-y += -DVZKERNEL
endif

all:
	$(MAKE) -C $(KERNEL_SRC) V=0 M=$$PWD

clean:
	rm -rf deps.h *.o *.ko *.mod.c *.symvers *.order .*.cmd *.a *.mod .tmp_versions
