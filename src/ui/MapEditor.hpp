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

#  include "Names.hpp"
#  include "SimTaDynLoaders.hpp"
#  include "Inspector.hpp"
#  include "DrawingArea.hpp"
#  include "MapEditorCommands.hpp"
#  include "SimTaDynMapHolder.hpp"

// *************************************************************************************************
// This class implements a Controler pattern of the Model-View-Controler (MVC) design pattern.
// *************************************************************************************************
class MapEditor
  : //private IEditor,
    public Singleton<MapEditor>,
    public MapEditorCommands,
    public SimTaDynMapHolder
{
private:

  friend class Singleton<MapEditor>;

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  MapEditor();

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  virtual ~MapEditor();

public:

  //------------------------------------------------------------------
  //! \brief Return the widget to be attached to a GTK widget.
  //------------------------------------------------------------------
  inline Gtk::Widget &widget()
  {
    return m_hbox;
  }

  //------------------------------------------------------------------
  //! \brief Wrap SimTaDynMapHolder::use() and popup a dialog if
  //! the map is not known.
  //------------------------------------------------------------------
  inline SimTaDynMapPtr map(std::string const& name)
  {
    if (nullptr == use(name))
      {
        Gtk::MessageDialog dialog(window(), "Not in ResourceManager", false, Gtk::MESSAGE_WARNING);
        dialog.set_secondary_text("SimTaDyn map '" + name + "' Cannot be used as current SimTaDyn map");
        dialog.run();
      }

    return getMap();
  }

  //------------------------------------------------------------------
  //! \brief Reset the map. Delete contents on spreadsheets.
  //! FIXME all sheets or the just the current one ?
  //! \signal emit m_signal_map_changed<void, SimTaDynMapPtr>.
  //------------------------------------------------------------------
  inline void clear()
  {
    SimTaDynMapPtr map = getMap();
    if (nullptr != m_map)
      {
        m_map->clear();
      }
  }

  //------------------------------------------------------------------
  //! \brief Load a new SimTaDyn map from a file. The file is choosen
  //! through a dialog box.
  //! \signal emit m_signal_map_changed<void, SimTaDynMapPtr>.
  //------------------------------------------------------------------
  inline bool open()
  {
    return dialogLoadMap(false, true);
  }

  //------------------------------------------------------------------
  //! \brief Load a new map from the given file.
  //! \signal emit m_signal_map_changed<void, SimTaDynMapPtr>.
  //------------------------------------------------------------------
  inline bool open(std::string const& filename)
  {
    return doOpen(filename, false, true);
  }

  //------------------------------------------------------------------
  //! \brief Close the current map, ask the user if he wants to save its
  //! document and finaly set the previous map as current document (if
  //! present).
  //------------------------------------------------------------------
  bool close();

  //------------------------------------------------------------------
  //! \brief Save the map in a desired file. The file is choosen
  //! through a dialog box.
  //------------------------------------------------------------------
  inline bool saveAs()
  {
    SimTaDynMapPtr map = getMap();
    if (nullptr == map)
      return true;

    return dialogSaveAsMap(map);
  }

  //------------------------------------------------------------------
  //! \brief Save the current document. Depending on its state the
  //! document is directly saved or a "Save As" dialog box is opened.
  //------------------------------------------------------------------
  bool save(const bool closing);

  //------------------------------------------------------------------
  //! \brief TODO
  //------------------------------------------------------------------
  //template<class T>
  //inline bool export(SimTaDynMapPtr map)
  //{
  //  return dialogSaveAsMap<T>(map);
  //}

  //------------------------------------------------------------------
  //! \brief Load a new map from a file through a dialog box.
  //! \signal emit m_signal_map_changed<void, SimTaDynMapPtr>.
  //------------------------------------------------------------------
  inline bool import()
  {
    return dialogLoadMap(true, false); // FIXME doit etre templater
  }

  //------------------------------------------------------------------
  //! \brief Load a new map from a file through a dialog box.
  //! \signal emit m_signal_map_changed<void, SimTaDynMapPtr>.
  //------------------------------------------------------------------
  inline bool import(std::string const& filename)
  {
    return doOpen(filename, true, false);
  }

  //------------------------------------------------------------------
  //! \brief Load a map from a file in the current map.
  //! \signal emit m_signal_map_changed<void, SimTaDynMapPtr>.
  //------------------------------------------------------------------
  inline bool addMap() // FIXME: addSheet
  {
    return dialogLoadMap(false, false);
  }

  //------------------------------------------------------------------
  //! \brief Load a map from a file in the current map.
  //! \signal emit m_signal_map_changed<void, SimTaDynMapPtr>.
  //------------------------------------------------------------------
  inline bool addMap(std::string const& filename)
  {
    return doOpen(filename, false, false);
  }

  //------------------------------------------------------------------
  //! \brief Load a map and replace the current map.
  //! \signal emit m_signal_map_changed<void, SimTaDynMapPtr>.
  //------------------------------------------------------------------
  inline bool replace() // FIXME: sheet or map ?
  {
    return dialogLoadMap(false, true);
  }

  //------------------------------------------------------------------
  //! \brief Load a map and replace the current map.
  //! \signal emit m_signal_map_changed<void, SimTaDynMapPtr>.
  //------------------------------------------------------------------
  inline bool replace(std::string const& filename)
  {
    return doOpen(filename, false, true);
  }

  //------------------------------------------------------------------
  //------------------------------------------------------------------
  bool exec();

  //------------------------------------------------------------------
  //! FIXME \brief Attach a the MVC view to this MVC controller.
  //------------------------------------------------------------------
  void attachView(GLDrawingArea& drawing_area)
  {
    m_drawing_area = &drawing_area;
    //m_drawing_area->attachController(this);
    m_vbox.pack_start(drawing_area);
  }

protected:

  //------------------------------------------------------------------
  //! \brief Return the main window needed for gtk dialogs.
  //------------------------------------------------------------------
  inline Gtk::Window &window()
  {
    return (Gtk::Window&) *(widget().get_toplevel());
  }

  //------------------------------------------------------------------
  //! \brief Open a folder chooser dialog for letting the user choosing
  //! the file to be loaded.
  //------------------------------------------------------------------
  bool dialogLoadMap(const bool new_map, const bool reset_map);

  //------------------------------------------------------------------
  //! \brief Call the loader for loading a document.
  //------------------------------------------------------------------
  bool load(const std::string& filename, SimTaDynMapPtr oldmap);

  //------------------------------------------------------------------
  //! \brief Perform operations before and after calling load().
  //------------------------------------------------------------------
  bool doOpen(std::string const& filename, const bool new_map, const bool reset_map);

  //------------------------------------------------------------------
  //! \brief Call the loader for saving the document.
  //------------------------------------------------------------------
  bool doSave(std::string const& filename, SimTaDynMapPtr map);

  //------------------------------------------------------------------
  //! \brief Open a folder chooser dialog for letting the user choosing
  //! the file path for saving its document.
  //------------------------------------------------------------------
  bool dialogSaveAsMap(SimTaDynMapPtr map);

  //------------------------------------------------------------------
  //! \brief Popup for asking the user if he wants to save its document.
  //! \param closing set it to true when the user click on the close
  //! button.
  //------------------------------------------------------------------
  bool popupAskForSaving(SimTaDynMapPtr map, const bool closing);

  //------------------------------------------------------------------
  //! \brief Create a dummy map and set it as current map.
  //------------------------------------------------------------------
  SimTaDynMapPtr create();

  //------------------------------------------------------------------
  //! \brief Add a map as resource in the ResourceManager
  //------------------------------------------------------------------
  void add(const Key name, SimTaDynMapPtr map);

  //------------------------------------------------------------------
  //! \brief Remove a map as resource from the ResourceManager
  //------------------------------------------------------------------
  void remove(const Key name);

public:

  //! \brief View implementation of the MVC
  GLDrawingArea         *m_drawing_area = nullptr;
  //!
  Gtk::MenuItem          m_menuitem[simtadyn::MaxMapMenuNames + 1];

  sigc::signal<void, SimTaDynMapPtr> loaded_success;
  sigc::signal<void, const std::string &, const std::string &> loaded_failure;
  sigc::signal<void, SimTaDynMapPtr> saved_success;
  sigc::signal<void, const std::string &, const std::string &> saved_failure;

protected:

  Gtk::Menu              m_menu[simtadyn::MaxMapMenuNames + 1];
  Gtk::ImageMenuItem     m_submenu[8];
  Gtk::Image             m_image[8];
  Gtk::SeparatorMenuItem m_menu_separator[2];
  Gtk::SeparatorToolItem m_toolbar_separator[2];
  Inspector              m_inspector;
  Gtk::VBox              m_vbox;
  Gtk::HBox              m_hbox;
};

#endif /* MAPEDITOR_HPP_ */
