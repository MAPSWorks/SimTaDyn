// -*- c++ -*- Coloration Syntaxique pour Emacs

#ifndef GLVERTEX_BUFFER_TPP_
#  define GLVERTEX_BUFFER_TPP_

//! \file This class get its inspiration from the code of the Glumpy
//! project (Python + Numpy + modern OpenGL: a fast, scalable and
//! beautiful scientific visualization). https://github.com/glumpy/glumpy
//!
//! Original Copyright from Glumpy project's Python code:
//! Copyright (c) 2009-2016 Nicolas P. Rougier. All rights reserved.
//! Distributed under the (new) BSD License.

#  include "GLAttrib.hpp"
#  include "Set.tpp"
#  include <algorithm>

// **************************************************************
//!
// **************************************************************
template<typename T, const uint32_t N>
class GLBlockBuffer: public GLObject, public Block<T, N>
{
protected:

#  include "ContainerEnums.ipp"

public:

  //! \brief A GLBlockBuffer is an interface used to upload data to a GPU
  //! array buffer (GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER).
  GLBlockBuffer(const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject(), Block<T, N>()
  {
    m_target = target;
    m_usage = usage;
    clearPending();
  }

  GLBlockBuffer(std::string const& name, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject(name), Block<T, N>()
  {
    m_target = target;
    m_usage = usage;
    clearPending();
  }

  GLBlockBuffer(const char *name, const GLenum target, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLObject(name), Block<T, N>()
  {
    m_target = target;
    m_usage = usage;
    clearPending();
  }

  //! \brief Delete the object from GPU memory.
  ~GLBlockBuffer()
  {
    GLObject::destroy();
  }

  void draw(const GLenum mode, GLAttrib const& attrib)
  {
    // FIXME: attrib.size() peut etre 0 ici ?
    glCheck(glDrawArrays(mode, 0, Block<T, N>::occupation()/* / attrib.size()*/));
  }

protected:

  //! \brief Allocate memory in the GPU.
  virtual void create() override
  {
    // Total size of the container
    const uint32_t bytes = Block<T, N>::size() * sizeof (T);

    glCheck(glGenBuffers(1, &m_handle));
    activate();
    glCheck(glBufferData(m_target, bytes, nullptr, m_usage));
    deactivate();
  }

  //! \brief Delete the object from GPU memory.
  virtual inline void release() override
  {
    GLboolean res = glCheck(glIsBuffer(m_handle));
    if (res)
      {
        glCheck(glDeleteBuffers(1, &m_handle));
      }
  }

  //! \brief Bind the buffer to some target.
  virtual inline void activate() override
  {
    glCheck(glBindBuffer(m_target, m_handle));
  }

  //! \brief Unbind the current bound buffer.
  virtual inline void deactivate() override
  {
    glCheck(glBindBuffer(m_target, 0));
  }

  //! \brief Whether object needs to be updated.
  virtual inline bool needUpdate() override
  {
    uint32_t pos_start;
    uint32_t pos_end;
    bool b = hasPendingData(pos_start, pos_end);

    if (b)
      {
        m_offset = sizeof (T) * pos_start;
        m_nbytes = sizeof (T) * (pos_end - pos_start);
      }
    return b;
  }

  //! \brief Upload all pending data to GPU.
  virtual inline void update() override
  {
    glCheck(glBufferSubData(m_target, m_offset, m_nbytes, Block<T, N>::m_block));
    clearPending();
  }

  //! \brief Dummy configuration.
  virtual void setup() override
  {
  }

private:

  bool hasPendingData(uint32_t &pos_start, uint32_t &pos_end) const
  {
    // FIXME begin
    #warning "A finir"
    pos_start = 0;
    pos_end = Block<T,N>::occupation();
    return true;
    // FIXME end

    if ((uint32_t) -1 == m_pending_start)
      return false;

    pos_start = m_pending_start;
    pos_end = m_pending_end;
    return true;
  }

  void clearPending()
  {
    m_pending_start = (uint32_t) -1;
    m_pending_end = (uint32_t) -1;
  }

  void addPendingData(const uint32_t pos_start, const uint32_t pos_end)
  {
    if ((uint32_t) -1 == m_pending_start)
      {
        m_pending_start = pos_start;
        m_pending_end = pos_end;
      }
    else
      {
        m_pending_start = std::min(m_pending_start, pos_start);
        m_pending_end = std::max(m_pending_end, pos_end);
      }
  }

  //! Indicate which elements have been changed.
  uint32_t m_pending_start, m_pending_end;
  uint32_t m_offset, m_nbytes;

protected:

  GLenum m_usage;
};

// **************************************************************
//! \brief Buffer for vertex attribute data.
// **************************************************************
template<typename T, const uint32_t N>
class GLVertexBlockBuffer: public GLBlockBuffer<T, N>
{
public:

  GLVertexBlockBuffer(const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(GL_ARRAY_BUFFER, usage)
  {
  }

  GLVertexBlockBuffer(std::string const& name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(name, GL_ARRAY_BUFFER, usage)
  {
  }

  GLVertexBlockBuffer(const char *name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(name, GL_ARRAY_BUFFER, usage)
  {
  }
};

// **************************************************************
//! \brief Buffer for index data.
// **************************************************************
template<typename T, const uint32_t N>
class GLIndexBlockBuffer: public GLBlockBuffer<T, N>
{
public:

  GLIndexBlockBuffer(const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }

  GLIndexBlockBuffer(std::string const& name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }

  GLIndexBlockBuffer(const char *name, const GLenum usage = GL_DYNAMIC_DRAW)
    : GLBlockBuffer<T, N>(name, GL_ELEMENT_ARRAY_BUFFER, usage)
  {
  }
};

// **************************************************************
//! \brief
// **************************************************************
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block = GLBlockBuffer>
class GLBuffer: public Set<T, N, Block>
{
public:

  GLBuffer(const uint32_t reserve_elements = 1)
    : Set<T,N,Block>(reserve_elements)
  {
  }
};

// **************************************************************
//! \brief
// **************************************************************
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block = GLVertexBlockBuffer>
class GLVertexBuffer: public GLBuffer<T, N, Block>
{
public:

  GLVertexBuffer(const uint32_t reserve_elements = 1)
    : GLBuffer<T,N,Block>(reserve_elements)
  {
  }
};

// **************************************************************
//! \brief
// **************************************************************
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block = GLIndexBlockBuffer>
class GLIndexBuffer: public GLBuffer<T, N, Block>
{
public:

  GLIndexBuffer(const uint32_t reserve_elements = 1)
    : GLBuffer<T,N,Block>(reserve_elements)
  {
  }
};

//#  include "GLVertexBuffer.ipp"

#endif /* GLVERTEX_BUFFER_TPP_ */