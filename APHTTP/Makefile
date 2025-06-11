CXX = g++
CXXFLAGS = -std=c++11 -Wall -pedantic -I../src

BUILD_DIR = build
TEMPLATE_DIR = .template
OUT_EXE = myserver.out

# HW6 Source and Object files
HW6_SRC_FILES = \
    ../src/Controller.cpp \
    ../src/DateTime.cpp \
    ../src/Event.cpp \
    ../src/HolidayManager.cpp \
    ../src/JointEvent.cpp \
    ../src/Model.cpp \
    ../src/NormalEvent.cpp \
    ../src/PeriodicEvent.cpp \
    ../src/Task.cpp \
    ../src/User.cpp \
    ../src/UserManager.cpp

HW6_OBJS = $(patsubst ../src/%.cpp,$(BUILD_DIR)/%.o,$(HW6_SRC_FILES))

ifeq ($(OS),Windows_NT)
	LDLIBS += -l Ws2_32
endif

all: $(BUILD_DIR) $(OUT_EXE)

$(OUT_EXE): $(BUILD_DIR)/main.o $(BUILD_DIR)/handlers.o $(BUILD_DIR)/auth_handlers.o $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/strutils.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/template_parser.o $(HW6_OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/template_parser.o: utils/template_parser.cpp utils/template_parser.hpp utils/request.cpp utils/request.hpp utils/utilities.hpp utils/utilities.cpp utils/strutils.hpp utils/strutils.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/response.o: utils/response.cpp utils/response.hpp utils/include.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/request.o: utils/request.cpp utils/request.hpp utils/include.hpp utils/utilities.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/utilities.o: utils/utilities.cpp utils/utilities.hpp utils/strutils.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/strutils.o: utils/strutils.cpp utils/strutils.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/server.o: server/server.cpp server/server.hpp server/route.hpp utils/utilities.hpp utils/strutils.hpp utils/response.hpp utils/request.hpp utils/include.hpp utils/template_parser.hpp utils/template_parser.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/route.o: server/route.cpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/handlers.o: examples/handlers.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: examples/main.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp handlers/auth_handlers.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/auth_handlers.o: handlers/auth_handlers.cpp handlers/auth_handlers.hpp ../src/Controller.hpp ../src/Model.hpp server/server.hpp
	$(CXX) $(CXXFLAGS) -c handlers/auth_handlers.cpp -o $(BUILD_DIR)/auth_handlers.o

# Rule for HW6 object files
# This generic rule should handle all HW6 .cpp files.
# Dependencies are listed broadly; for more fine-grained dependencies, individual rules might be needed.
$(BUILD_DIR)/%.o: ../src/%.cpp $(wildcard ../src/*.hpp)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean
clean:
	rm -rf $(BUILD_DIR) $(TEMPLATE_DIR) *.o *.out $(OUT_EXE) &> /dev/null
