root=..
default: all

include $(root)/swl/Makefile.public
include $(root)/png/Makefile.public
include $(root)/freetype/Makefile.public
include $(root)/Makefile.common

TARGET = $(CFG)/blending

SOURCES = \
	main.cpp \
	gles.cpp \
	$(NULL)

OBJECTS = $(patsubst %.cpp, $(CFG)/%.o, $(SOURCES) )

$(TARGET): swl $(OBJECTS)
	@$(CXX) $(CFLAGS) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

all: build-dirs $(TARGET)

.PHONY: clean 
clean: swl-clean
	@rm -f $(OBJECTS) $(TARGET)
