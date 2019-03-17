################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
DspBootLoad/boot.obj: ../DspBootLoad/boot.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS/include" --include_path="C:/Users/Thinkpad-X1Carbon/git/Ti28035_Bootload/Ti28035_flashLoad/Can/can" --include_path="C:/Users/Thinkpad-X1Carbon/git/Ti28035_Bootload/Ti28035_flashLoad/Can/candsp" --include_path="C:/Users/Thinkpad-X1Carbon/git/Ti28035_Bootload/Ti28035_flashLoad/DspBootLoad" --include_path="C:/Users/Thinkpad-X1Carbon/git/Ti28035_Bootload/Ti28035_flashLoad/Public" --include_path="C:/Users/Thinkpad-X1Carbon/git/Ti28035_Bootload/Ti28035_flashLoad/Dsp/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="DspBootLoad/boot.d" --obj_directory="DspBootLoad" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

DspBootLoad/bootApp.obj: ../DspBootLoad/bootApp.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 --include_path="C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS/include" --include_path="C:/Users/Thinkpad-X1Carbon/git/Ti28035_Bootload/Ti28035_flashLoad/Can/can" --include_path="C:/Users/Thinkpad-X1Carbon/git/Ti28035_Bootload/Ti28035_flashLoad/Can/candsp" --include_path="C:/Users/Thinkpad-X1Carbon/git/Ti28035_Bootload/Ti28035_flashLoad/DspBootLoad" --include_path="C:/Users/Thinkpad-X1Carbon/git/Ti28035_Bootload/Ti28035_flashLoad/Public" --include_path="C:/Users/Thinkpad-X1Carbon/git/Ti28035_Bootload/Ti28035_flashLoad/Dsp/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="DspBootLoad/bootApp.d" --obj_directory="DspBootLoad" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


