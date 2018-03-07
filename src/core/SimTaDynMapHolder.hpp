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

#ifndef MAP_HOLDER_HPP_
#  define MAP_HOLDER_HPP_

// *************************************************************************************************
//! \brief A class holding the currently edited SimTaDynMap by the MapEditor. This class allows to
//! trig events to observer when the user swap of map for a new one.
// ***********************************************************************************************
class SimTaDynMapHolder
{
public:

  // TODO: notify to SimForth to get the address of the scenegraph<SimTaDynSheet>
  //! \brief pass nullptr for closing
  void use(SimTaDynMapPtr p)
  {
    if (m_map == p)
      return ;

    m_signal_map_unselected.emit(m_map);
    m_map = p;
    if (nullptr != m_map)
      {
        m_map->m_signal_map_modified.connect(
            sigc::mem_fun(*this, &SimTaDynMapHolder::onMapModified));
        m_signal_map_selected.emit(m_map);
      }
  }

  //! \brief Acquire the map from the resource manager
  SimTaDynMapPtr use(std::string const& name)
  {
    SimTaDynMapPtr new_map = SimTaDynMapManager::instance().acquire(name);
    if (nullptr == new_map)
      {
        LOGE("Cannot select SimTaDyn map #%s because it does not exist",
             name.c_str());
        return nullptr;
      }

    use(new_map);
    return m_map;
  }

  //! \brief Create and acquire a new map
  inline SimTaDynMapPtr create(std::string const& name)
  {
    SimTaDynMapPtr map = SimTaDynMapManager::instance().create(name);
    if (nullptr != map)
      {
        use(map);
      }

    return map;
  }

  //! \brief Return the current map
  inline SimTaDynMapPtr getMap() // FIXME: map() ne marche pas
  {
    return m_map;
  }

  //! \brief Return the previous map
  inline void usePreviousMap()
  {
    use(SimTaDynMapManager::instance().acquire());
  }

  void onMapModified(SimTaDynMapPtr p)
  {
    m_signal_map_modified.emit(p);
  }

public:

  //! \brief Signal sending the newly effective map (currently edited).
  //! to observer (like Renderer for forcing the map to be drawn).
  sigc::signal<void, SimTaDynMapPtr> m_signal_map_selected;

  //! \brief Signal sending the closing map.
  sigc::signal<void, SimTaDynMapPtr> m_signal_map_unselected;

  sigc::signal<void, SimTaDynMapPtr> m_signal_map_modified;

protected:

  //! \brief The SimTaDyn map currently edited.
  SimTaDynMapPtr m_map = nullptr;
};

#endif /* MAP_HOLDER_HPP_ */
