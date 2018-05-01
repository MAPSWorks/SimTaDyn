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

#include "ClassicSpreadSheet.hpp"
#include "PathManager.hpp"
#include <crpcut.hpp>

TESTSUITE(SpreadSheetTests)
{

  class Fixture
  {
  public:
    void eatSpreadsheet(ClassicSpreadSheet& sheet,
                        std::string const& file,
                        std::pair<bool, std::string> expected)
    {
      // Call it before Logger constructor
      if (!File::mkdir(config::tmp_path))
        {
          std::cerr << "Failed creating the temporary directory '"
                    << config::tmp_path << "'" << std::endl;
        }

      // Get full path of the file
      std::cout << PathManager::instance().toString() << std::endl;
      std::string filename = PathManager::instance().expand("src/core/standalone/ClassicSpreadSheet/examples/" + file);

      // Start Forth
      SimForth& forth = SimForth::instance();
      forth.boot();

      // Compute thes spreadsheet
      ASSERT_TRUE(sheet.readInput(filename));
      sheet.parse(forth);
      std::pair<bool, std::string> res = sheet.evaluate(forth);
      forth.ok(res);

      ASSERT_EQ(expected.first, res.first);
      ASSERT_EQ(expected.second, res.second);
    }
  };

  TEST(Input1, Fixture)
  {
    ClassicSpreadSheet sheet("Sheet1");
    eatSpreadsheet(sheet, "input1.txt", std::make_pair(true, "ok"));
    ASSERT_EQ(std::make_pair(true, 20), sheet.value(0, 0));
    ASSERT_EQ(std::make_pair(true, 20), sheet.value(0, 1));
    ASSERT_EQ(std::make_pair(true, 20), sheet.value(0, 2));
    ASSERT_EQ(std::make_pair(true, 8), sheet.value(1, 0));
    ASSERT_EQ(std::make_pair(true, 3), sheet.value(1, 1));
    ASSERT_EQ(std::make_pair(true, 1), sheet.value(1, 2));
  }

  TEST(Input2, Fixture)
  {
    ClassicSpreadSheet sheet("Sheet2");
    eatSpreadsheet(sheet, "input2.txt",
                   std::make_pair(false, "Aborting 'CircularDependencyFound: Unable to solve the spreadsheet'"));
    ASSERT_EQ(false, sheet.value(0, 0).first);
    ASSERT_EQ(false, sheet.value(0, 1).first);
    ASSERT_EQ(false, sheet.value(0, 2).first);
    ASSERT_EQ(false, sheet.value(1, 0).first);
    ASSERT_EQ(std::make_pair(true, 3), sheet.value(1, 1));
    ASSERT_EQ(false, sheet.value(1, 2).first);
  }

  TEST(Input3, Fixture)
  {
    ClassicSpreadSheet sheet("Sheet3");
    eatSpreadsheet(sheet, "input3.txt",
                   std::make_pair(false, "Aborting 'CircularDependencyFound: Unable to solve the spreadsheet'"));
    ASSERT_EQ(std::make_pair(true, 6),  sheet.value(0, 0));
    ASSERT_EQ(false,                    sheet.value(0, 1).first);
    ASSERT_EQ(std::make_pair(true, 3),  sheet.value(0, 2));
    ASSERT_EQ(std::make_pair(true, 12), sheet.value(1, 0));
    ASSERT_EQ(std::make_pair(true, 6),  sheet.value(1, 1));
    ASSERT_EQ(std::make_pair(true, 9),  sheet.value(1, 2));
    ASSERT_EQ(std::make_pair(true, 15), sheet.value(2, 0));
    ASSERT_EQ(std::make_pair(true, 10), sheet.value(2, 1));
    ASSERT_EQ(std::make_pair(true, 4),  sheet.value(2, 2));
  }

  TEST(Input4, Fixture)
  {
    ClassicSpreadSheet sheet("Sheet4");
    eatSpreadsheet(sheet, "input4.txt", std::make_pair(true, "ok"));
    ASSERT_EQ(std::make_pair(true, 6),   sheet.value(0, 0));
    ASSERT_EQ(std::make_pair(true, -11), sheet.value(0, 1));
    ASSERT_EQ(std::make_pair(true, 3),   sheet.value(0, 2));
    ASSERT_EQ(std::make_pair(true, 12),  sheet.value(1, 0));
    ASSERT_EQ(std::make_pair(true, 6),   sheet.value(1, 1));
    ASSERT_EQ(std::make_pair(true, 9),   sheet.value(1, 2));
    ASSERT_EQ(std::make_pair(true, 15),  sheet.value(2, 0));
    ASSERT_EQ(std::make_pair(true, 10),  sheet.value(2, 1));
    ASSERT_EQ(std::make_pair(true, 4),   sheet.value(2, 2));
  }

  TEST(Input5, Fixture)
  {
    ClassicSpreadSheet sheet("Sheet5");
    eatSpreadsheet(sheet, "input5.txt", std::make_pair(true, "ok"));
    ASSERT_EQ(std::make_pair(true, -6),  sheet.value(0, 0));
    ASSERT_EQ(std::make_pair(true, 1),   sheet.value(0, 1));
    ASSERT_EQ(std::make_pair(true, 3),   sheet.value(0, 2));
    ASSERT_EQ(std::make_pair(true, 0),   sheet.value(1, 0));
    ASSERT_EQ(std::make_pair(true, 6),   sheet.value(1, 1));
    ASSERT_EQ(std::make_pair(true, 9),   sheet.value(1, 2));
    ASSERT_EQ(std::make_pair(true, 3),   sheet.value(2, 0));
    ASSERT_EQ(std::make_pair(true, 10),  sheet.value(2, 1));
    ASSERT_EQ(std::make_pair(true, 4),   sheet.value(2, 2));
  }
}
