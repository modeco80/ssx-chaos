NAME = ssx_og_chaos_$(REGION)
TYPE := elf

# Effect files
EFFECTS := \
	effects/no_stats.cpp

# Runtime sources (startup code, and code
# needed to link the ml libs properly)
RUNTIME := \
	crt0.S \
	runtime/ml_memory.cpp \
	runtime/ml_string.cpp \
	runtime/ml_abort.cpp

UTILS := \
	utils/random.cpp \
	utils/hook/trampoline.cpp

SOURCES := \
	$(RUNTIME) \
	$(UTILS) \
	game_hooks.cpp \
	chaos_core.cpp \
	$(EFFECTS) \
	vote.cpp \
	main.cpp

LINKSCRIPT = $(OBJDIR)/chaos_blob.ld
LINKLIBS := \
	../ml/core/obj/libml3_core.a \
	../ml/cxx/obj/libml3_cxx.a \
	../ml/hw/obj/libml3_hw.a

# Set user C flags:
# Tell some headers which are shared between the IRC agent
# and the blob that they are being used inside the blob,
# so they declare things properly.
USER_CFLAGS = -DIN_CHAOS_BLOB -DCHAOS_REGION_$(REGION) \
	-I bxapi/include

# Pre deps
EXTRATARGETS_PRE = $(LINKSCRIPT)
EXTRATARGETS_POST = $(GAME_$(GAME)_$(REGION)_PNACH)

include $(TOP)/mk/makedef.mk

# Preprocess the linker script to include game symbols here.
$(OBJDIR)/chaos_blob.ld: $(OBJDIR)/ chaos_blob.ld
	$(info Preprocessing linker script $@)
	sed 's|@REGION@|$(REGION)|' chaos_blob.ld > $@

$(GAME_$(GAME)_$(REGION)_PNACH): bin/$(NAME).elf
	$(info Making PNACH $@)
	./mkpnach.py $@ $(REGION)

