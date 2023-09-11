#===================================================================================================
#
#												VARS
#
#===================================================================================================

BUILD=$(PATH_MAKE)/../build
REPORT = $(PATH_MAKE)/../report
DOCUMENTATION = $(PATH_MAKE)/../documentation
DIST = $(PATH_MAKE)/../dist
TARGET_INSTALL=~
PATH_MAKE=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))

PRE_MAKE=make -C $(BUILD)

ifeq ($(OS), Darwin)
	OPEN = open
else
	OPEN = xdg-open
endif

#===================================================================================================
#
#											MAIN TARGETS
#
#===================================================================================================

all: tests install

install: pre_cmake
	@cp $(BUILD)/MLP $(TARGET_INSTALL)

uninstall:
	@-rm $(TARGET_INSTALL)/MLP

clean:
	@-rm -rf $(BUILD)

dvi:
	@doxygen $(PATH_MAKE)/docs/Doxyfile
	@cp -r $(PATH_MAKE)/images $(PATH_MAKE)/../misc $(DOCUMENTATION)
	@-$(OPEN) $(DOCUMENTATION)/index.html

dist:
	@mkdir -p $(DIST)
	@tar -cvzf $(DIST)/source_code.tar.gz .
	@echo "The archive was successfully created on the path '$(DIST)'"

tests: pre_cmake
	@cp -r $(PATH_MAKE)/tests/sample $(BUILD)
	@$(PRE_MAKE) test

#===================================================================================================
#
#												OTHER
#
#===================================================================================================

pre_cmake:
	cmake -B $(BUILD) && make -C $(BUILD) -j `nproc`;

style:
	@clang-format -n -style=google `find $(PATH_MAKE) -name *.cc -or -name *.h -or -name *.cpp -or -name *.c`

#===================================================================================================
#
#												END
#
#===================================================================================================
