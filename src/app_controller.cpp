/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cctype>
#include <cstdint>
#include <memory>
#include <string>

/* third-party c++ includes */
#include <ncurses_facade.hpp>

/* project specific c++ includes */
#include "include/app_controller.hpp"
#include "include/view.hpp"
#include "include/command.hpp"

// =============================================================================
//                       AppController Impl
// =============================================================================
AppController::AppController(ViewUI& view) : __view(view) {
    __view.initialize();
}

void AppController::run() {
    while (true) {
        int ch = __view.editor->read();
        handle_special_keys(ch);
        if (!valid_character(ch)) continue;
        ch = toupper(ch);
        __view.editor->update(ch);
        if (ch == 'Q') break;
    }
}

void AppController::handle_special_keys(int ch) {
    switch (ch) {
        // NOTE: this is KEY_ENTER_ and not KEY_ENTER
        case KEY_ENTER_:
            handle_enter();
            break;
        case KEY_BACKSPACE:
            handle_backspace();
            break;
    }
}

void AppController::handle_enter() {
    std::string instruction = __view.editor->get_line();
    std::shared_ptr<ICommand> cmd = ICommand::get_command(instruction);
    cmd->set_address(next_address());
    __source_code.push_back(cmd);
    ViewState state = {
        .machine_code = cmd->get_machine_code(),
        .address = cmd->get_address(),
    };
    __view.update(state);
}

void AppController::handle_backspace() {
    size_t current_row = __view.editor->get_line_number();
    size_t current_col = __view.editor->get_column_number();
    if (current_col == Editor::START_X) {
        if (current_row == Editor::START_Y) return;
        // TODO: handle this case
        return;
    }
    __view.editor->delete_last_char();
}

uint16_t AppController::next_address() {
    if (__source_code.empty()) return 0;
    auto last = __source_code.back();
    const uint16_t current_addr = last->get_address();
    const uint16_t next_addr = current_addr + last->get_machine_code().size();
    return next_addr;
}

bool AppController::valid_character(int ch) {
    return std::isalnum(ch) || ch == ' ' || ch == ',';
}
