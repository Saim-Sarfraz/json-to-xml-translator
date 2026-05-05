CXX = g++
CXXFLAGS = -Wall -g
LEX = flex
YACC = bison

SRC_DIR = src
TARGET = json2xml

all: $(TARGET)

$(TARGET): $(SRC_DIR)/parser.tab.c $(SRC_DIR)/lex.yy.c $(SRC_DIR)/ast.c
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC_DIR)/parser.tab.c $(SRC_DIR)/lex.yy.c $(SRC_DIR)/ast.c

$(SRC_DIR)/parser.tab.c $(SRC_DIR)/parser.tab.h: $(SRC_DIR)/parser.y
	$(YACC) -d -o $(SRC_DIR)/parser.tab.c --defines=$(SRC_DIR)/parser.tab.h $(SRC_DIR)/parser.y

$(SRC_DIR)/lex.yy.c: $(SRC_DIR)/scanner.l $(SRC_DIR)/parser.tab.h
	$(LEX) -o $(SRC_DIR)/lex.yy.c $(SRC_DIR)/scanner.l

clean:
	rm -f $(TARGET) $(SRC_DIR)/parser.tab.c $(SRC_DIR)/parser.tab.h $(SRC_DIR)/lex.yy.c
