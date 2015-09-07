# The MIT License (MIT)

# Copyright (c) 2015 nabijaczleweli

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

include configMakefile

.PHONY : clean all dll test $(BUILD)/cpponfig_version$(OBJ)

all : dll test

clean :
	rm -rf $(BUILD) $(TEST)/*$(EXE)

dll : $(BUILD)/configuration$(OBJ) $(BUILD)/configuration_category$(OBJ) $(BUILD)/property$(OBJ) $(BUILD)/util/salt$(OBJ) $(BUILD)/cpponfig_version$(OBJ)
	$(CXX) $(CPPAR) -shared $(PIC) -o$(BUILD)/$(PREDLL)cpponfig$(DLL) $^

test : $(TEST)/test$(EXE)
	@cp $(BUILD)/$(PREDLL)cpponfig$(DLL) $(TEST)
	$^
	@rm -rf $(TEST)/$(PREDLL)cpponfig$(DLL)


$(BUILD)/%$(OBJ) : $(SOURCE)/%.cpp
	@mkdir $(dir $@) 2>$(nul) | $(nop)
	$(CXX) $(CPPAR) -c -o$@ $^

$(BUILD)/cpponfig_version$(OBJ) :
	@mkdir $(dir $@) 2>$(nul) | $(nop)
	@$(ECHO) -e "#include <string>\nextern const std::string cpponfiguration_version;const std::string cpponfiguration_version(__DATE__ \" \" __TIME__);" | tee asdfasdfasdf | $(CXX) $(CPPAR) -x c++ -c -o$@ -

%$(EXE) : %.cpp
	$(CXX) $(CPPAR) -I$(SOURCE) -Lout -lcpponfig -o$@ $^
