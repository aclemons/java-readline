#
#

BUILD=../build

all : java native

java: always $(BUILD)
	$(JAVAC) $(JC_FLAGS) -d $(BUILD) `find . -name "*.java"`

native: always
	cd native ; $(MAKE) T_LIBS="$(T_LIBS)" JAVAINCLUDE="$(JAVAINCLUDE)" \
                            OS_FLAVOR=$(OS_FLAVOR) JAVANATINC="$(JAVANATINC)" all

$(BUILD):
	mkdir $@

always:
