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
#include "Resource.hpp"
#include "ResourcesTests.hpp"
#include "ClassCounter.tpp"
#include "YesEngine/GenHierarchies.h"
#include <string>

using namespace Yes;

// Register the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(ResourcesTests);

// -----------------------------------------------------------
static bool destroyedA = false;
static bool destroyedB = false;
static bool loadedA = false;
static bool loadedB = false;
static bool savedA = false;
static bool savedB = false;

// -----------------------------------------------------------
class ResourceA: public Resource, private UniqueID<ResourceA>
{
public:

  void type() { LOGI("ResourceA"); }
  ResourceA() { LOGI("New ResourceA"); }
  ~ResourceA() { destroyedA = true; LOGI("Bye ResourceA"); }
};

using ResourceAPtr = std::shared_ptr<ResourceA>;

// -----------------------------------------------------------
class ResourceB: public Resource, private UniqueID<ResourceB>
{
public:

  void type() { LOGI("ResourceB"); }
  ResourceB() { LOGI("New ResourceB"); }
  ~ResourceB() { destroyedB = true; LOGI("Bye ResourceB"); }
};

using ResourceBPtr = std::shared_ptr<ResourceB>;

// -----------------------------------------------------------
typedef TYPELIST_2(ResourceA, ResourceB) ResourceList;

#include "ResourceManager.tpp"

using ResourceAManager = ResourceManager<ResourceA, int>;
using ResourceBManager = ResourceManager<ResourceB, std::string>;

#include "ILoader.tpp"

// -----------------------------------------------------------
class ResourceALoader : public ILoader<ResourceA>
{
public:

  ResourceALoader()
    : ILoader<ResourceA>("ResourceA")
  {
    LOGI("Creating a ResourceA loader %p", this);
  }

  virtual void loadFromFile(std::string const& /*filename*/, ResourceAPtr& resource) override
  {
    loadedA = true;
    std::cout << "ResourceA* loadFromFile(filename)" << std::endl;

    if (nullptr == resource)
      {
        ResourceAManager& mgrA = ResourceAManager::instance();
        ResourceAPtr r = mgrA.create();
        mgrA.add(42, r);
        assert(nullptr != r);
        resource = std::static_pointer_cast<ResourceA>(r);
        assert(nullptr != resource);
      }
  }

  virtual void saveToFile(ResourceAPtr const /*resource*/, std::string const& /*filename*/) override
  {
    savedA = true;
    std::cout << "void saveToFile(ResourceA)" << std::endl;
  }
};

// -----------------------------------------------------------
class ResourceBLoader : public ILoader<ResourceB>
{
public:

  ResourceBLoader()
    : ILoader<ResourceB>("ResourceB")
  {
    LOGI("Creating a ResourceB loader %p", this);
  }

  virtual void loadFromFile(std::string const& /*filename*/, ResourceBPtr &/*resource*/) override
  {
    loadedB = true;
    std::cout << "ResourceB* loadFromFile(filename)" << std::endl;
  }

  virtual void saveToFile(ResourceBPtr const /*resource*/, std::string const& /*filename*/) override
  {
    savedB = true;
    std::cout << "void saveToFile(ResourceB)" << std::endl;
  }
};

#include "LoaderManager.tpp"

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
  ResourceAPtr rA1 = ResourceAManager::instance().create();
  CPPUNIT_ASSERT_EQUAL(0L, rA1.use_count());
  CPPUNIT_ASSERT_EQUAL(0U, rA1->getID());
  CPPUNIT_ASSERT_EQUAL(false, destroyedA);

  ResourceAPtr rA2 = ResourceAManager::instance().create();
  CPPUNIT_ASSERT_EQUAL(0L, rA1.use_count());
  CPPUNIT_ASSERT_EQUAL(0L, rA2.use_count());
  CPPUNIT_ASSERT_EQUAL(1U, rA2->getID());
  CPPUNIT_ASSERT_EQUAL(false, destroyedA);

  rA1->acquire();
  CPPUNIT_ASSERT_EQUAL(1L, rA1.use_count());
  CPPUNIT_ASSERT_EQUAL(false, destroyedA);
  rA1->dispose();
  CPPUNIT_ASSERT_EQUAL(0L, rA1.use_count());
  rA1->dispose();
  CPPUNIT_ASSERT_EQUAL(true, destroyedA);
  destroyedA = false;
  rA2->dispose();
  CPPUNIT_ASSERT_EQUAL(true, destroyedA);

  // -- Resource B

  CPPUNIT_ASSERT_EQUAL(false, destroyedB);
  ResourceBPtr rB1 = ResourceB::create();
  CPPUNIT_ASSERT_EQUAL(0U, rB1->use_count());
  CPPUNIT_ASSERT_EQUAL(0, rB1->getID().compare("BB_0"));
  CPPUNIT_ASSERT_EQUAL(false, destroyedB);

  ResourceBPtr rB2 = ResourceB::create();
  CPPUNIT_ASSERT_EQUAL(0U, rB1->use_count());
  CPPUNIT_ASSERT_EQUAL(0U, rB2->use_count());
  CPPUNIT_ASSERT_EQUAL(0, rB2->getID().compare("BB_1"));
  CPPUNIT_ASSERT_EQUAL(false, destroyedB);

  rB1->acquire();
  CPPUNIT_ASSERT_EQUAL(1U, rB1->use_count());
  CPPUNIT_ASSERT_EQUAL(false, destroyedB);
  rB1->dispose();
  CPPUNIT_ASSERT_EQUAL(0U, rB1->use_count());
  rB1->dispose();
  CPPUNIT_ASSERT_EQUAL(true, destroyedB);
  destroyedB = false;
  rB2->dispose();
  CPPUNIT_ASSERT_EQUAL(true, destroyedB);
}

