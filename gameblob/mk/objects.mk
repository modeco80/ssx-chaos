# This piece builds the object list from the target's provided sources
OBJS_INTERMEDIATE := $(addprefix $(OBJDIR)/,$(SOURCES))
OBJS_INTERMEDIATE := $(OBJS_INTERMEDIATE:.S=.o)
OBJS_INTERMEDIATE := $(OBJS_INTERMEDIATE:.c=.o)
OBJS_INTERMEDIATE := $(OBJS_INTERMEDIATE:.cpp=.o)
OBJECTS := $(OBJS_INTERMEDIATE)
