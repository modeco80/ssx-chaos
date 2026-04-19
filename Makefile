

.PHONY: all clean

# This contains all the things needed to build a subproject in the tree
SUBMAKE = $(MAKE) V=$(V)

define submake
$(info Making project $(1))
$(SUBMAKE) -C $(1) $(2)
endef

define makeall
$(call submake,gameblob/,$(1))
endef

# Forwarders
all:
	$(call makeall,)
clean:
	$(call makeall,clean)

$V.SILENT:
