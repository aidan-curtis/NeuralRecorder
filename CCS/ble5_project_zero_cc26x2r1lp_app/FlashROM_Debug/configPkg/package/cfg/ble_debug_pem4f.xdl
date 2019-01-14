/*
 * Do not modify this file; it is automatically generated from the template
 * linkcmd.xdt in the ti.platforms.simplelink package and will be overwritten.
 */

/*
 * put '"'s around paths because, without this, the linker
 * considers '-' as minus operator, not a file name character.
 */


-l"C:\Users\zhang\Desktop\TI\ble5_project_zero_cc26x2r1lp_app\FlashROM_Debug\configPkg\package\cfg\ble_debug_pem4f.oem4f"
-l"C:\ti\simplelink_cc26x2_sdk_2_30_00_34\kernel\tirtos\packages\ti\uia\sysbios\lib\release\ti.uia.sysbios.aem4f"
-l"C:\ti\simplelink_cc26x2_sdk_2_30_00_34\kernel\tirtos\packages\ti\uia\loggers\lib\release\ti.uia.loggers.aem4f"
-l"C:\Users\zhang\Desktop\TI\ble5_project_zero_cc26x2r1lp_app\TOOLS\src\sysbios\sysbios.aem4f"
-l"C:\ti\simplelink_cc26x2_sdk_2_30_00_34\kernel\tirtos\packages\ti\uia\services\lib\release\ti.uia.services.aem4f"
-l"C:\ti\simplelink_cc26x2_sdk_2_30_00_34\kernel\tirtos\packages\ti\uia\runtime\lib\release\ti.uia.runtime.aem4f"
-l"C:\ti\simplelink_cc26x2_sdk_2_30_00_34\kernel\tirtos\packages\ti\uia\events\lib\release\ti.uia.events.aem4f"
-l"C:\ti\simplelink_cc26x2_sdk_2_30_00_34\kernel\tirtos\packages\ti\targets\arm\rtsarm\lib\boot.aem4f"
-l"C:\ti\simplelink_cc26x2_sdk_2_30_00_34\kernel\tirtos\packages\ti\targets\arm\rtsarm\lib\auto_init.aem4f"

--retain="*(xdc.meta)"

/* C6x Elf symbols */
--symbol_map __TI_STACK_SIZE=__STACK_SIZE
--symbol_map __TI_STACK_BASE=__stack
--symbol_map _stack=__stack


--args 0x0
-heap  0x0
-stack 0x400

/*
 * Linker command file contributions from all loaded packages:
 */

/* Content from xdc.services.global (null): */

/* Content from xdc (null): */

/* Content from xdc.corevers (null): */

/* Content from xdc.shelf (null): */

/* Content from xdc.services.spec (null): */

/* Content from xdc.services.intern.xsr (null): */

/* Content from xdc.services.intern.gen (null): */

/* Content from xdc.services.intern.cmd (null): */

/* Content from xdc.bld (null): */

/* Content from ti.targets (null): */

/* Content from ti.targets.arm.elf (null): */

/* Content from xdc.rov (null): */

/* Content from xdc.runtime (null): */

/* Content from ti.targets.arm.rtsarm (null): */

/* Content from ti.sysbios.interfaces (null): */

/* Content from ti.sysbios.family (null): */

/* Content from ti.sysbios.family.arm (ti/sysbios/family/arm/linkcmd.xdt): */
--retain "*(.vecs)"

/* Content from xdc.services.getset (null): */

/* Content from xdc.rta (null): */

/* Content from ti.uia.events (null): */

/* Content from ti.uia.runtime (null): */

/* Content from xdc.runtime.knl (null): */

/* Content from ti.sysbios.rts (ti/sysbios/rts/linkcmd.xdt): */

/* Content from ti.uia.services (null): */

/* Content from xdc.platform (null): */

/* Content from ti.catalog.arm.cortexm4 (null): */

/* Content from ti.catalog (null): */

/* Content from ti.catalog.peripherals.hdvicp2 (null): */

/* Content from ti.catalog.arm.peripherals.timers (null): */

/* Content from xdc.cfg (null): */

/* Content from ti.platforms.simplelink (null): */

/* Content from ti.sysbios.hal (null): */

/* Content from ti.sysbios (null): */

/* Content from ti.sysbios.family.arm.cc26xx (null): */

/* Content from ti.sysbios.family.arm.m3 (ti/sysbios/family/arm/m3/linkcmd.xdt): */
-u _c_int00
--retain "*(.resetVecs)"
ti_sysbios_family_arm_m3_Hwi_nvic = 0xe000e000;

/* Content from ti.sysbios.knl (null): */

/* Content from ti.sysbios.syncs (null): */

/* Content from ti.sysbios.heaps (null): */

/* Content from ti.sysbios.gates (null): */

/* Content from ti.sysbios.xdcruntime (null): */

/* Content from ti.uia.loggers (null): */

/* Content from ti.uia.sysbios (null): */

/* Content from ti.sysbios.utils (null): */

/* Content from configPkg (null): */

/* Content from xdc.services.io (null): */



/*
 * symbolic aliases for static instance objects
 */
xdc_runtime_Startup__EXECFXN__C = 1;
xdc_runtime_Startup__RESETFXN__C = 1;


SECTIONS
{
    .bootVecs:  type = DSECT
    .resetVecs: load > 0x50
    .vecs: load > 0x20000000, type = NOLOAD



    xdc.meta: type = COPY
}
