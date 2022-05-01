#
#

BUILD=../build

all : java native

java: always $(BUILD)
ifneq ($(USE_JAVAH),true)
	$(JAVAC) $(JC_FLAGS) -h ./native -d $(BUILD) `find . -name "*.java"`
else
	$(JAVAC) $(JC_FLAGS) -d $(BUILD) `find . -name "*.java"`
endif

native: always
	cd native ; $(MAKE) T_LIBS="$(T_LIBS)" JAVAINCLUDE="$(JAVAINCLUDE)" \
                            OS_FLAVOR=$(OS_FLAVOR) JAVANATINC="$(JAVANATINC)" all

$(BUILD):
	mkdir $@

always:
