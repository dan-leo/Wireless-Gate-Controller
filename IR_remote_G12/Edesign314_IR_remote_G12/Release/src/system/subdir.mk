################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\src/system/core.c \
..\src/system/infrared.c 

OBJS += \
./src/system/core.o \
./src/system/infrared.o 

C_DEPS += \
./src/system/core.d \
./src/system/infrared.d 


# Each subdirectory must supply rules for building sources it contributes
src/system/%.o: ../src/system/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@rl78-elf-gcc -MM -MP -MF ""$(@:%.o=%.d)"" -MT"$(@:%.o=%.o)" -MT""$(@:%.o=%.d)"" @"src/system/c.depsub" "$<"
	@echo	rl78-elf-gcc -MM -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)"  -nostdinc -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-ELF/rl78-elf/optlibinc" -Os -fno-function-cse -funit-at-a-time -falign-jumps -fdata-sections -ffunction-sections -mmul=none -x c "$<"
	@rl78-elf-gcc @"src/system/c.sub" -c -x c -o "$(@:%.d=%.o)" "$<"
	@echo rl78-elf-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-ELF/rl78-elf/optlibinc" -Os -fno-function-cse -funit-at-a-time -falign-jumps -fdata-sections -ffunction-sections -mmul=none -c -x c -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

