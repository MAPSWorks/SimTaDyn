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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef TEXTURE_FILE_LOADER_HPP_
#  define TEXTURE_FILE_LOADER_HPP_

#  include "ILoader.tpp"
#  include "OpenGL.hpp"
#  include "ResourceManager.tpp"

using GLTexture2DPtr = std::shared_ptr<GLTexture2D>;
using GLTexture2DManager = ResourceManager<GLTexture2D, std::string>;

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
class TextureLoader
  : public ILoader<GLTexture2D>
{
public:

  TextureLoader()
    : ILoader<GLTexture2D>("OpenGL 2D Texture")
  {
    LOGI("Creating an OpenGL loader %p", this);
  }
  virtual void loadFromFile(std::string const& filename, GLTexture2DPtr &texture) override;
};

#endif
