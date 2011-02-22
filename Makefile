root=.
default: all

include $(root)/river/Makefile.public
include $(root)/Makefile.common

TARGET = $(CFG)/scaled

SOURCES = \
	main.cpp \
	window-state.cpp \
	$(NULL)

OBJECTS = $(patsubst %.cpp, $(CFG)/%.o, $(SOURCES) )

$(TARGET): river swl $(OBJECTS)
	@$(CXX) $(CFLAGS) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

all: build-dirs $(TARGET)

.PHONY: clean 
clean: river-clean swl-clean
	@rm -f $(OBJECTS) $(TARGET)
