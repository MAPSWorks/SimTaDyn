#include "Example06.hpp"

//! \file Draw a cone

// FIXME
void GLExample06::setUniform(const char *name, Matrix44f const &mat)
{
  GLint id  = glCheck(glGetUniformLocation(m_shader, name));
  glCheck(glUniformMatrix4fv(id, 1, GL_FALSE, &mat[0U][0U]));
}

bool GLExample06::setup()
{
  LOGI("GLExample06::setup()");

  // Enable the depth buffer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Compile a shader program
  if (0U == m_shader.load("Example06.vertex", "Example06.fragment"))
    return false;

  // Tell to OpenGL how to manage VBO values. This fixes the size
  // of the VBO container to its current capacity (ie. now the VBO
  // size no longer be larger): the GPU have allocated static memory.
  return m_cone.setup(m_shader);
}

// Move or translate the cone
void GLExample06::moveMe()
{
  // Sign of translation, rotation
  int r = 0;
  int t = 0;

  // Translation
  if (glfwGetKey(m_window, GLFW_KEY_RIGHT))
    {
      t = 1;
    }
  else if (glfwGetKey(m_window, GLFW_KEY_LEFT))
    {
      t = -1;
    }

  if (t != 0)
    {
      const GLfloat offset = dt() * ((float) t) / 1.0f;
      m_movable.move(Vector3f(offset));
    }

  // Rotation
  if (glfwGetKey(m_window, GLFW_KEY_UP))
    {
      r = 1;
    }
  else if (glfwGetKey(m_window, GLFW_KEY_DOWN))
    {
      r = -1;
    }

  if (r != 0)
    {
      const GLfloat degreesPerSecond = 80.0f;
      m_degreesRotated += r * dt() * degreesPerSecond;
      m_degreesRotated = maths::wrapTo180(m_degreesRotated);
      m_movable.rotate(maths::radians(m_degreesRotated), Vector3f(0, 1, 0));
    }
}

// Draw the cone
bool GLExample06::draw()
{
  LOGI("GLExample06::draw()");

  // clear everything
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  m_shader.start();
  {
    moveMe();

    // Model matrix transformation
    setUniform("model", m_movable.transform());

    // Projetion matrix transformation
    float ratio = ((float) m_width) / ((float) m_height);
    Matrix44f projection = matrix::perspective(maths::radians(50.0f), ratio, 0.001f, 1000.0f);
    setUniform("projection", projection);

    // View matrix transformation
    Matrix44f camera = matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));
    setUniform("camera", camera);

    m_cone.draw();
  }
  m_shader.stop();

  return true;
}
