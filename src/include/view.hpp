/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __VIEW_HPP__
#define __VIEW_HPP__

/* standard c++ includes */
#include <memory>

/* project specific c++ includes */
#include "ui_builder.hpp"

// =============================================================================
//                       Edtior Class for View
// =============================================================================
class Editor {
 public:
    explicit Editor(std::unique_ptr<TableUI> editor_ui);

    int read() const;
    void update(int ch);

 private:
    std::unique_ptr<TableUI> __editor_ui;
    CursesWindow& __window;
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
    void run();

 public:
    std::shared_ptr<Editor> editor;

 private:
    std::unique_ptr<TableUI> __address_ui, __source_code_ui, __machine_code_ui;
    std::unique_ptr<RegistersUI> __register_ui;
    std::unique_ptr<FlagsUI> __flag_ui;
    std::unique_ptr<ButtonUI> __run_btn, __inspect_memory_btn;
};

#endif  // __VIEW_HPP__
