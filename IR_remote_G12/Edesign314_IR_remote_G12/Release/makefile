################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf *.lst *.lis *.lpp *.map

# All of the sources participating in the build are defined here
-include sources.mk
-include src/system/subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_UPPER_DEPS)),)
-include $(C++_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
-include $(SRC_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_UPPER_DEPS)),)
-include $(CPP_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CC_UPPER_DEPS)),)
-include $(CC_UPPER_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_UPPER_DEPS)),)
-include $(CXX_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
libEdesign314_IR_remote_G12.a \

LINKER_OUTPUT_OUTPUTS += \
Edesign314_IR_remote_G12.x \


# All Target
# Main-build Target
all: Edesign314_IR_remote_G12.mot
	@echo 'Build complete.'

# Tool invocations
Edesign314_IR_remote_G12.mot: $(LINKER_OUTPUT_OUTPUTS)
	@echo 'Invoking: Objcopy'
	rl78-elf-objcopy -O srec  $(LINKER_OUTPUT_OUTPUTS)"Edesign314_IR_remote_G12.mot"
	@echo 'Finished building target:'
	@echo.

libEdesign314_IR_remote_G12.a: 
	@echo 'Invoking: Library Generator'
	rl78-elf-libgen --select-lib=optlib --header-files=all --compiler-options=-Os,-mmul=none,-ffunction-sections,-fdata-sections,-fno-function-cse,-falign-jumps,-funit-at-a-time -o "libEdesign314_IR_remote_G12.a"
	@echo 'Finished building:'
	@echo.

Edesign314_IR_remote_G12.x: $(OBJS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(ALL_ASMS)
	@echo 'Invoking: Linker'
	@echo 'Building target:'
	rl78-elf-ld -o "Edesign314_IR_remote_G12.x" -T"C:\e2_studio\Edesign314_2016\IR_remote_G12\Edesign314_IR_remote_G12\Release\Edesign314_IR_remote_G12_Release_auto.gsi" -T"C:\e2_studio\Edesign314_2016\IR_remote_G12\Edesign314_IR_remote_G12\Release\LinkerSubCommand.tmp" $(USER_OBJS) $(LIBS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) -M=Edesign314_IR_remote_G12.map -e_PowerON_Reset -no-keep-memory -S -L"C:\e2_studio\Edesign314_2016\IR_remote_G12\Edesign314_IR_remote_G12\Release" -L"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-ELF/lib/gcc/rl78-elf/4.9-GNURL78_v15.02" --start-group -lEdesign314_IR_remote_G12 -lgcc --end-group
	@echo 'Finished building:'
	@echo.

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS)$(OBJCOPY_OUTPUTTYPE_OUTPUTS)$(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)
	rm -rf ./"src/system"/*.o ./"src/system"/*.d 
	rm -rf ./"src"/*.o ./"src"/*.d  Edesign314_IR_remote_G12.mot
	@echo.
	@echo 'Clean complete.'

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
