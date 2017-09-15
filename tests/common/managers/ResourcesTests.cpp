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

#include "ResourcesTests.hpp"
#include "ClassCounter.tpp"
#include <iostream>
#include <fstream>

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(ResourcesTests);

//=====================================================================
// Class where unique ID is integer
static bool destroyedA = false;
class DonneeA
  : private UniqueID<DonneeA>
{
public:

  DonneeA()
    : m_id(UniqueID<DonneeA>::getID())
  {
     LOGI("Create DonneeA %u", m_id);
  }

  ~DonneeA()
  {
     LOGI("Destroy DonneeA %u", m_id);
     destroyedA = true;
  }

  inline Key id() const
  {
    return m_id;
  }

  Key m_id;
};

//=====================================================================
// Class where unique ID is string
static bool destroyedB = false;
class DonneeB
  : private UniqueID<DonneeB>
{
public:

  DonneeB()
    : m_id("DonneeB_" + std::to_string(UniqueID<DonneeB>::getID()))
  {
    LOGI("Create DonneeB %s", m_id.c_str());
  }

  ~DonneeB()
  {
    LOGI("Destroy DonneeB %s", m_id.c_str());
     destroyedB = true;
  }

  inline const std::string& id() const
  {
    return m_id;
  }

  std::string m_id;
};

//=====================================================================
// For unit test
#define protected public
#define private public
// Finish constructing loader manager and resource manager
#include "Utilities/GenHierarchies.h"
using namespace Yes;
typedef TYPELIST_2(DonneeA, DonneeB) ResourceList;
#include "LoaderManager.tpp"
#include "ResourceManager.tpp"
// For unit test
#undef protected
#undef private

//=====================================================================
// Class loading DonneeA
static bool loadedA = false;
static bool savedA = false;
class LoaderA: public ILoader<DonneeA>
{
public:
  LoaderA()
    : ILoader<DonneeA>("Loader DonneeA")
  {
  }

  ~LoaderA() {};

  // FIXME: filename doit etre l'unique ID
  // FIXME peut on cacher DonneeA* &object en faisant un DonneeA* rA = rm.get(id); if (nullptr == rA) {nouvelle carte }
  virtual void loadFromFile(std::string const& /*filename*/, DonneeA* &object) override
  {
    std::cout << "DonneeA* loadFromFile(std::string const& filename)" << std::endl;
    Resource<DonneeA> *rA = ResourceManager::instance().add();
    *object = rA->object();
    loadedA = true;
  }

  virtual void saveToFile(DonneeA const& /*object*/, std::string const& /*filename*/) override
  {
    std::cout << "void saveToFile(DonneeA const& /*object*/, std::string const& filename)" << std::endl;
    savedA = true;
  }
};

// FIXME dois je faire une function globale (pour le Forth) get(id) { if (nullptr == get(id) { gtkfiledialog; load(id); rm.add() } return get() }

//=====================================================================
// Class loading DonneeB
static bool loadedB = false;
static bool savedB = false;
class LoaderB: public ILoader<DonneeB>
{
public:
  LoaderB()
    : ILoader<DonneeB>("Loader DonneeB")
  {
  }

  ~LoaderB() {};

  virtual void loadFromFile(std::string const& /*filename*/, DonneeB* &object) override
  {
    std::cout << "DonneeB* loadFromFile(std::string const& filename)" << std::endl;
    Resource<DonneeB> *rB = ResourceManager::instance().create<DonneeB>();
    *object = rB->object();
    loadedB = true;
  }

  virtual void saveToFile(DonneeB const& /*object*/, std::string const& /*filename*/) override
  {
    std::cout << "void saveToFile(DonneeB const& /*object*/, std::string const& filename)" << std::endl;
    savedB = true;
  }
};

void LoaderManager::registerAllLoaders()
{
}

//--------------------------------------------------------------------------
void ResourcesTests::setUp()
{
}

//--------------------------------------------------------------------------
void ResourcesTests::tearDown()
{
}

