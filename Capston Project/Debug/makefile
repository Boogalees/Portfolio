################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./cx2538_setup.obj" \
"./cx2538_startup_ccs.obj" \
"./main.obj" \
"./uartstdio.obj" \
"./SourceFiels/basic_rf.obj" \
"./SourceFiels/hal_int.obj" \
"./SourceFiels/hal_mcu.obj" \
"./SourceFiels/hal_rf.obj" \
"./SourceFiels/hal_timer_32k.obj" \
"./SourceFiels/lcd_dogm128_6_ti_logo.obj" \
"./SourceFiels/per_test_menu.obj" \
"./SourceFiels/util_menu_system.obj" \
"./MAC_Layer/CSMA-CA.obj" \
"./MAC_Layer/Random.obj" \
"./MAC_Layer/mac.obj" \
"./MAC_Layer/mac_assoc.obj" \
"./MAC_Layer/mac_data.obj" \
"./MAC_Layer/mac_gen.obj" \
"./MAC_Layer/mac_parse.obj" \
"./MAC_Layer/mac_poll.obj" \
"./MAC_Layer/mac_reset.obj" \
"./MAC_Layer/mac_scan.obj" \
"./MAC_Layer/sleep.obj" \
"./MAC_Dirver/buf.obj" \
"./MAC_Dirver/list.obj" \
"./MAC_Dirver/mem_heap.obj" \
"./MAC_Dirver/mmem.obj" \
"../bsp.lib" \
"../cx2538xf53_ccs.cmd" \
"../driverlib.lib" \
"../sdriverlib.lib" \
$(GEN_CMDS__FLAG) \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include SourceFiels/subdir_vars.mk
-include MAC_Layer/subdir_vars.mk
-include MAC_Dirver/subdir_vars.mk
-include subdir_rules.mk
-include SourceFiels/subdir_rules.mk
-include MAC_Layer/subdir_rules.mk
-include MAC_Dirver/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
802_15_4_test.out \

EXE_OUTPUTS__QUOTED += \
"802_15_4_test.out" \

BIN_OUTPUTS += \
802_15_4_test.hex \

BIN_OUTPUTS__QUOTED += \
"802_15_4_test.hex" \


# All Target
all: 802_15_4_test.out

# Tool invocations
802_15_4_test.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M3 --code_state=16 --abi=eabi -me -g --define=DEBUG --define=CC2538_USE_ALTERNATE_INTERRUPT_MAP --diag_wrap=off --diag_warning=225 --display_error_number -z -m"802_15_4_test.map" --heap_size=0 --stack_size=256 -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/lib" -i"F:/swrc272a_2/cc2538_per_test_1_1_0/source/apps/per_test" -i"C:/Workspace/CCS_NEW_WorkSpace/lib (3)/swrc271a (1)/cc2538_foundation_firmware_1_0_1_0/bsp/srf06eb_cc2538/drivers/bin/ccs" -i"C:/Workspace/CCS_NEW_WorkSpace/lib (3)/swrc271a (1)/cc2538_foundation_firmware_1_0_1_0/driverlib/cc2538/bin/ccs" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --reread_libs --display_error_number --diag_wrap=off --warn_sections --xml_link_info="802_15_4_test_linkInfo.xml" --rom_model -o "802_15_4_test.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

802_15_4_test.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armhex"  -o "802_15_4_test.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "cx2538_setup.pp" "cx2538_startup_ccs.pp" "main.pp" "uartstdio.pp" "SourceFiels\basic_rf.pp" "SourceFiels\hal_int.pp" "SourceFiels\hal_mcu.pp" "SourceFiels\hal_rf.pp" "SourceFiels\hal_timer_32k.pp" "SourceFiels\lcd_dogm128_6_ti_logo.pp" "SourceFiels\per_test_menu.pp" "SourceFiels\util_menu_system.pp" "MAC_Layer\CSMA-CA.pp" "MAC_Layer\Random.pp" "MAC_Layer\mac.pp" "MAC_Layer\mac_assoc.pp" "MAC_Layer\mac_data.pp" "MAC_Layer\mac_gen.pp" "MAC_Layer\mac_parse.pp" "MAC_Layer\mac_poll.pp" "MAC_Layer\mac_reset.pp" "MAC_Layer\mac_scan.pp" "MAC_Layer\sleep.pp" "MAC_Dirver\buf.pp" "MAC_Dirver\list.pp" "MAC_Dirver\mem_heap.pp" "MAC_Dirver\mmem.pp" 
	-$(RM) "cx2538_setup.obj" "cx2538_startup_ccs.obj" "main.obj" "uartstdio.obj" "SourceFiels\basic_rf.obj" "SourceFiels\hal_int.obj" "SourceFiels\hal_mcu.obj" "SourceFiels\hal_rf.obj" "SourceFiels\hal_timer_32k.obj" "SourceFiels\lcd_dogm128_6_ti_logo.obj" "SourceFiels\per_test_menu.obj" "SourceFiels\util_menu_system.obj" "MAC_Layer\CSMA-CA.obj" "MAC_Layer\Random.obj" "MAC_Layer\mac.obj" "MAC_Layer\mac_assoc.obj" "MAC_Layer\mac_data.obj" "MAC_Layer\mac_gen.obj" "MAC_Layer\mac_parse.obj" "MAC_Layer\mac_poll.obj" "MAC_Layer\mac_reset.obj" "MAC_Layer\mac_scan.obj" "MAC_Layer\sleep.obj" "MAC_Dirver\buf.obj" "MAC_Dirver\list.obj" "MAC_Dirver\mem_heap.obj" "MAC_Dirver\mmem.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

