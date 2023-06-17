# Define the compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -std=c++17 -g #-Werror
INCLUDE_DIRS := -Icore/include/public -Icrypto/include/private -Icrypto/include/public
LDLIBS = -lcryptopp
DEPFLAGS = -MMD -MT $@

# Define the directories
SRCDIR := crypto
OBJDIR := objects

# Define the sources, objects, and dependencies
SRCFILES := $(shell find $(SRCDIR) -name "*.cpp")
OBJFILES := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCFILES))
DEPFILES := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.d,$(SRCFILES))

# Define the main target
TARGET := out

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(DEPFLAGS) -c $< -o $@

-include $(OBJFILES:.o=.d)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
