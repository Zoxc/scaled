root=..
default: all

BUILD_DIRS += layout scene widgets

include $(root)/Makefile.common

TARGET = $(CFG)/libriver.a

SOURCES = \
	gles.cpp \
	layout/block.cpp \
	layout/element.cpp \
	layout/extends.cpp \
	layout/flow.cpp \
	layout/window.cpp \
	scene/gradient-object.cpp \
	scene/gradient-state.cpp \
	scene/layer.cpp \
	scene/object.cpp \
	scene/object-list.cpp \
	scene/scene.cpp \
	scene/state.cpp \
	scene/shader-state.cpp \
	widgets/gradient.cpp \
	$(NULL)

OBJECTS = $(patsubst %.cpp, $(CFG)/%.o, $(SOURCES) )

$(TARGET): $(OBJECTS)
	@$(AR) rsc $@ $(OBJECTS)

.PHONY: clean 
clean:
	@rm -f $(OBJECTS) $(TARGET)

all: build-dirs $(TARGET)
