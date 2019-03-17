################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./Can/can/candriver.obj" \
"./Can/can/canmsgblock.obj" \
"./Can/candsp/candsp.obj" \
"./Dsp/src/DSP2803x_CSMPasswords.obj" \
"./Dsp/src/DSP2803x_CodeStartBranch.obj" \
"./Dsp/src/DSP2803x_CpuTimers.obj" \
"./Dsp/src/DSP2803x_GlobalVariableDefs.obj" \
"./Dsp/src/DSP2803x_PieCtrl.obj" \
"./Dsp/src/DSP2803x_Sci.obj" \
"./Dsp/src/DSP2803x_SysCtrl.obj" \
"./Dsp/src/DSP2803x_usDelay.obj" \
"./DspBootLoad/boot.obj" \
"./DspBootLoad/bootApp.obj" \
"./Public/assertEx.obj" \
"./Public/basic.obj" \
"./Public/crc.obj" \
"./Public/mempool.obj" \
"../2803x_FlashAPI_BootROMSymbols.lib" \
"../Dsp/cmd/DSP2803x_Headers_nonBIOS.cmd" \
"../Dsp/cmd/F28035.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Can/can/subdir_vars.mk
-include Can/candsp/subdir_vars.mk
-include Dsp/cmd/subdir_vars.mk
-include Dsp/src/subdir_vars.mk
-include DspBootLoad/subdir_vars.mk
-include Public/subdir_vars.mk
-include subdir_rules.mk
-include Can/can/subdir_rules.mk
-include Can/candsp/subdir_rules.mk
-include Dsp/cmd/subdir_rules.mk
-include Dsp/src/subdir_rules.mk
-include DspBootLoad/subdir_rules.mk
-include Public/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Ti28035_flashLoad.out \

EXE_OUTPUTS__QUOTED += \
"Ti28035_flashLoad.out" \

BIN_OUTPUTS += \
Ti28035_flashLoad.hex \

BIN_OUTPUTS__QUOTED += \
"Ti28035_flashLoad.hex" \


# All Target
all: Ti28035_flashLoad.out secondary-outputs

# Tool invocations
Ti28035_flashLoad.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla0 -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"Ti28035_flashLoad.map" --stack_size=0x300 --warn_sections -i"C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS/lib" -i"C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="Ti28035_flashLoad_linkInfo.xml" --rom_model -o "Ti28035_flashLoad.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

Ti28035_flashLoad.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C2000 Hex Utility'
	"C:/software/ccs7.0/ccsv7/tools/compiler/ti-cgt-c2000_16.9.1.LTS/bin/hex2000" --memwidth=16 --gpio16 --ascii -o "Ti28035_flashLoad.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "Can\can\candriver.obj" "Can\can\canmsgblock.obj" "Can\candsp\candsp.obj" "Dsp\src\DSP2803x_CSMPasswords.obj" "Dsp\src\DSP2803x_CodeStartBranch.obj" "Dsp\src\DSP2803x_CpuTimers.obj" "Dsp\src\DSP2803x_GlobalVariableDefs.obj" "Dsp\src\DSP2803x_PieCtrl.obj" "Dsp\src\DSP2803x_Sci.obj" "Dsp\src\DSP2803x_SysCtrl.obj" "Dsp\src\DSP2803x_usDelay.obj" "DspBootLoad\boot.obj" "DspBootLoad\bootApp.obj" "Public\assertEx.obj" "Public\basic.obj" "Public\crc.obj" "Public\mempool.obj" 
	-$(RM) "main.d" "Can\can\candriver.d" "Can\can\canmsgblock.d" "Can\candsp\candsp.d" "Dsp\src\DSP2803x_CpuTimers.d" "Dsp\src\DSP2803x_GlobalVariableDefs.d" "Dsp\src\DSP2803x_PieCtrl.d" "Dsp\src\DSP2803x_Sci.d" "Dsp\src\DSP2803x_SysCtrl.d" "DspBootLoad\boot.d" "DspBootLoad\bootApp.d" "Public\assertEx.d" "Public\basic.d" "Public\crc.d" "Public\mempool.d" 
	-$(RM) "Dsp\src\DSP2803x_CSMPasswords.d" "Dsp\src\DSP2803x_CodeStartBranch.d" "Dsp\src\DSP2803x_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

