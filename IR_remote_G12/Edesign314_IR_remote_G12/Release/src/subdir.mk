################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
..\src/r_reset_program.asm 

C_SRCS += \
..\src/r_cg_cgc.c \
..\src/r_cg_cgc_user.c \
..\src/r_cg_intc.c \
..\src/r_cg_intc_user.c \
..\src/r_cg_port.c \
..\src/r_cg_port_user.c \
..\src/r_cg_timer.c \
..\src/r_cg_timer_user.c \
..\src/r_cg_vector_table.c \
..\src/r_hardware_setup.c \
..\src/r_main.c 

OBJS += \
./src/r_cg_cgc.o \
./src/r_cg_cgc_user.o \
./src/r_cg_intc.o \
./src/r_cg_intc_user.o \
./src/r_cg_port.o \
./src/r_cg_port_user.o \
./src/r_cg_timer.o \
./src/r_cg_timer_user.o \
./src/r_cg_vector_table.o \
./src/r_hardware_setup.o \
./src/r_main.o \
./src/r_reset_program.o 

C_DEPS += \
./src/r_cg_cgc.d \
./src/r_cg_cgc_user.d \
./src/r_cg_intc.d \
./src/r_cg_intc_user.d \
./src/r_cg_port.d \
./src/r_cg_port_user.d \
./src/r_cg_timer.d \
./src/r_cg_timer_user.d \
./src/r_cg_vector_table.d \
./src/r_hardware_setup.d \
./src/r_main.d 

ASM_DEPS += \
./src/r_reset_program.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@rl78-elf-gcc -MM -MP -MF ""$(@:%.o=%.d)"" -MT"$(@:%.o=%.o)" -MT""$(@:%.o=%.d)"" @"src/c.depsub" "$<"
	@echo	rl78-elf-gcc -MM -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)"  -nostdinc -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-ELF/rl78-elf/optlibinc" -Os -fno-function-cse -funit-at-a-time -falign-jumps -fdata-sections -ffunction-sections -mmul=none -x c "$<"
	@rl78-elf-gcc @"src/c.sub" -c -x c -o "$(@:%.d=%.o)" "$<"
	@echo rl78-elf-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-ELF/rl78-elf/optlibinc" -Os -fno-function-cse -funit-at-a-time -falign-jumps -fdata-sections -ffunction-sections -mmul=none -c -x c -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

src/%.o: ../src/%.asm
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@rl78-elf-gcc -MM -MP -MF ""$(@:%.o=%.d)"" -MT"$(@:%.o=%.o)" -MT""$(@:%.o=%.d)"" @"src/asm.depsub" -o"$@" "$<"
	@echo	rl78-elf-gcc -MM -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)"  -nostdinc -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-ELF/rl78-elf/optlibinc" -Os -fno-function-cse -funit-at-a-time -falign-jumps -fdata-sections -ffunction-sections -mmul=none -Wa,-gdwarf2 -x assembler-with-cpp -o"$@" "$<"
	@rl78-elf-gcc @"src/asm.sub" -c -x assembler-with-cpp  -o "$(@:%.d=%.o)" "$<"
	@echo rl78-elf-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-ELF/rl78-elf/optlibinc" -Os -fno-function-cse -funit-at-a-time -falign-jumps -fdata-sections -ffunction-sections -mmul=none -Wa,-gdwarf2 -c -x assembler-with-cpp  -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

