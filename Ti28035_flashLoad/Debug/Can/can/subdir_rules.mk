################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Can/can/candriver.obj: ../Can/can/candriver.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS/include" --include_path="C:/software/ccs7.0/project/Ti28035_flashLoad/Can/can" --include_path="C:/software/ccs7.0/project/Ti28035_flashLoad/Can/candsp" --include_path="C:/software/ccs7.0/project/Ti28035_flashLoad/DspBootLoad" --include_path="C:/software/ccs7.0/project/Ti28035_flashLoad/Public" --include_path="C:/software/ccs7.0/project/Ti28035_flashLoad/Dsp/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Can/can/candriver.d" --obj_directory="Can/can" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Can/can/canmsgblock.obj: ../Can/can/canmsgblock.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS/include" --include_path="C:/software/ccs7.0/project/Ti28035_flashLoad/Can/can" --include_path="C:/software/ccs7.0/project/Ti28035_flashLoad/Can/candsp" --include_path="C:/software/ccs7.0/project/Ti28035_flashLoad/DspBootLoad" --include_path="C:/software/ccs7.0/project/Ti28035_flashLoad/Public" --include_path="C:/software/ccs7.0/project/Ti28035_flashLoad/Dsp/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Can/can/canmsgblock.d" --obj_directory="Can/can" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


