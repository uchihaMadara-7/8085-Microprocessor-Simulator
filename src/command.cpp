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
#include <vector>

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

uint16_t ICommand::get_address() {
    return __address;
}

std::vector<uint8_t> ICommand::get_machine_code() {
    return __machine_code;
}

uint8_t ICommand::get_opcode() {
    return __opcode;
}

void ICommand::set_address(uint16_t address) {
    __address = address;
}

void ICommand::parse(Parser parser) {
    size_t start_pos = __instruction.find(INSTR_SEPARATOR) + 1;
    std::string operand_part = __instruction.substr(start_pos);
    size_t length;
    std::string operand;
    switch (parser) {
        case Parser::NoOperand:
            break;
        case Parser::SingleOperand:
            _operands.push_back(operand_part);
            break;
        case Parser::DualOperand:
            length = operand_part.find(OPERAND_SEPARATOR);
            operand = operand_part.substr(0, length);
            _operands.push_back(operand);
            start_pos = length + 1;
            operand = utils::normalize_string(operand_part.substr(start_pos));
            _operands.push_back(operand);
            break;
    }
    setup_opcode_table();
    set_opcode(lookup_opcode());
    update_machine_code();
}

uint8_t ICommand::lookup_opcode() {
    std::string key{};
    for (auto& operand : _operands) {
        // TODO::
        // if (ICommand::is_address(operand)) {
        //     key += "Z";
        //     continue;
        // }
        key += operand;
    }
    return _opcode_db.at(key);
}

std::vector<uint8_t> ICommand::get_operand_codes() {
    std::vector<uint8_t> operand_code;
    // for (auto& operand : _operands) {
        // TODO:
        // if (!ICommand::is_address(operand)) continue;
        // // check if address/value is integer or hex
        // // if integer, convert to hex
        // // if address, break into two values 0xFFFF to 0xFF 0xFF
        // operand_code += hex_address;
    // }

    return operand_code;
}

void ICommand::set_opcode(const uint8_t opcode) {
    __opcode = opcode;
}

void ICommand::update_machine_code() {
    __machine_code.push_back(get_opcode());
    std::vector<uint8_t> operand_code = get_operand_codes();
    __machine_code.insert(__machine_code.end(), operand_code.begin(),
                          operand_code.end());
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
    for (auto& first_register : _registers) {
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
    for (auto& first_register : _registers) {
        for (auto& second_register : _registers) {
            const std::string key = first_register + second_register;
            _opcode_db[key] = current_opcode++;
        }
    }
    current_opcode = 0x77;
    _opcode_db["MA"] = current_opcode++;
    for (auto& second_register : _registers) {
        const std::string key = "A" + second_register;
        _opcode_db[key] = current_opcode++;
    }
}
