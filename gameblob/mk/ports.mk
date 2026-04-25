GAME := ssxog
REGION ?= usa

# US version
GAME_ssxog_usa_VALID = yes
# This is incorrect, but I really don't feel like rebuilding the
# 2.9-ee-991101 version that the game *actually* used, and it's still
# somewhat ABI compatible, so whatever
# If you want to rebuild it and make it correct, go right ahead
GAME_ssxog_usa_GCC_VERSION = 2.96-ee-001003-1
GAME_ssxog_usa_PNACH = 085653F4.chaos_mod.pnach

# sanity check
ifneq ($(GAME_$(GAME)_$(REGION)_VALID),yes)
$(error The game/version combination $(GAME)/$(REGION) is currently not supported by the SSX Chaos Mod. Please pick a different one)
endif
