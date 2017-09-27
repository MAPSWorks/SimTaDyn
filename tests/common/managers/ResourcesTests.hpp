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

#ifndef RESOURCES_TESTS_HPP_
#  define RESOURCES_TESTS_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#define protected public
#define private public
#include "ResourceManager.tpp"
#include "ILoader.hpp"
#undef protected
#undef private

class ResourcesTests : public CppUnit::TestFixture
{
  // CppUnit macros for setting up the test suite
  CPPUNIT_TEST_SUITE(ResourcesTests);
  CPPUNIT_TEST(testsResources);
  CPPUNIT_TEST(testsResourceManager);
  CPPUNIT_TEST(testsLoaderManager);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp();
  void tearDown();
  void testsResources();
  void testsResourceManager();
  void testsLoaderManager();
};

#endif /* RESOURCES_TESTS_HPP_ */