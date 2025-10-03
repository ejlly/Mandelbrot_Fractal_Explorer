#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "bmp/bmp.hpp"

#include "window.hpp"

void calculate_frame(Window &glWindow, Plot &memory, bool recalculate, bool drawsJulia);

bool revert_frame(std::vector<Plot> &memory);

bool catch_julia_press(Complex &origin);