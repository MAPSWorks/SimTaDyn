#ifndef EXAMPLE_01_HPP_
#  define EXAMPLE_01_HPP_

#  include "OpenGL.hpp"
#  include "Movable.tpp"

class GLExample01: public IGLWindow
{
public:

  GLExample01()
    : m_quad("quad")
  {
  }

  ~GLExample01()
  {
    std::cout << "Bye" << std::endl;
  }

  virtual bool setup() override;
  virtual bool draw() override;

public:

  GLVertexShader     m_vs;
  GLFragmentShader   m_fs;
  GLProgram          m_quad;
  Movable<float, 3U> m_movable;
};

#  endif /* EXAMPLE_01_HPP_ */
