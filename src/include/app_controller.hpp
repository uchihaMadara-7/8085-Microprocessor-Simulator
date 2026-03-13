/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __APP_CONTROLLER_HPP__
#define __APP_CONTROLLER_HPP__

/* standard c++ includes */
#include <cstdint>
#include <vector>

/* project specific c++ includes */
#include "view.hpp"
#include "command.hpp"

// =============================================================================
//                       App Controller (Presenter in MVP)
// =============================================================================
class AppController {
 public:
    AppController(ViewUI& view);

    void run();
    void handle_special_keys(int ch);
    void handle_enter();
    void handle_backspace();
    uint16_t next_address();
    bool valid_character(int ch);

 private:
    ViewUI& __view;
    std::vector<std::shared_ptr<ICommand>> __source_code;
};

#endif  // __APP_CONTROLLER_HPP__
