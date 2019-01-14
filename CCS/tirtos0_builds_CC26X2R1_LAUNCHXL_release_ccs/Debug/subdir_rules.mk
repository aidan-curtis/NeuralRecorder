################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1285151985:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1285151985-inproc

build-1285151985-inproc: ../release.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_50_08_24_core/xs" --xdcpath="C:/ti/simplelink_cc26x2_sdk_2_30_00_34/source;C:/ti/simplelink_cc26x2_sdk_2_30_00_34/kernel/tirtos/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.simplelink:CC2642 -r release -c "C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS" --compileOptions " -DDeviceFamily_CC26X2 " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1285151985 ../release.cfg
configPkg/compiler.opt: build-1285151985
configPkg/: build-1285151985


