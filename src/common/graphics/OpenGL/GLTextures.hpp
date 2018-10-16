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

#ifndef GLTEXTURES_HPP_
#  define GLTEXTURES_HPP_

#  include "IGLObject.hpp"
//#  include "Resource.hpp"
#  include "PendingData.hpp"
#  include "SOIL/SOIL.h"

// **************************************************************
//
// **************************************************************
class IGLTexture
  : public IGLObject<GLenum>,
    //public Resource,
    protected PendingData
{
public:

  IGLTexture(const GLenum target)
    : IGLObject()
  {
    m_target = target;
  }

  IGLTexture(std::string const& name, const GLenum target)
    : IGLObject(name)
  {
    m_target = target;
  }

  virtual ~IGLTexture()
  {
    destroy();
  }

  void interpolation(const float min_filter, const float  mag_filter)
  {
    m_min_filter = min_filter;
    m_mag_filter = mag_filter;
    redoSetup();
  }

  void interpolation(const float min_filter)
  {
    m_min_filter = min_filter;
    m_mag_filter = min_filter;
    redoSetup();
  }

  void wrapping(const float wrap)
  {
    m_wrapping = wrap;
    redoSetup();
  }

protected:

  void applyParam()
  {
    glCheck(glTexParameterf(m_target, GL_TEXTURE_MIN_FILTER, m_min_filter));
    glCheck(glTexParameterf(m_target, GL_TEXTURE_MAG_FILTER, m_mag_filter));
    glCheck(glTexParameterf(m_target, GL_TEXTURE_WRAP_S, m_wrapping));
    glCheck(glTexParameterf(m_target, GL_TEXTURE_WRAP_T, m_wrapping));
    glCheck(glTexParameterf(m_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
  }

private:

  virtual inline bool needUpdate() const override
  {
    return PendingData::hasPendingData();
  }

  virtual bool create() override
  {
    glCheck(glGenTextures(1U, &m_handle));
    return false;
  }

  virtual void release() override
  {
    glCheck(glDeleteTextures(1U, &m_handle));
  }

  virtual void activate() override
  {
    glCheck(glBindTexture(m_target, m_handle));
  }

  virtual void deactivate() override
  {
    glCheck(glBindTexture(m_target, 0U));
  }

  virtual bool setup() override
  {
    applyParam();
    return false;
  }

  float m_min_filter = GL_NEAREST;
  float m_mag_filter = GL_NEAREST;
  float m_wrapping = GL_CLAMP_TO_EDGE;
};

// **************************************************************
//!
// **************************************************************
class GLTexture2D: public IGLTexture
{
public:

  GLTexture2D()
    : IGLTexture(GL_TEXTURE_2D)
  {
  }

  GLTexture2D(std::string const& name)
    : IGLTexture(name, GL_TEXTURE_2D)
  {
  }

  virtual ~GLTexture2D()
  {
    if (likely(nullptr != m_buffer))
      {
        SOIL_free_image_data(m_buffer);
      }
  }

  inline bool loaded() const
  {
    return nullptr != m_buffer;
  }

  inline bool load(std::string const& filename, const bool rename = false)
  {
    return load(filename.c_str(), rename);
  }

  bool load(const char *const filename, const bool rename = true)
  {
    int width, height;
    bool res;

    LOGI("Loading texture '%s'", filename);
    if (nullptr != m_buffer)
      {
        SOIL_free_image_data(m_buffer);
      }
    m_buffer = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);

    // Success
    if (likely(nullptr != m_buffer))
      {
        m_width = static_cast<uint32_t>(width);
        m_height = static_cast<uint32_t>(height);
        if (rename || name().empty())
          {
            name() = filename;
            LOGI("Renaming texture '%s'", filename);
          }
        PendingData::tagAsPending(0U, m_width * m_height);
        LOGI("Successfuly load picture file '%s'", filename);
        res = true;
      }
    else
      {
        /*if (m_throw_enable)
          {
            std::string msg("Failed loading picture file '");
            msg += filename; msg += "'";
            OpenGLException e(msg);
            throw e;
          }
          else*/
          {
            LOGES("Failed loading picture file '%s'", filename);
            res = false;
          }
      }

    return res;
  }

  inline unsigned char& operator[](size_t nth)
  {
    //TBD ?
    //if (nth > m_width * m_height)
    //  {
    //    reserve(nth);
    //  }
    PendingData::tagAsPending(nth);
    return m_buffer[nth];
  }

  inline const unsigned char& operator[](size_t nth) const
  {
    return m_buffer[nth];
  }

  inline uint32_t width() const
  {
    return m_width;
  }

  inline uint32_t height() const
  {
    return m_height;
  }

private:

  virtual bool setup() override
  {
    LOGD("Texture '%s' setup", name().c_str());
    if (unlikely(!loaded()))
      return true;

    applyParam();
    glCheck(glTexImage2D(m_target, 0, m_gpu_format,
                         static_cast<GLsizei>(m_width),
                         static_cast<GLsizei>(m_height),
                         0, m_cpu_format, m_type, m_buffer));
    return false;
  }

  virtual bool update() override
  {
    LOGD("Texture '%s' update", name().c_str());
    size_t pos_start;
    size_t pos_end;
    PendingData::getPendingData(pos_start, pos_end);

    // FIXME: pour le moment on envoie toute la texture entiere
    // au lieu du rectangle modifie.
    // TODO pendingData --> x,y,width,height
    const GLint x = 0U;
    const GLint y = 0U;
    const GLsizei width = static_cast<GLsizei>(m_width);
    const GLsizei height = static_cast<GLsizei>(m_height);

    glCheck(glBindTexture(m_target, m_handle));
    glCheck(glTexSubImage2D(m_target, 0, x, y, width, height,
                            m_cpu_format, m_type, m_buffer));

    PendingData::clearPending();
    return false;
  };

  uint32_t m_width = 0;
  uint32_t m_height = 0;
  GLenum m_cpu_format = GL_RGBA;
  GLint m_gpu_format = GL_RGBA;
  GLenum m_type = GL_UNSIGNED_BYTE;
  // TODO smart pointer + deleter=SOIL_free_image_data
  unsigned char* m_buffer = nullptr;
};

// **************************************************************
//!
// **************************************************************
/*class GLTextureDepth2D: public GLTexture2D
{
  GLTextureDepth2D()
    : GLTexture2D()
  {
    m_gpu_format = GL_DEPTH_COMPONENT; // FIXME incompatible avec load() ??
    m_cpu_format = GL_DEPTH_COMPONENT;
  }

  GLTextureDepth2D(std::string const& name)
    : GLTexture2D(name)
  {
    m_gpu_format = GL_DEPTH_COMPONENT;
    m_cpu_format = GL_DEPTH_COMPONENT;
  }
  };*/

#endif /* GLTEXTURES_HPP_ */
