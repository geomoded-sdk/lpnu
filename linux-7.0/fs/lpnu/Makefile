obj-$(CONFIG_LPNU) += lpnu.o

lpnu-objs := lpnu_main.o lpnu_mach_o.o lpnu_vfs_compat.o lpnu_kext.o

ccflags-y += -I$(src)/include
ccflags-y += -I$(srctree)/$(src)/../xnu-xnu-11215.1.10/bsd/sys
ccflags-y += -I$(srctree)/$(src)/../xnu-xnu-11215.1.10/osfmk