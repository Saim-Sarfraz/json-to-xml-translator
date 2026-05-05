#include "ast.h"
#include <iostream>
#include <iomanip>

std::string unescape(const std::string& input) {
    std::string result;
    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == '\\' && i + 1 < input.length()) {
            switch (input[++i]) {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case 'r': result += '\r'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'u': {
                    if (i + 4 < input.length()) {
                        std::string hex = input.substr(i + 1, 4);
                        int code = std::stoi(hex, nullptr, 16);
                        i += 4;
                        // Basic UTF-8 encoding for Unicode point
                        if (code < 0x80) {
                            result += (char)code;
                        } else if (code < 0x800) {
                            result += (char)(0xC0 | (code >> 6));
                            result += (char)(0x80 | (code & 0x3F));
                        } else {
                            result += (char)(0xE0 | (code >> 12));
                            result += (char)(0x80 | ((code >> 6) & 0x3F));
                            result += (char)(0x80 | (code & 0x3F));
                        }
                    }
                    break;
                }
                default: result += '\\'; result += input[i]; break;
            }
        } else {
            result += input[i];
        }
    }
    return result;
}

AstNode* AstNode::createString(std::string val) {
    AstNode* node = new AstNode(NodeType::STRING);
    node->stringValue = unescape(val);
    return node;
}

AstNode* AstNode::createNumber(double val) {
    AstNode* node = new AstNode(NodeType::NUMBER);
    node->numberValue = val;
    return node;
}

AstNode* AstNode::createBool(bool val) {
    AstNode* node = new AstNode(NodeType::BOOL);
    node->stringValue = val ? "true" : "false";
    return node;
}

AstNode* AstNode::createNull() {
    return new AstNode(NodeType::NULL_VAL);
}

AstNode* AstNode::createKeyValuePair(std::string key, AstNode* value) {
    AstNode* node = new AstNode(NodeType::KEY_VALUE);
    node->key = key;
    node->child = value;
    return node;
}

AstNode* AstNode::createObject(AstNode* members) {
    AstNode* node = new AstNode(NodeType::OBJECT);
    node->child = members;
    return node;
}

AstNode* AstNode::createArray(AstNode* elements) {
    AstNode* node = new AstNode(NodeType::ARRAY);
    node->child = elements;
    return node;
}

void escapeXml(const std::string& input) {
    for (char c : input) {
        switch (c) {
            case '&': std::cout << "&amp;"; break;
            case '<': std::cout << "&lt;"; break;
            case '>': std::cout << "&gt;"; break;
            case '\"': std::cout << "&quot;"; break;
            case '\'': std::cout << "&apos;"; break;
            default: std::cout << c; break;
        }
    }
}

void printIndent(int indent) {
    for (int i = 0; i < indent; i++) std::cout << "  ";
}

void printXmlInternal(AstNode* node, int indent) {
    if (!node) return;

    switch (node->type) {
        case NodeType::OBJECT: {
            AstNode* curr = node->child;
            while (curr) {
                printXmlInternal(curr, indent);
                curr = curr->next;
            }
            break;
        }
        case NodeType::ARRAY: {
            AstNode* curr = node->child;
            while (curr) {
                printIndent(indent);
                std::cout << "<item>";
                if (curr->type == NodeType::OBJECT || curr->type == NodeType::ARRAY) {
                    std::cout << "\n";
                    printXmlInternal(curr, indent + 1);
                    printIndent(indent);
                } else {
                    printXmlInternal(curr, -1); // -1 means don't print newline or indent
                }
                std::cout << "</item>\n";
                curr = curr->next;
            }
            break;
        }
        case NodeType::KEY_VALUE: {
            printIndent(indent);
            if (node->child && node->child->type == NodeType::NULL_VAL) {
                std::cout << "<" << node->key << "/>\n";
            } else {
                std::cout << "<" << node->key << ">";
                if (node->child && (node->child->type == NodeType::OBJECT || node->child->type == NodeType::ARRAY)) {
                    std::cout << "\n";
                    printXmlInternal(node->child, indent + 1);
                    printIndent(indent);
                } else {
                    printXmlInternal(node->child, -1);
                }
                std::cout << "</" << node->key << ">\n";
            }
            break;
        }
        case NodeType::STRING:
            escapeXml(node->stringValue);
            if (indent != -1) std::cout << "\n";
            break;
        case NodeType::NUMBER:
            if (node->numberValue == (long long)node->numberValue) {
                std::cout << (long long)node->numberValue;
            } else {
                std::cout << node->numberValue;
            }
            if (indent != -1) std::cout << "\n";
            break;
        case NodeType::BOOL:
            std::cout << node->stringValue;
            if (indent != -1) std::cout << "\n";
            break;
        case NodeType::NULL_VAL:
            if (indent != -1) std::cout << "\n";
            break;
    }
}

void AstNode::printXml(int indent) {
    std::cout << "<root>\n";
    printXmlInternal(this, indent + 1);
    std::cout << "</root>" << std::endl;
}

void AstNode::printAst(int indent) {
    printIndent(indent);
    switch (type) {
        case NodeType::OBJECT: std::cout << "OBJECT" << std::endl; break;
        case NodeType::ARRAY: std::cout << "ARRAY" << std::endl; break;
        case NodeType::KEY_VALUE: std::cout << "KEY_VALUE: " << key << std::endl; break;
        case NodeType::STRING: std::cout << "STRING: " << stringValue << std::endl; break;
        case NodeType::NUMBER: std::cout << "NUMBER: " << numberValue << std::endl; break;
        case NodeType::BOOL: std::cout << "BOOL: " << stringValue << std::endl; break;
        case NodeType::NULL_VAL: std::cout << "NULL" << std::endl; break;
    }
    if (child) child->printAst(indent + 1);
    if (next) next->printAst(indent);
}
