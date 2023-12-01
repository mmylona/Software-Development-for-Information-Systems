CXX = g++
CXXFLAGS = -std=c++11 -fPIC
LIBS = -lgtest -lgtest_main -pthread
INCLUDES = -Iheaders
SRCDIR = src
TESTDIR = tests/unit_tests
OUTDIR = bin
MKDIR_P = mkdir -p
SCRIPTS_DIR = scripts
INSTALL_GPP_SCRIPT = $(SCRIPTS_DIR)/gplusplus_install.sh
INSTALL_GTEST_SCRIPT = $(SCRIPTS_DIR)/install_gtest.sh


# Source files for the library
LIB_SRCS = $(SRCDIR)/structures/point/Point.cpp \
           $(SRCDIR)/structures/point/PointInfo.cpp \
           $(SRCDIR)/structures/point/Neighbors.cpp \
           $(SRCDIR)/structures/Dataset.cpp \
           $(SRCDIR)/structures/graph/GraphInitialization.cpp \
           $(SRCDIR)/structures/graph/Optimizations.cpp \
           $(SRCDIR)/utils/Metrics.cpp \
           $(SRCDIR)/utils/Statistics.cpp \
           $(SRCDIR)/utils/StatisticInfo.cpp \
           $(SRCDIR)/config/Logger.cpp

# Shared library (SO) and Static library (A) names
LIB_NAME = dataforge
LIB_SHARED = $(OUTDIR)/lib$(LIB_NAME).so
LIB_STATIC = $(OUTDIR)/lib$(LIB_NAME).a

# Source files for unit tests
TEST_SRCS = $(TESTDIR)/structures/point/PointTest.cpp \
            $(TESTDIR)/structures/graph/GraphInitializationTest.cpp \
            $(TESTDIR)/structures/dataset/DatasetTest.cpp \
            $(TESTDIR)/structures/vector/VectorTestFloat.cpp \
            $(TESTDIR)/structures/vector/VectorTestNeighbor.cpp \
            $(TESTDIR)/structures/vector/VectorTestPoint.cpp \
            $(TESTDIR)/structures/unorderedMap/UnorderedMapTest.cpp


# Test executables
TESTS = $(patsubst $(TESTDIR)/%.cpp,$(OUTDIR)/%,$(TEST_SRCS))

.PHONY: all clean test

all: install_dependencies  shared_library static_library export_library_path main_executable
install_dependencies:
	@chmod +x $(INSTALL_GPP_SCRIPT)
	@$(SHELL) $(INSTALL_GPP_SCRIPT)
	@chmod +x $(INSTALL_GTEST_SCRIPT)
	@$(SHELL) $(INSTALL_GTEST_SCRIPT)

shared_library: $(LIB_SHARED)

static_library: $(LIB_STATIC)

$(LIB_SHARED): $(LIB_SRCS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(INCLUDES)

$(LIB_STATIC): $(LIB_SRCS)
	ar rcs $@ $^

test: create_outdir $(TESTS)
	@for test in $(TESTS); do \
		./$$test; \
	done

$(OUTDIR)/%: $(TESTDIR)/%.cpp
	$(MKDIR_P) $(OUTDIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIB_SRCS) $(INCLUDES) $(LIBS)

export_library_path:
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(OUTDIR)

main_executable: create_outdir
	$(CXX) -o main $(SRCDIR)/library.cpp -L$(OUTDIR) -Wl,-rpath,'$$ORIGIN/$(OUTDIR)' -ldataforge




clean:
	rm -f $(LIB_SHARED) $(LIB_STATIC) $(TESTS) main

create_outdir:
	@mkdir -p $(OUTDIR)
