/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <string>
#include <memory>
#include <utility>

/* project specific c++ includes */
#include "include/utils.hpp"
#include "include/view.hpp"
#include "include/ui_builder.hpp"

// =============================================================================
//                       Editor Impl
// =============================================================================
Editor::Editor(std::unique_ptr<TableUI> editor_ui)
    : __editor_ui(std::move(editor_ui)) {
    __editor_ui->move(START_Y, START_X);
}

int Editor::read() const {
    return __editor_ui->read();
}

void Editor::update(int ch) {
    __editor_ui->print(ch);
}

size_t Editor::get_line_number() const {
    return __editor_ui->get_cursor_y();
}

size_t Editor::get_column_number() const {
    return __editor_ui->get_cursor_x();
}

void Editor::move_to_next_line() {
    __editor_ui->move(get_line_number()+1, START_X);
}

void Editor::delete_last_char() {
    __editor_ui->print(get_line_number(), get_column_number()-1, ' ');
    __editor_ui->movex(get_column_number()-1);
}

std::string Editor::get_line() const {
    size_t current_y = get_line_number();
    size_t current_x = get_column_number();
    std::string instruction{};
    for (size_t x = START_X; x <= current_x; ++x) {
        instruction.append(1, __editor_ui->get_char_at(current_y, x));
    }
    return instruction;
}

// =============================================================================
//                       ViewUI Impl
// =============================================================================
void ViewUI::initialize() {
    add_table();
    add_buttons();
    add_registers();
    add_flags();
}

void ViewUI::add_table() {
    __address_ui = UIBuilder::create(UIType::Table)
        .setDimension(TABLE_LENGTH, ADDRESS_COL_SIZE)
        .setStartPosition(0, 0)
        .setHeader("ADDRESS")
        .build<TableUI>();

    std::unique_ptr<TableUI> source_code_ui = UIBuilder::create(UIType::Table)
        .setDimension(TABLE_LENGTH, SRC_CODE_SIZE)
        .setStartPosition(0, ADDRESS_COL_SIZE-1)
        .setHeader("SOURCE CODE")
        .build<TableUI>();
    editor = std::make_shared<Editor>(std::move(source_code_ui));

    __machine_code_ui = UIBuilder::create(UIType::Table)
        .setDimension(TABLE_LENGTH, MACHINE_CODE_SIZE)
        .setStartPosition(0, ADDRESS_COL_SIZE + SRC_CODE_SIZE -2)
        .setHeader("MACHINE CODE")
        .build<TableUI>();
}

void ViewUI::add_buttons() {
    __run_btn = UIBuilder::create(UIType::Button)
        .setDimension(BTN_LENGTH, RUN_BTN_SIZE)
        .setStartPosition(TABLE_LENGTH, 0)
        .setHeader("RUN PROGRAM")
        .build<ButtonUI>();

    __inspect_memory_btn = UIBuilder::create(UIType::Button)
        .setDimension(BTN_LENGTH, INSPECT_BTN_SIZE)
        .setStartPosition(TABLE_LENGTH, RUN_BTN_SIZE)
        .setHeader("INSPECT MEMORY")
        .build<ButtonUI>();
}

void ViewUI::add_registers() {
    __register_ui = UIBuilder::create(UIType::Registers)
        .setDimension(REG_WIN_LENGTH, REG_WIN_WIDTH)
        .setStartPosition(0, TABLE_WIDTH)
        .setHeader("REGISTERS")
        .build<RegistersUI>();
}

void ViewUI::add_flags() {
    __flag_ui = UIBuilder::create(UIType::Flags)
        .setDimension(FLAG_WIN_LENGTH, FLAG_WIN_WIDTH)
        .setStartPosition(REG_WIN_LENGTH, TABLE_WIDTH)
        .setHeader("FLAGS")
        .build<FlagsUI>();
}

void ViewUI::update(const ViewState& state) {
    const size_t current_line = editor->get_line_number();
    // __machine_code_ui->print(current_line, 2, utils::to_hex(opcode, 2));
    size_t current_pos = 2;
    for (auto& machine_code : state.machine_code) {
        const std::string code = utils::to_hex(machine_code, 2);
        __machine_code_ui->print(current_line, current_pos, code);
        current_pos += code.size() + 1;
    }
    __address_ui->print(current_line, 2, utils::to_hex(state.address, 4));
    editor->move_to_next_line();
}
