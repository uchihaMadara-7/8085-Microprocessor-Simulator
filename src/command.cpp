/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

/* project specific c++ includes */
#include "include/utils.hpp"
#include "include/command.hpp"

// =============================================================================
//                       ICommand Impl
// =============================================================================
ICommand::ICommand(const std::string& instruction)
    : __instruction(utils::normalize_string(instruction)) {}

std::shared_ptr<ICommand> ICommand::get_command(std::string instruction) {
    std::string command = utils::get_first_word(instruction);
    if (command == "ADD") {
        return std::make_shared<ADD>(instruction);
    } else if (command == "MOV") {
        return std::make_shared<MOV>(instruction);
    }
    throw std::invalid_argument("invalid command");
}

uint8_t ICommand::get_opcode() {
    return __opcode;
}

void ICommand::parse(Parser parser) {
    size_t start_pos;
    std::string operand;
    switch (parser) {
        case Parser::NoOperand:
            break;
        case Parser::SingleOperand:
            start_pos = __instruction.find(' ') + 1;
            operand = __instruction.substr(start_pos, 1);
            _operands.push_back(operand);
            break;
        case Parser::DualOperand:
            start_pos = __instruction.find(' ') + 1;
            operand = __instruction.substr(start_pos, 1);
            _operands.push_back(operand);
            start_pos = __instruction.find(',') + 1;
            operand = utils::normalize_string(__instruction.substr(start_pos));
            _operands.push_back(operand);
            break;
    }
    setup_opcode_table();
    set_opcode(lookup_opcode());
}

uint8_t ICommand::lookup_opcode() {
    std::string key{};
    for (auto &operand : _operands) {
        // if (is_address(operand)) {
        //     key += "Z";
        //     continue;
        // }
        key += operand;
    }
    return resolve_opcode(key);
}

uint8_t ICommand::resolve_opcode(const std::string& key) {
    return _opcode_db.at(key);
}

void ICommand::set_opcode(const uint8_t opcode) {
    __opcode = opcode;
}

// =============================================================================
//                       ADD Impl
// =============================================================================
ADD::ADD(const std::string& instruction) : ICommand(instruction) {
    parse(Parser::SingleOperand);
}

bool ADD::execute() {
    return true;
}

void ADD::undo() {
}

void ADD::setup_opcode_table() {
    uint8_t current_opcode = 0x80;
    for (auto &first_register : _registers) {
        _opcode_db[first_register] = current_opcode++;
    }
}

// =============================================================================
//                       MOV Impl
// =============================================================================
MOV::MOV(const std::string& instruction) : ICommand(instruction) {
    parse(Parser::DualOperand);
}

bool MOV::execute() {
    return true;
}

void MOV::undo() {
}

void MOV::setup_opcode_table() {
    uint8_t current_opcode = 0x40;
    for (auto &first_register : _registers) {
        for (auto &second_register : _registers) {
            const std::string key = first_register + second_register;
            _opcode_db[key] = current_opcode++;
        }
    }
    current_opcode = 0x77;
    _opcode_db["MA"] = current_opcode++;
    for (auto &second_register : _registers) {
        const std::string key = "A" + second_register;
        _opcode_db[key] = current_opcode++;
    }
}
