#include "FileTests.hpp"
#include "VectorTests.hpp"
#include "MatrixTests.hpp"
#include "TransformationTests.hpp"
#include "BoundingBoxTests.hpp"
#include "ColorTests.hpp"
#include "SetTests.hpp"
#include "CollectionTests.hpp"
#include "BasicNodeTests.hpp"
#include "BasicArcTests.hpp"
#include "BasicGraphTests.hpp"
#include "GraphAlgoTests.hpp"
#include "GLBufferTests.hpp"
#include "OpenGLTests.hpp"
#include "FilteringTests.hpp"
#include <cppunit/ui/text/TestRunner.h>

//--------------------------------------------------------------------------
static void testUtils(CppUnit::TextUi::TestRunner& runner)
{
  CppUnit::TestSuite* suite;

  suite = new CppUnit::TestSuite("FileTests");
  suite->addTest(new CppUnit::TestCaller<FileTests>("File tests", &FileTests::tests));
  runner.addTest(suite);
}

//--------------------------------------------------------------------------
static void testMath(CppUnit::TextUi::TestRunner& runner)
{
  CppUnit::TestSuite* suite;

  /*suite = new CppUnit::TestSuite("VectorTests");
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testCreator", &VectorTests::testCreator));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testSwap", &VectorTests::testSwap));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testEquality", &VectorTests::testEquality));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testArithmetic", &VectorTests::testArithmetic));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testCopy", &VectorTests::testCopy));
  suite->addTest(new CppUnit::TestCaller<VectorTests>("testOperations", &VectorTests::testOperations));
  runner.addTest(suite);*/

  suite = new CppUnit::TestSuite("MatrixTests");
  suite->addTest(new CppUnit::TestCaller<MatrixTests>("testCreator", &MatrixTests::testCreator));
  suite->addTest(new CppUnit::TestCaller<MatrixTests>("testSwap", &MatrixTests::testSwap));
  suite->addTest(new CppUnit::TestCaller<MatrixTests>("testEquality", &MatrixTests::testEquality));
  suite->addTest(new CppUnit::TestCaller<MatrixTests>("testArithmetic", &MatrixTests::testArithmetic));
  suite->addTest(new CppUnit::TestCaller<MatrixTests>("testCopy", &MatrixTests::testCopy));
  suite->addTest(new CppUnit::TestCaller<MatrixTests>("testOperations", &MatrixTests::testOperations));
  runner.addTest(suite);

  /*suite = new CppUnit::TestSuite("TransformationsTests");
  suite->addTest(new CppUnit::TestCaller<TransformationTests>("testMatrixTransform", &TransformationTests::tests));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("Filtering");
  suite->addTest(new CppUnit::TestCaller<FilteringTests>("RollingAverage", &FilteringTests::rolling));
  suite->addTest(new CppUnit::TestCaller<FilteringTests>("LowPassFilter", &FilteringTests::lpf));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("BoundingBoxTests");
  suite->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testCreator", &BoundingBoxTests::testCreator));
  suite->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testSwap", &BoundingBoxTests::testSwap));
  suite->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testEquality", &BoundingBoxTests::testEquality));
  suite->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testArithmetic", &BoundingBoxTests::testArithmetic));
  suite->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testCopy", &BoundingBoxTests::testCopy));
  suite->addTest(new CppUnit::TestCaller<BoundingBoxTests>("testOperations", &BoundingBoxTests::testOperations));
  runner.addTest(suite);*/
}

