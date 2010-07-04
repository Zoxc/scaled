root=..
include ../Makefile.common

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
	scene/shader-state.cpp \
	widgets/gradient.cpp \
	$(NULL)

OBJECTS = $(patsubst %.cpp, $(CFG)/%.o, $(SOURCES) )

.PHONY: build-dir 
build-dir:
	@mkdir -p $(CFG)
	@mkdir -p $(CFG)/layout
	@mkdir -p $(CFG)/scene
	@mkdir -p $(CFG)/widgets

$(TARGET): $(OBJECTS)
	@$(AR) rsc $@ $(OBJECTS)

.PHONY: clean 
clean:
	@rm -f $(OBJECTS) $(TARGET)

all: build-dir $(TARGET)
