/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __VIEW_HPP__
#define __VIEW_HPP__

/* standard c++ includes */
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <ncurses_facade.hpp>

/* project specific c++ includes */
#include "ui_builder.hpp"

struct ViewState {
    std::vector<uint8_t> machine_code;
    uint16_t address;
};

// =============================================================================
//                       Edtior Class for View
// =============================================================================
class Editor {
 public:
    explicit Editor(std::unique_ptr<TableUI> editor_ui);

    int read() const;
    void update(int ch);
    size_t get_line_number() const;
    size_t get_column_number() const;
    void move_to_next_line();
    void delete_last_char();
    std::string get_line() const;

 private:
    std::unique_ptr<TableUI> __editor_ui;

 public:
    static constexpr size_t START_Y = 3, START_X = 2;
};

// =============================================================================
//                       ViewUI Class for View
// =============================================================================
class ViewUI {
 public:
    void initialize();
    void add_table();
    void add_buttons();
    void add_registers();
    void add_flags();
    int read();
    void update(int ch);
    void update(const ViewState& state);

 public:
    std::shared_ptr<Editor> editor;

 private:
    std::unique_ptr<TableUI> __address_ui, __machine_code_ui;
    std::unique_ptr<RegistersUI> __register_ui;
    std::unique_ptr<FlagsUI> __flag_ui;
    std::unique_ptr<ButtonUI> __run_btn, __inspect_memory_btn;
};

#endif  // __VIEW_HPP__
