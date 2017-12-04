// MIT License
//
// Copyright (c) 2017 Artur Wyszyński, aljen at hitomi dot pl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#ifndef SPAGHETTI_LOGGER_H
#define SPAGHETTI_LOGGER_H

#include "spaghetti/api.h"

#include <spdlog/spdlog.h>

namespace spaghetti::log {

using Logger = std::shared_ptr<spdlog::logger>;
using Loggers = std::vector<Logger>;

template<typename... Args>
void info(Args... a_args)
{
  spdlog::apply_all([&](Logger l) { l->info(a_args...); });
}

template<typename... Args>
void warn(Args... a_args)
{
  spdlog::apply_all([&](Logger l) { l->warn(a_args...); });
}

template<typename... Args>
void error(Args... a_args)
{
  spdlog::apply_all([&](Logger l) { l->error(a_args...); });
}

template<typename... Args>
void critical(Args... a_args)
{
  spdlog::apply_all([&](Logger l) { l->critical(a_args...); });
}

template<typename... Args>
void debug(Args... a_args)
{
  spdlog::apply_all([&](Logger l) { l->debug(a_args...); });
}

template<typename... Args>
void trace(Args... a_args)
{
  spdlog::apply_all([&](Logger l) { l->trace(a_args...); });
}

SPAGHETTI_API void init();

SPAGHETTI_API Loggers get();

inline void init_from_plugin()
{
  auto loggers = get();
  for (auto &&logger : loggers) spdlog::register_logger(logger);
}

} // namespace spaghetti::log

#endif // SPAGHETTI_LOGGER_H
