################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/Users/aidancurtis/NeuralRecorder/CCSprojects/rfPacketTx_CC26X2R1_LAUNCHXL_nortos_ccs" --include_path="/Users/aidancurtis/ti/simplelink_cc26x2_sdk_2_20_00_36/source" --include_path="/Users/aidancurtis/ti/simplelink_cc26x2_sdk_2_20_00_36/kernel/nortos" --include_path="/Users/aidancurtis/ti/simplelink_cc26x2_sdk_2_20_00_36/kernel/nortos/posix" --include_path="/Applications/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include" --define=DeviceFamily_CC26X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


