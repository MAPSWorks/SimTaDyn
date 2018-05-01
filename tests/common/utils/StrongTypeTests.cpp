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
#include "StrongType.tpp"
#undef protected
#undef private

#include <crpcut.hpp>

TESTSUITE(StrongTypeClass)
{
  TEST(StrongTypeClassEquality)
  {
    STRONG_TYPEDEF(int, Foo);
    STRONG_TYPEDEF(int, Bar);

    Foo f1;
    Foo f2(42);
    Bar f3;

    f1 = 42;
    f3 = 20;
    ASSERT_EQ(true, f1 == f2);
    ASSERT_EQ(true, f3 != Bar(42));
    ASSERT_EQ(true, f3 < Bar(42));
    ASSERT_EQ(true, Bar(42) <= Bar(42));
    ASSERT_EQ(true, Bar(42) >= Bar(42));
    ASSERT_EQ(true, Bar(42) == Bar(42));
    ASSERT_EQ(true, Bar(42) < Bar(43));
    ASSERT_EQ(true, Bar(43) > Bar(42));
  }
}
