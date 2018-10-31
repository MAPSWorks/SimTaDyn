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

#ifndef GLVERTEX_BUFFER_HPP_
#  define GLVERTEX_BUFFER_HPP_

#  include "IGLObject.hpp"
#  include "PendingContainer.tpp"

// **************************************************************
//! \brief Buffer objects are OpenGL objects that store an array of
//! unformatted memory allocated by the OpenGL context (aka: the
//! GPU). These can be used to store vertex data, pixel data retrieved
//! from images or the framebuffer, and a variety of other things.
// **************************************************************
template<typename T>
class GLBuffer: public IGLObject<GLenum>
{
public:

  //! \brief Constructor with the object name
  GLBuffer(std::string const& name, const GLenum target,
           const GLenum usage = GL_DYNAMIC_DRAW)
    : IGLObject(name)
  {
    IGLObject::m_target = target;
    m_usage = usage;
  }

  //! \brief Constructor with the object name
  GLBuffer(const char *name, const GLenum target,
           const GLenum usage = GL_DYNAMIC_DRAW)
    : IGLObject(name)
  {
    IGLObject::m_target = target;
    m_usage = usage;
  }

  virtual ~GLBuffer() override
  {
    destroy();
  }

private:

  virtual bool create() override
  {
    glCheck(glGenBuffers(1, &m_handle));
    return false;
  }

  virtual void release() override
  {
    glCheck(glDeleteBuffers(1, &m_handle));
  }

  virtual void activate() override
  {
    glCheck(glBindBuffer(m_target, m_handle));
  }

  virtual void deactivate() override
  {
    glCheck(glBindBuffer(m_target, 0));
  }

  virtual bool setup() override
  {
    const GLsizeiptr bytes = static_cast<GLsizeiptr>
      (m_container.capacity() * sizeof (T));
    glCheck(glBufferData(m_target, bytes, NULL, m_usage));

    return false;
  }

  virtual inline bool needUpdate() const override
  {
    return m_container.hasPendingData();
  }

  virtual bool update() override
  {
    size_t pos_start, pos_end;
    m_container.getPendingData(pos_start, pos_end);
    m_container.clearPending();

    size_t offset = sizeof (T) * pos_start;
    size_t nbytes = sizeof (T) * (pos_end - pos_start + 1_z);
    glCheck(glBufferSubData(m_target,
                            static_cast<GLintptr>(offset),
                            static_cast<GLsizeiptr>(nbytes),
                            &(m_container.m_container[0])));
    return false;
  }

private:

  GLenum m_usage;

public:
  PendingContainer<T> m_container;


  /*
    dtype = [("position", np.float32, 3),
              ("color",    np.float32, 4)]
     V = np.zeros(4,dtype).view(gloo.VertexBuffer)

doit donner:

V.tofile("qq.bin")
00000000  00 00 5c 42 00 00 5c 42  00 00 5c 42 00 00 00 00  |..\B..\B..\B....|
00000010  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000070

et V['position'] pointe sur element 0 alors que V['color'] pointe sur

   */

  //TODO std::map<std::string, PendingContainer<T>> m_containers;
  //TODO PendingContainer<T>& operator[](std::string const& name)
  /*
     dtype = [("position", np.float32, 3),
              ("color",    np.float32, 4)]
     V = np.zeros(4,dtype).view(gloo.VertexBuffer)

V['position']
VertexBuffer([[ 55.,  55.,  55.],
       [  0.,   0.,   0.],
       [  0.,   0.,   0.],
       [  0.,   0.,   0.]], dtype=float32)

VertexBuffer([[ 0.,  0.,  0.,  0.],
       [ 0.,  0.,  0.,  0.],
       [ 0.,  0.,  0.,  0.],
       [ 0.,  0.,  0.,  0.]], dtype=float32)


V.pending_data
(0, 112) // 112 = 4 * (sizeof(Vect3f) + sizeof(Vector4f))
V._pending_data = None

V['position'][0] = 42
V.pending_data
(0, 12)

VertexBuffer([([ 42.,  42.,  42.], [ 0.,  0.,  0.,  0.]),
       ([  0.,   0.,   0.], [ 0.,  0.,  0.,  0.]),
       ([  0.,   0.,   0.], [ 0.,  0.,  0.,  0.]),
       ([  0.,   0.,   0.], [ 0.,  0.,  0.,  0.])],
      dtype=[('position', '<f4', (3,)), ('color', '<f4', (4,))])


   */
};

// **************************************************************
//! \brief Buffer for vertex attribute data.
// **************************************************************
template<typename T>
class GLVertexBuffer: public GLBuffer<T>
{
public:

  //! \brief Constructor with the object name
  GLVertexBuffer(std::string const& name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ARRAY_BUFFER, usage)
  {
  }

  //! \brief Constructor with the object name
  GLVertexBuffer(const char *name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ARRAY_BUFFER, usage)
  {
  }
};

// **************************************************************
//! \brief Buffer for index data.
// **************************************************************
template<typename T>
class GLIndexBuffer: public GLBuffer<T>
{
public:

  //! \brief Constructor with the object name
  GLIndexBuffer(std::string const& name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }

  //! \brief Constructor with the object name
  GLIndexBuffer(const char *name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBuffer<T>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }
};

#endif /* GLVERTEX_BUFFER_HPP_ */
