OUT_DIR			= ./output
SRC 			= ./src
LINK_TARGET 	= ./create_csv
SRC_FILES 		= $(shell find $(SRC)/*.cpp)
OBJS 			= $(patsubst $(SRC)/%.cpp, $(OUT_DIR)/%.o, $(SRC_FILES))
REBUILDABLES	= $(OBJS) $(LINK_TARGET)
CC_FLAGS		= -std=c++17 -Wall -g
CC				= clang++

all: $(LINK_TARGET)
	@echo "compilation success ✅"

$(LINK_TARGET): $(OBJS)
	$(CC) $(CC_FLAGS) -o $@ $^ -luuid

$(OUT_DIR)/%.o: $(SRC)/%.cpp
	$(CC) $(CC_FLAGS) -o $@ -c $<

clean:
	rm -f $(OUT_DIR)/*
	rm $(LINK_TARGET)
	@echo $(LINK_TARGET)
	@echo "clean done ✨"

run:
	./output/create_csv
