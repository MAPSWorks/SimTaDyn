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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef GLVBOTESTS_HPP_
#  define GLVBOTESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "OpenGL.hpp"
#undef protected
#undef private

class GLVBOTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(GLVBOTests);
  CPPUNIT_TEST(tests);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void tests();
};

#endif /* GLVBOTESTS_HPP_ */
