################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
_LED/%.obj: ../_LED/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccs1250/ccs/ccs_base/msp430/include" --include_path="C:/Projetos/projeto_SMP" --include_path="C:/Projetos/projeto_SMP/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power="none" --define=DEPRECATED --define=__MSP430F67641A__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="_LED/$(basename $(<F)).d_raw" --obj_directory="_LED" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


