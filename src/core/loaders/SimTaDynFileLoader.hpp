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

#ifndef SIMTADYNFILELOADER_HPP_
#  define SIMTADYNFILELOADER_HPP_

#  include "ILoader.hpp"
#  include "SimTaDynMap.hpp"

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
class SimTaDynFileLoader : public ILoader<SimTaDynMap>
{
public:

  SimTaDynFileLoader()
    : ILoader<SimTaDynMap>("SimTaDynMap")
  {
    LOGI("Creating a SimTaDynMap loader %p", this);
  }
  virtual void loadFromFile(std::string const& filename, SimTaDynMap* &map) override;
  virtual void saveToFile(SimTaDynMap const& map, std::string const& filename) override;

protected:

  //! \brief Generate an unique name for creating a temporary
  //! directory.
  std::string generateTempDirName() const;

  //! \brief
  void unzip(std::string const &zip_file);
  bool zip(SimTaDynMap const& map, std::string const& filename);

private:

  std::string m_base_dir;
};

#endif