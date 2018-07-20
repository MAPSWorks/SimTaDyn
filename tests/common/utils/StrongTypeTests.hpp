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

#ifndef STRONG_TYPE_TESTS_HPP
#  define STRONG_TYPE_TESTS_HPP

#  include <cppunit/TestFixture.h>
#  include <cppunit/TestResult.h>
#  include <cppunit/extensions/HelperMacros.h>

#  define protected public
#  define private public
#  include "StrongType.tpp"
#  undef protected
#  undef private

class StrongTypeTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(StrongTypeTests);
  CPPUNIT_TEST(testEquality);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();
  void testEquality();
};

#endif /* STRONG_TYPE_TESTS_HPP */
