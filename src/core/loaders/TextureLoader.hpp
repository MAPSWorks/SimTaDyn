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

#ifndef TEXTURELOADER_HPP_
#  define TEXTURELOADER_HPP_

#  include "ILoader.tpp"
#  include "GLTextures.hpp"

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
class TextureLoader
  : public ILoader<GLTexture2D>
{
public:

  TextureLoader()
    : ILoader<GLTexture2D>("OpenGL Texture")
  {
    LOGI("Creating an OpenGL loader %p", this);
  }
  virtual void loadFromFile(std::string const& filename, GLTexture2D* &texture) override;
};

#endif
