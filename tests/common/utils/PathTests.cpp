//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
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

#define protected public
#define private public
#include "Path.hpp"
#undef protected
#undef private

#include <crpcut.hpp>

TESTSUITE(PathClass)
{
  TEST(EmptyConstructor)
  {
    Path path;

    ASSERT_EQ(0u, path.m_paths.size());
    ASSERT_EQ(':', path.m_delimiter);
    ASSERT_EQ(true, path.m_path == "");
  }

  TEST(SplitConstructor)
  {
    Path path("/a/b:c/d");

    ASSERT_EQ(2u, path.m_paths.size());
    ASSERT_EQ(true, path.toString() == "/a/b:c/d:");
    path.clear();
    ASSERT_EQ(0u, path.m_paths.size());
    ASSERT_EQ(true, path.toString() == "");
    path.add("g/g");
    ASSERT_EQ(1u, path.m_paths.size());
    ASSERT_EQ(true, path.toString() == "g/g:");
    path.init("a/b");
    ASSERT_EQ(1u, path.m_paths.size());
    ASSERT_EQ(true, path.toString() == "a/b:");
    std::cout << path.toString() << std::endl;
  }

  TEST(SplitDir)
  {
    Path path;
    path.split("/a//b\\d/:e\\d:");
    ASSERT_EQ(true, path.toString() == "/a//b\\d:e\\d:");
    ASSERT_EQ(2u, path.m_paths.size());
    path.remove("incorrect/path");
    ASSERT_EQ(2u, path.m_paths.size());
    path.remove("/a//b\\d");
    ASSERT_EQ(2u, path.m_paths.size()); // FIXME should be1
    path.remove("/a//b\\d/");
    ASSERT_EQ(1u, path.m_paths.size());
    ASSERT_EQ(true, path.toString() == "e\\d:");
    path.remove("e\\d/");
    ASSERT_EQ(0u, path.m_paths.size());
    path.remove("");
    ASSERT_EQ(0u, path.m_paths.size());
    path.remove("incorrect/path");
    ASSERT_EQ(0u, path.m_paths.size());
    path.add("g/g");
    ASSERT_EQ(1u, path.m_paths.size());
    ASSERT_EQ(true, path.toString() == "g/g:");
  }

  TEST(FindAndExpand)
  {
    Path path("/bin:/usr/bin:/usr/local/bin");
    ASSERT_EQ(true, path.expand("ls") != "ls");
    auto res1 = path.find("ls");
    ASSERT_EQ(true, res1.second);
    ASSERT_EQ(true, res1.first != "ls");
    ASSERT_EQ(true, res1.first != "");

    auto res3 = path.find(res1.first);
    ASSERT_EQ(true, res3.second);
    ASSERT_EQ(true, res3.first == res1.first);

    path.clear();
    ASSERT_EQ(true, path.expand("ls") == "ls");
    auto res2 = path.find("ls");
    ASSERT_EQ(false, res2.second);
    ASSERT_EQ(true, res2.first == "");
  }
}
