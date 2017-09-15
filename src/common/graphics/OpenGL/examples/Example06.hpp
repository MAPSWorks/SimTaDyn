#ifndef EXAMPLE_06_HPP_
#  define EXAMPLE_06_HPP_

#  include "GLWindow.hpp"
#  include "GLShader.hpp"
#  include "Cone.hpp"
#  include "Movable.tpp"

class GLExample06: public IGLWindow
{
public:

  GLExample06()
    : m_shader("Shader06"),
      m_cone(Vector3f(0.0f, 1.0f, 0.0f),
             Vector3f(-10.0f),
             6.0f, 2.0f, 12)
  {
  }

  ~GLExample06()
  {
    std::cout << "Bye" << std::endl;
  }

  virtual bool setup() override;
  virtual bool draw() override;
  void moveMe();
  void setUniform(const char *name, Matrix44f const &mat);

  GLShader m_shader;
  MeshCone m_cone;
  Movable<float, 3U> m_movable;
  float m_degreesRotated = 0.0f;
};

#  endif /* EXAMPLE_06_HPP_ */
