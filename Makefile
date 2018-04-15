# Makefile for Stocks

#!/bin/bash

#Compiler and Linker
CXX = g++

#The Target Binary Program
TARGET      := oddSim

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := 
TYPEDIR		:= 
BUILDDIR    := obj
LIBDIR    	:= lib
RESDIR      := res
SRCEXT      := cpp
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
CFLAGS      := -std=c++11 -Wall -Wextra -g
LIB         := -lboost_system -lcrypto -lssl -lcpprest -lpthread -lutil -lboost_iostreams -lboost_filesystem
INC         := #-I./$(INCDIR) -I./$(TYPEDIR)
INCDEP      := #-I./$(INCDIR) -I./$(TYPEDIR)

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Defauilt Make
all: $(TARGET)
	@echo DONE
	
#Remake
remake: cleaner all

#Make the Directories
directories:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(LIBDIR)

#Clean only Objects
clean:
	@$(RM) -rf $(BUILDDIR)
	@$(RM) -rf $(LIBDIR)
	@$(RM) -rf $(TARGET)
	@echo Object, library, and executable files deleted
	
#Clean only Objects
cleanobj:
	@$(RM) -rf $(BUILDDIR)
	@echo Object files deleted
	
#Clean only Libs
cleanlib:
	@$(RM) -rf $(LIBDIR)
	@echo Lirary files deleted

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $^ $(LIB)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CXX) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

#Non-File Targets
.PHONY: all remake clean cleaner

