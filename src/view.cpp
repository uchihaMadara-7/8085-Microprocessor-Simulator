/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cctype>
#include <string>
#include <memory>
#include <utility>

/* project specific c++ includes */
#include "include/ui_builder.hpp"
#include "include/view.hpp"

// =============================================================================
//                       Editor Impl
// =============================================================================
Editor::Editor(std::unique_ptr<TableUI> editor_ui)
    : __editor_ui(std::move(editor_ui)),
        __window(__editor_ui->get_window()) {
    __window.move(3, 2);
}

int Editor::read() const {
    return __window.read();
}

void Editor::update(int ch) {
    __window.print(ch);
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

    __source_code_ui = UIBuilder::create(UIType::Table)
        .setDimension(TABLE_LENGTH, SRC_CODE_SIZE)
        .setStartPosition(0, ADDRESS_COL_SIZE-1)
        .setHeader("SOURCE CODE")
        .build<TableUI>();
    editor = std::make_shared<Editor>(std::move(__source_code_ui));

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

void ViewUI::run() {
    while (true) {
        int ch = editor->read();
        if (!std::isalnum(ch) && ch != ' ' && ch != ',') continue;
        ch = toupper(ch);
        editor->update(ch);
        if (ch == 'Q') break;
    }
}
