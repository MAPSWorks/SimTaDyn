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
#include "File.hpp"
#undef protected
#undef private

#include <crpcut.hpp>

TESTSUITE(FileClass)
{
  TEST(BasicTests)
  {
    std::string res;

    res = File::fileName("/home/qq/SimTaDyn/tests/common/FileTests.cpp");
    ASSERT_TRUE("FileTests.cpp" == res);

    res = File::fileName("/home/qq/SimTaDyn/tests/common");
    ASSERT_TRUE("common" == res);

    res = File::fileName("common");
    ASSERT_TRUE("common" == res);

    res = File::baseName("/home/qq/SimTaDyn/tests/common/FileTests.cpp");
    ASSERT_TRUE("FileTests" == res);

    res = File::baseName("/home/qq/SimTaDyn/tests/common");
    ASSERT_TRUE("common" == res);

    res = File::baseName("common");
    ASSERT_TRUE("common" == res);

    res = File::extension("/home/qq/SimTaDyn/tests/common/FileTests.cpp");
    ASSERT_TRUE("cpp" == res);

    res = File::extension("/home/qq/SimTaDyn/tests/common/FileTests.cpp~");
    ASSERT_TRUE("cpp" == res);

    res = File::extension("/home/qq/SimTaDyn/tests/common/FileTests.cpp.hpp");
    ASSERT_TRUE("hpp" == res);

    res = File::extension("/home/qq/SimTaDyn/tests/common");
    ASSERT_TRUE("" == res);

    res = File::extension("common");
    ASSERT_TRUE("" == res);

    ASSERT_FALSE(File::exist("pouet"));
    ASSERT_TRUE(File::exist("/dev/null"));

    ASSERT_TRUE(File::Directory == File::type("/tmp"));
    ASSERT_TRUE(File::isReadable("/tmp"));
    ASSERT_TRUE(File::isWritable("/tmp"));

    if (File::exist("/usr/sbin/")) // FIXME: does not exist on Travis-CI docker
      {
        ASSERT_TRUE(File::Directory == File::type("/usr/sbin/"));
        ASSERT_TRUE(File::isReadable("/usr/sbin/"));
        //FIXME ASSERT_TRUE(false == File::isWritable("/usr/sbin/"));
      }
    ASSERT_TRUE(File::DoesNotExist == File::type("/usr/sbin/foobar"));
    ASSERT_FALSE(File::isReadable("/usr/sbin/foobar"));
    ASSERT_FALSE(File::isWritable("/usr/sbin/foobar"));

    ASSERT_TRUE(File::Document == File::type("/bin/ls"));
    ASSERT_TRUE(File::isReadable("/bin/ls"));
    //FIXME ASSERT_TRUE(false == File::isWritable("/bin/ls"));

    ASSERT_TRUE(File::mkdir("/tmp/qq"));
    ASSERT_TRUE(File::mkdir("/tmp/foo/bar/"));
    ASSERT_TRUE(File::mkdir("/tmp/foo/bar/"));

    ASSERT_FALSE(File::mkdir("/dev/null"));

    ASSERT_TRUE(File::dirName("/tmp/foo/bar") == "/tmp/foo/");
    ASSERT_TRUE(File::dirName("/tmp/foo/") == "/tmp/foo/");
    ASSERT_TRUE(File::dirName("/tmp/foo") == "/tmp/");
    ASSERT_TRUE(File::dirName("/tmp/") == "/tmp/");
    ASSERT_TRUE(File::dirName("/tmp") == "/");

    ASSERT_TRUE(File::dirName("tmp/foo") == "tmp/");
    ASSERT_TRUE(File::dirName("tmp/foo/") == "tmp/foo/");
    ASSERT_TRUE(File::dirName("tmp") == "");
  }
}
