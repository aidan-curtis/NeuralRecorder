################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
smartrf_settings/%.obj: ../smartrf_settings/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/zhang/Desktop/TI/rfPacketTx_CC26X2R1_LAUNCHXL_nortos_ccs" --include_path="C:/ti/simplelink_cc26x2_sdk_2_20_00_36/source" --include_path="C:/ti/simplelink_cc26x2_sdk_2_20_00_36/kernel/nortos" --include_path="C:/ti/simplelink_cc26x2_sdk_2_20_00_36/kernel/nortos/posix" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=DeviceFamily_CC26X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="smartrf_settings/$(basename $(<F)).d_raw" --obj_directory="smartrf_settings" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


