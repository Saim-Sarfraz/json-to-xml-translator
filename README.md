# JSON to XML Translator

A compiler-style translator that converts valid JSON input into well-formed XML. Built using Flex, Bison, and C++.

## Features
- **Full JSON Support**: Handles objects, arrays, strings, numbers, booleans, and null.
- **AST Construction**: Builds a complete Abstract Syntax Tree before translation.
- **XML Generation**:
  - Root wrapper `<root>`.
  - JSON objects mapped to elements.
  - JSON arrays mapped to repeated `<item>` tags.
  - Automatic XML character escaping (`&`, `<`, `>`, `"`, `'`).
  - Pretty-printed output with indentation.
- **Error Reporting**: Detailed error messages with line and column numbers.
- **Bonus Features**:
  - Pretty-printed XML output.
  - AST structure visualization.
  - Column-based error details.
  - Scientific notation support for numbers.
  - Unicode escape support (`\uXXXX`).

## Build Instructions
Ensure you have `flex`, `bison`, and `g++` installed.

```bash
make
```

## Running the Program
To translate JSON to XML:
```bash
./json2xml < input.json
```

To see the AST structure along with the XML output:
```bash
./json2xml --ast < input.json
```

## AST Structure
The AST is composed of `AstNode` objects, which track:
- `NodeType`: OBJECT, ARRAY, KEY_VALUE, STRING, NUMBER, BOOL, or NULL_VAL.
- `key`: The name for key-value pairs.
- `stringValue` / `numberValue`: Storage for scalar values.
- `child`: Pointer to the first child (e.g., members of an object or elements of an array).
- `next`: Pointer to the sibling node (linked list of siblings).

The tree is traversed recursively to generate the XML output. Objects and arrays increase the indentation level, while scalar values are printed within their respective tags.

## Project Structure
- `src/`: Contains all source code including the lexer, parser, and AST implementation.
  - `scanner.l`: Lexical analyzer (Flex).
  - `parser.y`: Syntax analyzer and AST builder (Bison).
  - `ast.h / ast.c`: AST node definitions and logic.
- `tests/`: Directory containing JSON test cases.
- `expected_outputs/`: Directory containing translated XML outputs for verification.
- `Makefile`: Build automation script.
- `README.md`: Project documentation.

## Assumptions
- Keys in JSON objects are valid XML tag names (as per assignment instructions).
- Input JSON is valid according to standard JSON syntax.
- Basic unescaping is supported via standard regex matching.
