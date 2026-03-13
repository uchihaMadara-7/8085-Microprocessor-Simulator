/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

/* standard c++ includes */
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// =============================================================================
//                       Macros for instructions
// =============================================================================
#define INSTR_SEPARATOR 0x20
#define OPERAND_SEPARATOR 0x2c

// =============================================================================
//                       Enum class for type of parsers
// =============================================================================
enum class Parser {
    NoOperand,
    SingleOperand,
    DualOperand,
};

// =============================================================================
//                       Command Interface Class
// =============================================================================
class ICommand {
 public:
    explicit ICommand(const std::string& instruction);

    static std::shared_ptr<ICommand> get_command(std::string instruction);
    virtual bool execute() = 0;
    virtual void undo() = 0;
    uint8_t get_opcode();

 protected:
    void parse(Parser parser);

    std::vector<std::string> _operands;
    std::unordered_map<std::string, int> _opcode_db;
    const std::vector<std::string> _registers =
        {"B", "C", "D", "E", "H", "L", "M", "A"};

 private:
    virtual uint8_t lookup_opcode();
    virtual uint8_t resolve_opcode(const std::string &key);
    virtual void setup_opcode_table() = 0;
    void set_opcode(const uint8_t opcode);

    std::string __instruction;
    uint8_t __opcode;
};

// =============================================================================
//                       ADD Class for ADD instruction
// =============================================================================
class ADD : public ICommand {
 public:
    explicit ADD(const std::string& instruction);

    bool execute() final;
    void undo() final;

 private:
    void setup_opcode_table() final;
};

// =============================================================================
//                       MOV Class for MOV instruction
// =============================================================================
class MOV : public ICommand {
 public:
    explicit MOV(const std::string& instruction);

    bool execute() final;
    void undo() final;

 private:
    void setup_opcode_table() final;
};
#endif  // __COMMAND_HPP__
