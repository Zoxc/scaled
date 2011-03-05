root=..
default: all

BUILD_DIRS += layout scene widgets scene/fonts

include $(root)/Makefile.common
include $(root)/river/Makefile.shared

TARGET = $(CFG)/libriver.a

SOURCES = \
	gles.cpp \
	color.cpp \
	memory-pool.cpp \
	layout/block.cpp \
	layout/element.cpp \
	layout/extends.cpp \
	layout/background.cpp \
	layout/flow.cpp \
	layout/stack.cpp \
	layout/window.cpp \
	scene/fonts/font.cpp \
	scene/fonts/font-size.cpp \
	scene/fonts/glyph.cpp \
	scene/fonts/glyph-canvas.cpp \
	scene/fonts/glyph-state.cpp \
	scene/gradient-state.cpp \
	scene/gradient-canvas.cpp \
	scene/colored-image-canvas.cpp \
	scene/layer.cpp \
	scene/layer-canvas.cpp \
	scene/buffer.cpp \
	scene/scene.cpp \
	scene/state.cpp \
	scene/shader-state.cpp \
	scene/content-serializer.cpp \
	image.cpp \
	widgets/gradient.cpp \
	widgets/label.cpp \
	$(NULL)

OBJECTS = $(patsubst %.cpp, $(CFG)/%.o, $(SOURCES) )

$(TARGET): $(OBJECTS)
	@$(AR) rsc $@ $(OBJECTS)

.PHONY: clean 
clean:
	@rm -f $(OBJECTS) $(TARGET)

all: build-dirs $(TARGET)