//--------------------------------------------------------------------------
static void testContainer(CppUnit::TextUi::TestRunner& runner)
{
  CppUnit::TestSuite* suite;

  suite = new CppUnit::TestSuite("SetTests");
  suite->addTest(new CppUnit::TestCaller<SetTests>("SetDummy", &SetTests::testDummy));
  suite->addTest(new CppUnit::TestCaller<SetTests>("SetInsert", &SetTests::testInsert));
  suite->addTest(new CppUnit::TestCaller<SetTests>("SetRemove", &SetTests::testRemove));
  suite->addTest(new CppUnit::TestCaller<SetTests>("SetInsertArr", &SetTests::testInsertArr));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("CollectionTests");
  suite->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionDummy", &CollectionTests::testDummy));
  suite->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionOccupy", &CollectionTests::testOccupy));
  suite->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionAppend", &CollectionTests::testAppend));
  suite->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionRemove", &CollectionTests::testRemove));
  suite->addTest(new CppUnit::TestCaller<CollectionTests>("CollectionInsert", &CollectionTests::testInsert));
  runner.addTest(suite);
}

//--------------------------------------------------------------------------
static void testGraph(CppUnit::TextUi::TestRunner& runner)
{
  CppUnit::TestSuite* suite;

  suite = new CppUnit::TestSuite("BasicArcTests");
  suite->addTest(new CppUnit::TestCaller<BasicArcTests>("test", &BasicArcTests::test));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("BasicNodeTests");
  suite->addTest(new CppUnit::TestCaller<BasicNodeTests>("test", &BasicNodeTests::dummy));
  suite->addTest(new CppUnit::TestCaller<BasicNodeTests>("test", &BasicNodeTests::neighbor));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("BasicGraphTests");
  suite->addTest(new CppUnit::TestCaller<BasicGraphTests>("test", &BasicGraphTests::test));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("GraphAlgoTests");
  suite->addTest(new CppUnit::TestCaller<GraphAlgoTests>("test", &GraphAlgoTests::test));
  runner.addTest(suite);
}

//--------------------------------------------------------------------------
static void testOpenGL(CppUnit::TextUi::TestRunner& runner)
{
  CppUnit::TestSuite* suite;

  suite = new CppUnit::TestSuite("ColorTests");
  suite->addTest(new CppUnit::TestCaller<ColorTests>("testCreator", &ColorTests::testCreator));
  suite->addTest(new CppUnit::TestCaller<ColorTests>("testSwap", &ColorTests::testSwap));
  suite->addTest(new CppUnit::TestCaller<ColorTests>("testEquality", &ColorTests::testEquality));
  suite->addTest(new CppUnit::TestCaller<ColorTests>("testArithmetic", &ColorTests::testArithmetic));
  suite->addTest(new CppUnit::TestCaller<ColorTests>("testCopy", &ColorTests::testCopy));
  suite->addTest(new CppUnit::TestCaller<ColorTests>("testOperations", &ColorTests::testOperations));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("GLBufferTests");
  suite->addTest(new CppUnit::TestCaller<GLBufferTests>("createOpenGLContext", &GLBufferTests::createOpenGLContext));
  suite->addTest(new CppUnit::TestCaller<GLBufferTests>("GLBufferDummy", &GLBufferTests::testDummy));
  suite->addTest(new CppUnit::TestCaller<GLBufferTests>("GLBufferPendingData", &GLBufferTests::testPendingData));
  suite->addTest(new CppUnit::TestCaller<GLBufferTests>("GLBufferInsert", &GLBufferTests::testInsert));
  suite->addTest(new CppUnit::TestCaller<GLBufferTests>("GLBufferSuppress", &GLBufferTests::testSuppress));
  suite->addTest(new CppUnit::TestCaller<GLBufferTests>("GLBufferSwap", &GLBufferTests::testSwap));
  suite->addTest(new CppUnit::TestCaller<GLBufferTests>("deleteOpenGLContext", &GLBufferTests::deleteOpenGLContext));
  runner.addTest(suite);

  suite = new CppUnit::TestSuite("OpenGLTests");
  suite->addTest(new CppUnit::TestCaller<OpenGLTests>("GLObject", &OpenGLTests::testOpenGL));
  runner.addTest(suite);
}

int main(void)
{
  CppUnit::TextUi::TestRunner runner;

  testUtils(runner);
  testMath(runner);
  testContainer(runner);
  //testGraph(runner);
  testOpenGL(runner);
  runner.run();

  return 0;
}
