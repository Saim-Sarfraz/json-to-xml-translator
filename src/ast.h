#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <iostream>

enum class NodeType {
    OBJECT,
    ARRAY,
    KEY_VALUE,
    STRING,
    NUMBER,
    BOOL,
    NULL_VAL
};

class AstNode {
public:
    NodeType type;
    std::string key;
    std::string stringValue;
    double numberValue;
    AstNode* child;
    AstNode* next;

    AstNode(NodeType t) : type(t), numberValue(0), child(nullptr), next(nullptr) {}
    ~AstNode() {
        if (child) delete child;
        if (next) delete next;
    }

    static AstNode* createString(std::string val);
    static AstNode* createNumber(double val);
    static AstNode* createBool(bool val);
    static AstNode* createNull();
    static AstNode* createKeyValuePair(std::string key, AstNode* value);
    static AstNode* createObject(AstNode* members);
    static AstNode* createArray(AstNode* elements);

    void printXml(int indent = 0);
    void printAst(int indent = 0);
};

void escapeXml(const std::string& input);

#endif
