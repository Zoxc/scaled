LIBS = swl river libEGL libGLESv2

include Makefile.common

TARGET = $(CFG)/scaled

.PHONY: river 
river:
	$(MAKE) -C river

.PHONY: swl 
swl:
	$(MAKE) -C swl

.PHONY: build-dir 
build-dir:
	@mkdir -p $(CFG)

SOURCES = \
	main.cpp \
	$(NULL)

OBJECTS = $(patsubst %.cpp, $(CFG)/%.o, $(SOURCES) )

$(TARGET): build-dir river swl $(OBJECTS)
	@$(CXX) $(CFLAGS) $(CXXFLAGS) -Lswl/$(CFG) -Lriver/$(CFG) $(OBJECTS) $(LDFLAGS) -o $@

all: $(TARGET)

.PHONY: river-clean
river-clean:
	$(MAKE) -C river clean

.PHONY: swl-clean
swl-clean:
	$(MAKE) -C swl clean

.PHONY: clean 
clean: river-clean swl-clean
	@rm -f $(OBJECTS) $(TARGET)