//--------------------------------------------------------------------------
void ResourcesTests::testsResourceManager()
{
  // Check empty resource manager
  ResourceManager<uint32_t> rm;
  CPPUNIT_ASSERT_EQUAL(0U, rm.size());
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.look(0U)); // Resource exists but not present in the manager
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.look(1U)); // Resource exists but not present in the manager

  // Insert 2 resources in the resource manager. Check they are inserted.
  rm.add(ResourceAManagerA::create()); // --> getID #2U
  rm.add(ResourceAManagerA::create()); // --> getID #3U
  CPPUNIT_ASSERT_EQUAL(2U, rm.size());
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rm.look(2U));
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rm.look(3U));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.look(4U));

  // Insert 3th resource and acquire it. Check acquisition.
  rm.add(ResourceAManagerA::create());
  uint32_t getID = 4U;
  CPPUNIT_ASSERT_EQUAL(3U, rm.size());
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rm.acquire(getID));
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.acquire(getID + 1U));
  CPPUNIT_ASSERT_EQUAL(1U, ((ResourceA*) rm.look(getID))->use_count());

  // Check disposing of the resource already acquired.
  rm.dispose(getID);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rm.acquire(getID));
  CPPUNIT_ASSERT_EQUAL(1U, ((ResourceA*) rm.look(getID))->use_count());
  rm.dispose(getID);
  CPPUNIT_ASSERT_EQUAL(0U, ((ResourceA*) rm.look(getID))->use_count());

  // Dispose the resource and check it's no longer exists.
  rm.dispose(getID);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rm.acquire(getID));
  CPPUNIT_ASSERT_EQUAL(2U, rm.size());
}

//--------------------------------------------------------------------------
void ResourcesTests::testsLoaderManager()
{
  LoaderManager &lm = LoaderManager::instance();
  lm.registerLoader<ResourceA>(std::make_shared<LoaderA>(), "a:A:aa::AA");
  lm.registerLoader<ResourceB>(std::make_shared<LoaderB>(), "bb");
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<ResourceA>("a"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<ResourceA>("A"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<ResourceA>("aa"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<ResourceA>("AA"));
  CPPUNIT_ASSERT_EQUAL(true, lm.hasLoader<ResourceB>("bb"));
  CPPUNIT_ASSERT_EQUAL(false, lm.hasLoader<ResourceA>("cc"));
  CPPUNIT_ASSERT_EQUAL(false, lm.hasLoader<ResourceB>("bb"));

  CPPUNIT_ASSERT_EQUAL(1u, lm.getLoader<ResourceB>("bb").use_count());
  CPPUNIT_ASSERT_EQUAL(4u, lm.getLoader<ResourceA>("a").use_count());

  CPPUNIT_ASSERT_EQUAL(false, loadedA);
  CPPUNIT_ASSERT_EQUAL(false, loadedB);
  CPPUNIT_ASSERT_EQUAL(false, savedA);
  CPPUNIT_ASSERT_EQUAL(false, savedB);

  std::cout << "1-----------------------------------------" << std::endl;
  ResourceA rA;
  lm.saveToFile(rA, "/home/toto.AA~");
  CPPUNIT_ASSERT_EQUAL(true, savedA);

  std::cout << "2-----------------------------------------" << std::endl;
  ResourceAPtr rA1 = nullptr;
  CPPUNIT_ASSERT_THROW(lm.loadFromFile("/home/tutu.a", rA1), LoaderException);
  CPPUNIT_ASSERT_EQUAL(true, nullptr == rA1); // the file does not exist
  CPPUNIT_ASSERT_EQUAL(false, loadedA);

  std::cout << "3-----------------------------------------" << std::endl;
  { std::fstream fs; fs.open("/tmp/toto.a", std::ios::out); fs.close(); } // create a file
  lm.loadFromFile("/tmp/toto.a", rA1);
  CPPUNIT_ASSERT_EQUAL(true, nullptr != rA1); // the file exists
  CPPUNIT_ASSERT_EQUAL(true, loadedA);
  // FIXME: call ressourceManager.dispose();
  delete rA1;

  std::cout << "4-----------------------------------------" << std::endl;
  ResourceB rB;
  lm.saveToFile(rB, "/home/toto.AA.bb");
  CPPUNIT_ASSERT_EQUAL(true, savedB);
  ResourceBPtr rB1;
  { std::fstream fs; fs.open("/tmp/tutu.bb", std::ios::out); fs.close(); } // create a file
  lm.loadFromFile("/tmp/tutu.bb", rB1);
  CPPUNIT_ASSERT_EQUAL(true, loadedB);
  // FIXME: call ressourceManager.dispose();
  delete rB1;

  CPPUNIT_ASSERT_THROW(lm.saveToFile(rB, "/home/toto.cc"), LoaderException);
  CPPUNIT_ASSERT_THROW(lm.saveToFile(rB, "/home/toto"), LoaderException);

  // Use already existing extension
  lm.registerLoader(new LoaderB(), "a");
  lm.saveToFile(rB, "/home/toto.a");
  lm.saveToFile(rB, "/home/toto.bb");
  lm.saveToFile(rA, "/home/toto.a");
}
