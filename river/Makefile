root=..
default: all

BUILD_DIRS += layout scene widgets scene/fonts

include $(root)/Makefile.common
include $(root)/river/Makefile.shared

TARGET = $(CFG)/libriver.a

SOURCES = \
	gles.cpp \
	color.cpp \
	layout/block.cpp \
	layout/element.cpp \
	layout/extends.cpp \
	layout/flow.cpp \
	layout/window.cpp \
	scene/fonts/font.cpp \
	scene/fonts/font-size.cpp \
	scene/fonts/glyph.cpp \
	scene/fonts/glyph-cache.cpp \
	scene/fonts/glyph-object.cpp \
	scene/fonts/glyph-state.cpp \
	scene/fonts/text-object.cpp \
	scene/gradient-object.cpp \
	scene/gradient-state.cpp \
	scene/layer.cpp \
	scene/object.cpp \
	scene/rectangle.cpp \
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