//--------------------------------------------------------------------------
void ResourcesTests::testsResources()
{
  // -- Resource A

  CPPUNIT_ASSERT_EQUAL(false, destroyedA);
  LOGI("111111111111111111111111111111111");
  Resource<DonneeA> *rA1 = ResourceManager::instance().create<DonneeA>();
  CPPUNIT_ASSERT_EQUAL(0U, rA1->owners());
  CPPUNIT_ASSERT_EQUAL(1U, rA1->object().m_id); // FIXME Shall start to 0
  CPPUNIT_ASSERT_EQUAL(false, destroyedA);

  LOGI("111111111111111111111111111111111");
  Resource<DonneeA> *rA2 = ResourceManager::instance().create<DonneeA>();
  CPPUNIT_ASSERT_EQUAL(0U, rA1->owners());
  CPPUNIT_ASSERT_EQUAL(0U, rA2->owners());
  CPPUNIT_ASSERT_EQUAL(2U, rA2->object().m_id);
  CPPUNIT_ASSERT_EQUAL(false, destroyedA);

  rA1->acquire();
  CPPUNIT_ASSERT_EQUAL(1U, rA1->owners());
  CPPUNIT_ASSERT_EQUAL(false, destroyedA);
  rA1->dispose();
  CPPUNIT_ASSERT_EQUAL(0U, rA1->owners());
  rA1->dispose();
  CPPUNIT_ASSERT_EQUAL(true, destroyedA);

  // -- Resource B

  CPPUNIT_ASSERT_EQUAL(false, destroyedB);
  LOGI("111111111111111111111111111111111");
  Resource<DonneeB> *rB1 = ResourceManager::instance().create<DonneeB>();
  CPPUNIT_ASSERT_EQUAL(0U, rB1->owners());
  CPPUNIT_ASSERT_EQUAL(0, rB1->object().m_id.compare("DonneeB_1")); // FIXME Shall start to 0
  CPPUNIT_ASSERT_EQUAL(false, destroyedB);

  LOGI("111111111111111111111111111111111");
  Resource<DonneeB> *rB2 = ResourceManager::instance().create<DonneeB>();
  CPPUNIT_ASSERT_EQUAL(0U, rB1->owners());
  CPPUNIT_ASSERT_EQUAL(0U, rB2->owners());
  CPPUNIT_ASSERT_EQUAL(0, rB2->object().m_id.compare("DonneeB_2"));
  CPPUNIT_ASSERT_EQUAL(false, destroyedB);

  rB1->acquire();
  CPPUNIT_ASSERT_EQUAL(1U, rB1->owners());
  CPPUNIT_ASSERT_EQUAL(false, destroyedB);
  rB1->dispose();
  CPPUNIT_ASSERT_EQUAL(0U, rB1->owners());
  rB1->dispose();
  CPPUNIT_ASSERT_EQUAL(true, destroyedB);

  // Check if the suicide of a local resource
  // will not create a segfault ?
  Resource<DonneeB> resources[10];
}

