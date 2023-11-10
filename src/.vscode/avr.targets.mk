-include .vscode/avr.properties.mk

ifeq ($(AVR.source.compiler),$(notdir $(AVR.source.compiler)))
A.compiler.dir :=
else
A.compiler.dir := $(dir $(AVR.source.compiler))
endif
A.output.dir := ../bin
A.obj.dir := $(A.output.dir)/obj_app
A.libraries := $(addprefix -I,$(wildcard $(AVR.source.libraries:=/.) $(AVR.source.libraries:=/*/.) $(AVR.source.libraries:=/*/*/.)))
A.app.hex := $(A.output.dir)/square-wave-gen.hex
A.app.elf := $(A.output.dir)/square-wave-gen.elf
A.app.list := $(A.output.dir)/app.lst

# Put your board type here. "SWG_UNO" and "SWG_MEGA" are currently supported.
A.app.board := SWG_UNO

E.compiler := $(AVR.source.compiler) -DF_CPU=$(AVR.device.frequency) -mmcu=$(AVR.device.type) -D$(A.app.board) -pipe $(A.libraries)
E.get.dep := $(E.compiler) -MM
E.compile := $(E.compiler) -std=gnu99 -g -O2 -Wall -Wstrict-prototypes -c -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections
E.link := $(E.compiler) -Wall -g -fuse-linker-plugin -Wl,--gc-sections -lm
E.make.hex := $(A.compiler.dir)avr-objcopy -O ihex $(A.app.elf) $(A.app.hex)
E.make.list := $(A.compiler.dir)avr-objdump --disassemble --source --line-numbers --demangle
E.get.size := $(A.compiler.dir)avr-size -A

F.dep.base = $(basename $1 $(filter %.h %.hpp,$(shell $(E.get.dep) $1)))
F.dep.1lvl = $(sort $(wildcard $(addsuffix .c,$(call F.dep.base,$1)) $(addsuffix .cc,$(call F.dep.base,$1)) $(addsuffix .cpp,$(call F.dep.base,$1)) $(addsuffix .c++,$(call F.dep.base,$1)) $(addsuffix .C,$(call F.dep.base,$1))))
F.dep = $(if $(filter-out $1,$(call F.dep.1lvl,$1)),$(call F.dep,$(call F.dep.1lvl,$1)),$1)

A.src := $(call F.dep,$(sort $(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c) $(sort $(wildcard *.cc) $(wildcard */*.cc) $(wildcard */*/*.cc) $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp) $(wildcard *.c++) $(wildcard */*.c++) $(wildcard */*/*.c++) $(wildcard *.C) $(wildcard */*.C) $(wildcard */*/*.C))))
A.obj := $(addprefix $(A.obj.dir)/,$(addsuffix .o,$(A.src)))

.PHONY : clean build scan

build : $(A.app.list)
	@$(E.get.size) $(A.app.elf)
	@$(E.make.hex)

clean :
	@$(RM) -r $(A.output.dir)

scan :
	$(info Found source files)
	$(info )
	$(foreach s,$(A.src),$(info $(realpath $(s))))
	$(info )

-include $(A.obj:.o=.d)

$(A.app.list) : $(A.app.elf)
	@mkdir -p $(@D)
	$(info ===== Making $@)
	@$(E.make.list) $^ > $@
	$(info )

$(A.app.elf) : $(A.obj)
	@mkdir -p $(@D)
	$(info ===== Making $@)
	@$(E.link) $^ -o $@
	$(info )

$(A.obj.dir)/%.c.o : %.c
	@mkdir -p $(@D)
	$(info ===== Making $@)
	@$(E.compile) $< -o $@
	$(info )

$(A.obj.dir)/%.C.o : %.C
	@mkdir -p $(@D)
	$(info ===== Making $@)
	@$(E.compile) $< -o $@
	$(info )
