//=====================================================================
// SimTaDynContext: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDynContext.
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

#ifndef SIMTADYN_CONTEXT_HPP_
#  define SIMTADYN_CONTEXT_HPP_

#  include "Path.hpp"
#  include "SimTaDynWindow.hpp"

// **************************************************************
//! \brief
// **************************************************************
class SimTaDynContext
{
public:

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  SimTaDynContext()
  {
    LOGI("New SimTaDynContext");
    ResourceManager<Key>::instance();
    LoaderManager::instance();
    SimForth::instance();
    ForthEditor::instance();
    MapEditor::instance();

    m_window = std::unique_ptr<SimTaDynWindow>(new SimTaDynWindow);
    if (nullptr == m_window)
      {
        LOGE("Failed creating the SimTaDyn main window for GUI. Aborting");
        exit(1);
      }
  }

  //------------------------------------------------------------------
  //! \brief Private because of Singleton. Check if resources is still
  //! acquired which show a bug in the management of resources.
  //------------------------------------------------------------------
  ~SimTaDynContext()
  {
    LOGI("Leaving SimTaDynContext: releasing the memory");
    ForthEditor::destroy();
    MapEditor::destroy();
    SimForth::destroy();
    LoaderManager::destroy();
    ResourceManager<Key>::destroy();
    Logger::destroy();
  };

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void init()
  {
  }

  SimTaDynWindow& window()
  {
    return *m_window;
  }

protected:

  std::unique_ptr<SimTaDynWindow> m_window;
};

#endif /* SIMTADYN_CONTEXT_HPP_ */
