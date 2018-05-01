//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "Logger.hpp"
#include <iterator>
#include <thread>
#include <crpcut.hpp>

static const uint32_t header_footer_lines = 6U + 5U;

static uint32_t number_of_lines(std::string const& file)
{
  uint32_t line_count = 0U;
  std::ifstream myfile(file);

  // new lines will be skipped unless we stop it from happening:
  myfile.unsetf(std::ios_base::skipws);

  // count the newlines with an algorithm specialized for counting:
  line_count = std::count(
    std::istream_iterator<char>(myfile),
    std::istream_iterator<char>(), '\n');

  return line_count;
}

static void call_from_thread(uint32_t const x, uint32_t const lines_by_thread)
{
  for (uint32_t i = 0U; i < lines_by_thread; ++i)
    {
      LOGI("Hello World from thread %3u", x);
    }
}

TESTSUITE(LoggerClass)
{
  TEST(EmptyLogTests)
  {
    constexpr uint32_t num_threads = 0U;
    constexpr uint32_t lines_by_thread = 0U;

    Logger::instance("/tmp/empty.log");
    Logger::destroy();

    uint32_t lines = number_of_lines("/tmp/empty.log");
    ASSERT_EQ(num_threads * lines_by_thread + header_footer_lines, lines);
  }

  TEST(BasicTests)
  {
    constexpr uint32_t num_threads = 1U;
    constexpr uint32_t lines_by_thread = 2U;

    Logger::instance().changeLog("/tmp/foo/bar/new.log");
    CPP_LOG(logger::Fatal) << "test\n";
    LOGE("Coucou");
    Logger::destroy();

    uint32_t lines = number_of_lines("/tmp/foo/bar/new.log");
    ASSERT_EQ(num_threads * lines_by_thread + header_footer_lines, lines);
  }

  TEST(LogWithConcurrencyTests)
  {
    constexpr uint32_t num_threads = 10U;
    constexpr uint32_t lines_by_thread = 10U;

    Logger::instance();

    static std::thread t[num_threads];

    // Launch a group of threads
    for (uint32_t i = 0; i < num_threads; ++i)
      {
        t[i] = std::thread(call_from_thread, i, lines_by_thread);
      }

    // Join the threads with the main thread
    for (uint32_t i = 0; i < num_threads; ++i)
      {
        if (t[i].joinable())
          {
            t[i].join();
          }
      }
    Logger::destroy();

    // Check: count the number of lines == num_threads + header + footer
    uint32_t lines = number_of_lines(config::log_path);
    ASSERT_EQ(num_threads * lines_by_thread + header_footer_lines, lines);
  }
}
