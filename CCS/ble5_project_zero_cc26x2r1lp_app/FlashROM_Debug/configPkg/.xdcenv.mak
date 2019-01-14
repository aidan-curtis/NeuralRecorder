#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/simplelink_cc26x2_sdk_2_30_00_34/source;C:/ti/simplelink_cc26x2_sdk_2_30_00_34/kernel/tirtos/packages;C:/ti/simplelink_cc26x2_sdk_2_30_00_34/source/ti/ble5stack
override XDCROOT = C:/ti/xdctools_3_50_08_24_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/simplelink_cc26x2_sdk_2_30_00_34/source;C:/ti/simplelink_cc26x2_sdk_2_30_00_34/kernel/tirtos/packages;C:/ti/simplelink_cc26x2_sdk_2_30_00_34/source/ti/ble5stack;C:/ti/xdctools_3_50_08_24_core/packages;..
HOSTOS = Windows
endif
