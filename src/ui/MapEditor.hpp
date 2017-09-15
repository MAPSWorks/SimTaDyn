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

#ifndef MAPEDITOR_HPP_
#  define MAPEDITOR_HPP_

//#  include "Names.hpp"
#  include "SimTaDynLoaders.hpp"
#  include "Inspector.hpp"
#  include "DrawingArea.hpp"

class LoaderManager;

// ***********************************************************************************************
//! \brief A class holding the currently edited SimTaDynMap. When
//! the user changes of map, this class will notifies to observers that
//! map changed.
// ***********************************************************************************************
class SimTaDynMapHolder
{
public:

  SimTaDynMapHolder()
  {
    m_map = nullptr;
  }

  void set(SimTaDynMap* p)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_map != p)
      {
        ResourceManager &rm = ResourceManager::instance();
        const std::string sid = std::to_string(*m_map);
        if (nullptr != m_map)
          {
            //FIXME MapEditor::save();
            rm.dispose<SimTaDynMap>(sid);
          }
        m_map = p;
        if (nullptr != p)
          {
            SimTaDynMap *res = rm.acquire<SimTaDynMap>(sid);
            assert(nullptr != res);
            p->notify();
            // TODO ---> DrawingArea::onNotify(){>attachModel(*map);}
          }
      }
  }

  SimTaDynMap* get(Key const id)
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    const std::string sid = std::to_string(id);

    // The wanted map is already the current map
    if ((nullptr != m_map) && (std::to_string(*m_map) == sid))
      return m_map;

    // Get the desired map
    ResourceManager &rm = ResourceManager::instance();
    SimTaDynMap *new_map = rm.acquire<SimTaDynMap>(sid);

    // The desired map does not exist
    if (nullptr == new_map)
      {
        if (nullptr == m_map)
          {
            LOGE("Cannot select SimTaDyn map #%u because there is no map to select", id);
          }
        else
          {
            LOGE("Cannot select SimTaDyn map #%u. Keep using the current map #%u", id, *m_map);
          }
        return m_map;
      }

    // The desired map exist, close the current map (if opened)
    if (nullptr != m_map)
      {
        rm.dispose<SimTaDynMap>(std::to_string(*m_map));     //FIXME MapEditor::save();
      }

    m_map = new_map;
    m_map->notify();
    // TODO ---> DrawingArea::onNotify(){>attachModel(*map);}
    return m_map;
  }

  SimTaDynMap* get()
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_map;
  }

protected:

  SimTaDynMap* m_map;
  std::mutex m_mutex;
};

// *************************************************************************************************
// This class implements a Controler pattern of the Model-View-Controler (MVC) design pattern.
// *************************************************************************************************
class MapEditor
  : public Singleton<MapEditor>
{
private:

  friend class Singleton<MapEditor>;

  //! \brief Private because of Singleton.
  MapEditor();

  //! \brief Private because of Singleton.
  virtual ~MapEditor();

protected:

  // ***********************************************************************************************
  //! \brief Implements the SimTaDynMap::ISimTaDynMapListener interface.
  // ***********************************************************************************************
  class SimTaDynMapListener: public ISimTaDynMapListener
  {
  public:

    //! \brief
    SimTaDynMapListener(MapEditor &editor)
      : m_editor(editor)
    {
    };

    //! \brief Callback when the map changed: draw it.
    virtual void onChanged(SimTaDynMap* map) override
    {
      LOGI("MapEditor::SimTaDynMapListener has detected that Map %s has changed 0x%x",
           map->m_name.c_str(), map);
      if (nullptr == m_editor.m_drawing_area)
        return ;
      //FIXME m_editor.m_drawing_area->drawThat(*map);
    }

    MapEditor& m_editor;
  };

public:

  inline Gtk::Widget &widget()
  {
    return m_hbox;
  }

  //! \brief Return the current map
  inline SimTaDynMap* map()
  {
    return m_current_map.get();
  }
  SimTaDynMap* map(const Key id);

  //! \brief Load a new SimTaDyn map from a file through a dialog box.
  inline bool open()
  {
    return dialogLoadMap(false, true);
  }

  //! \brief Load a new map from a file.
  inline bool open(std::string const& filename)
  {
    return doOpen(filename, false, true);
  }

  //! \brief Load a new map from a file through a dialog box.
  inline bool import()
  {
    return dialogLoadMap(true, false);
  }

    //! \brief Load a new map from a file through a dialog box.
  inline bool import(std::string const& filename)
  {
    return doOpen(filename, true, false);
  }

  //! \brief Create a dummy map.
  void newMap();

  //! \brief Load a map from a file in the current map.
  inline bool addMap()
  {
    return dialogLoadMap(false, false);
  }

  //! \brief Load a map from a file in the current map.
  inline bool addMap(std::string const& filename)
  {
    return doOpen(filename, false, false);
  }

  //! \brief Reset the map (suppres everything)
  inline void clear()
  {
    if (nullptr != m_current_map.get())
      {
        m_current_map.get()->clear();
      }
  }

  //! \brief Close the current map and activate the previous one (if
  //! present)
  void close();
  void save() {} //TODO
  void saveAs();
  bool exec();

  //! \brief Attach a the MVC view to this MVC controller.
  void attachView(GLDrawingArea& drawing_area)
  {
    m_drawing_area = &drawing_area;
    //m_drawing_area->attachController(this);
    m_vbox.pack_start(drawing_area);
  }

protected:

  virtual bool load(const std::string& filename, SimTaDynMap* &oldmap);
  bool doOpen(std::string const& filename, const bool new_map, const bool reset_map);
  bool dialogLoadMap(const bool new_map, const bool reset_map);
  bool dialogSaveAsMap(const bool closing);
  void add(const Key id, SimTaDynMap* map);
  void remove(const Key id);

public:

  //! \brief Current model of the MVC design pattern
  SimTaDynMapHolder     m_current_map;
  //! \brief View implementation of the MVC
  GLDrawingArea         *m_drawing_area = nullptr;
  //!
  Gtk::MenuItem          m_menuitem[simtadyn::MaxMapMenuNames + 1];

protected:

  Gtk::Menu              m_menu[simtadyn::MaxMapMenuNames + 1];
  Gtk::ImageMenuItem     m_submenu[8];
  Gtk::Image             m_image[8];
  Gtk::SeparatorMenuItem m_menuseparator[2];
  Gtk::SeparatorToolItem m_separator[2];
  Gtk::Toolbar           m_toolbar;
  Inspector              m_inspector;
  Gtk::VBox              m_vbox;
  Gtk::HBox              m_hbox;
  SimTaDynMapListener    m_listener;
};

#endif /* MAPEDITOR_HPP_ */