//--------------------------------------------------------------------------
void ResourcesTests::testsResourceManager()
{
  // Check empty resource manager
  ResourceManager rm;
  CPPUNIT_ASSERT_EQUAL(0U, rm.size<DonneeA>());
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.find<DonneeA>("0")); // FIXME resource ID should starts to 0 not to 1
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.find<DonneeA>("1")); // Resource exists but not present in the manager
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.find<DonneeA>("2")); // Resource exists but not present in the manager


  // Insert 2 resources in the resource manager. Check they are inserted.
  LOGI("111111111111111111111111111111111");
  rm.add<DonneeA>(); // --> id #3U // FIXME shall be #2
  LOGI("222222222222222222222222222222222");
  rm.add<DonneeA>(); // --> id #4U // FIXME shall be #3
  CPPUNIT_ASSERT_EQUAL(2U, rm.size<DonneeA>());
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.find<DonneeA>("2")); // FIXME shall exists
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rm.find<DonneeA>("3"));
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rm.find<DonneeA>("4"));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.find<DonneeA>("5"));

  // Insert 3th resource and acquire it. Check acquisition.
  rm.add<DonneeA>(); // --> id #5U // FIXME shall be #4
  std::string id = "5";
  CPPUNIT_ASSERT_EQUAL(3U, rm.size<DonneeA>());
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rm.acquire<DonneeA>("5"));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.acquire<DonneeA>("6"));
  CPPUNIT_ASSERT_EQUAL(1U, rm.find<DonneeA>(id)->owners());

  // Check disposing of the resource already acquired.
  rm.dispose<DonneeA>(id);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rm.acquire<DonneeA>(id));
  CPPUNIT_ASSERT_EQUAL(1U, rm.find<DonneeA>(id)->owners());
  rm.dispose<DonneeA>(id);
  CPPUNIT_ASSERT_EQUAL(0U, rm.find<DonneeA>(id)->owners());

  // Dispose the resource and check it's no longer exists.
  rm.dispose<DonneeA>(id);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.acquire<DonneeA>(id));
  CPPUNIT_ASSERT_EQUAL(2U, rm.size<DonneeA>());
}

//--------------------------------------------------------------------------
void ResourcesTests::testsLoaderManager()
{
  LoaderManager &lm = LoaderManager::instance();
  lm.registerLoader(new LoaderA(), "a:A:aa::AA");
  lm.registerLoader(new LoaderB(), "bb");
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<DonneeA>("a"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<DonneeA>("A"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<DonneeA>("aa"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<DonneeA>("AA"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<DonneeB>("bb"));
  CPPUNIT_ASSERT_EQUAL(false, lm.hasLoader<DonneeA>("cc"));

  CPPUNIT_ASSERT_EQUAL(false, loadedA);
  CPPUNIT_ASSERT_EQUAL(false, loadedB);
  CPPUNIT_ASSERT_EQUAL(false, savedA);
  CPPUNIT_ASSERT_EQUAL(false, savedB);
  return ;
  std::cout << "1-----------------------------------------" << std::endl;
  DonneeA rA;
  lm.saveToFile(rA, "/home/toto.AA~");
  CPPUNIT_ASSERT_EQUAL(true, savedA);

  std::cout << "2-----------------------------------------" << std::endl;
  DonneeA *rA1 = nullptr;
  CPPUNIT_ASSERT_THROW(lm.loadFromFile("/home/tutu.a", rA1), LoaderException);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rA1); // the file does not exist
  CPPUNIT_ASSERT_EQUAL(false, loadedA);

  std::cout << "3-----------------------------------------" << std::endl;
  { std::fstream fs; fs.open("/tmp/toto.a", std::ios::out); fs.close(); } // create a file
  lm.loadFromFile("/tmp/toto.a", rA1);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rA1); // the file exists
  CPPUNIT_ASSERT_EQUAL(true, loadedA);

  std::cout << "4-----------------------------------------" << std::endl;
  DonneeB rB;
  lm.saveToFile(rB, "/home/toto.AA.bb");
  CPPUNIT_ASSERT_EQUAL(true, savedB);
  DonneeB *rB1;
  { std::fstream fs; fs.open("/tmp/tutu.bb", std::ios::out); fs.close(); } // create a file
  lm.loadFromFile("/tmp/tutu.bb", rB1);
  CPPUNIT_ASSERT_EQUAL(true, loadedB);

  CPPUNIT_ASSERT_THROW(lm.saveToFile(rB, "/home/toto.cc"), LoaderException);
  CPPUNIT_ASSERT_THROW(lm.saveToFile(rB, "/home/toto"), LoaderException);

  // Use already existing extension
  lm.registerLoader(new LoaderB(), "a");
  lm.saveToFile(rB, "/home/toto.a");
  lm.saveToFile(rB, "/home/toto.bb");
  lm.saveToFile(rA, "/home/toto.a");
}
