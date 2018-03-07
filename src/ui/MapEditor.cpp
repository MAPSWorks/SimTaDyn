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

#include "MapEditor.hpp"
#include <functional>

// *************************************************************************************************
//
// *************************************************************************************************
MapEditor::MapEditor()
{
  LOGI("Creating MapEditor");

  // Menu '_Map'
  {
    m_menuitem[simtadyn::MapMenu].set_label("_Map");
    m_menuitem[simtadyn::MapMenu].set_use_underline(true);
    m_menuitem[simtadyn::MapMenu].set_submenu(m_menu[simtadyn::MapMenu]);

    //
    m_submenu[1].set_label("New Map");
    m_image[1].set_from_icon_name("document-new", Gtk::ICON_SIZE_MENU);
    m_submenu[1].set_image(m_image[1]);
    m_submenu[1].signal_activate().connect([this](){ MapEditor::create(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[1]);

    //
    m_submenu[2].set_label("Open Map");
    m_image[2].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[2].set_image(m_image[2]);
    m_submenu[2].signal_activate().connect([this](){ MapEditor::open(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[2]);

    //
    m_submenu[3].set_label("Save Map");
    m_image[3].set_from_icon_name("document-save", Gtk::ICON_SIZE_MENU);
    m_submenu[3].set_image(m_image[3]);
    m_submenu[3].signal_activate().connect([this](){ MapEditor::save(false); });
    m_menu[simtadyn::MapMenu].append(m_submenu[3]);

    //
    m_submenu[4].set_label("Save As Map");
    m_image[4].set_from_icon_name("document-save-as", Gtk::ICON_SIZE_MENU);
    m_submenu[4].set_image(m_image[4]);
    m_submenu[4].signal_activate().connect([this](){ MapEditor::saveAs(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[4]);

    //
    m_menu[simtadyn::MapMenu].append(m_menu_separator[0]);

    //
    m_submenu[5].set_label("Replace Map");
    m_image[5].set_from_icon_name("document-import", Gtk::ICON_SIZE_MENU);
    m_submenu[5].set_image(m_image[5]);
    m_submenu[5].signal_activate().connect([this](){ MapEditor::replace(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[5]);

    //
    m_submenu[6].set_label("Add Map");
    m_image[6].set_from_icon_name("document-import", Gtk::ICON_SIZE_MENU);
    m_submenu[6].set_image(m_image[6]);
    m_submenu[6].signal_activate().connect([this](){ MapEditor::addMap(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[6]);

    //
    m_submenu[7].set_label("Clear Map");
    m_image[7].set_from_icon_name("user_trash", Gtk::ICON_SIZE_MENU);
    m_submenu[7].set_image(m_image[7]);
    m_submenu[7].signal_activate().connect([this](){ MapEditor::clear(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[7]);
  }

  // Pack all stuffs together
  {
    m_vbox.pack_start(m_inspector.m_scrolledwindow, Gtk::PACK_SHRINK);
    m_hbox.pack_start(m_vbox);
    m_hbox.pack_start(toolbar(), Gtk::PACK_SHRINK);
  }
}

// *************************************************************************************************
//
// *************************************************************************************************
MapEditor::~MapEditor()
{
  LOGI("Destroying MapEditor");
  close();// FIXME save(true);

  // TODO: be sure no Forth script is running on the map before destroying mapq
}

// *************************************************************************************************
//!
// *************************************************************************************************
bool MapEditor::close()
{
  bool res = save(true);

  // Get the previous map
  usePreviousMap();
  return res;
}

// *************************************************************************************************
//!
// *************************************************************************************************
bool MapEditor::save(const bool closing)
{
  // No map currently used or already saved.
  if ((nullptr == m_map) || (!m_map->modified()))
    return true;

  // Ask the user for saving it or not before closing for modified map
  // or newly created.
  return popupAskForSaving(m_map, closing);
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::dialogLoadMap(const bool new_map, const bool reset_map)
{
  Gtk::FileChooserDialog dialog("Load a SimTaDyn map", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(window());
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Open the dialog window and set the SimTaDyn path as current
  // folder instead of using the "smart-current-folder" strategy
  // thiugh by GTK+ developpers.
  dialog.set_current_folder(config::data_path);

  // Fill filters for selecting type of file. Use the loader manager
  // for filling these filters because its knows all loaders which
  // know file extensions they can load.
  for (auto it: LoaderManager::instance().loaders<SimTaDynMap>())
    {
      auto filter = Gtk::FileFilter::create();
      // loaders() return a map <<file extension>, <loader>>
      std::string extension("*." + it.first);
      filter->add_pattern(extension);
      filter->set_name(it.second->description() + " (" + extension + ')');
      dialog.add_filter(filter);
    }

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      doOpen(dialog.get_filename(), new_map, reset_map);
    }
  return false;
}

bool MapEditor::popupAskForSaving(SimTaDynMapPtr map, const bool closing)
{
  assert(nullptr != map);
  Gtk::MessageDialog dialog(window(), "The document '" + map->name() +
                            "' has been modified. Do you want to save it now ?",
                            false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
  dialog.add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_APPLY);

  int result = dialog.run();
  if (Gtk::RESPONSE_YES == result)
    {      bool newly_created = (m_map->m_zip_path == "") ||
        (m_map->m_base_dir == "") || (m_map->m_full_path == "");

      // SaveAs or save
      if (newly_created)
        {
          return dialogSaveAsMap(m_map);
        }
      else
        {
          return doSave(m_map->m_full_path, m_map);
        }
    }
  else if (Gtk::RESPONSE_APPLY == result)
    {
      return dialogSaveAsMap(m_map);
    }
  else // other button
    {
      if (closing)
        {
          //FIXME m_map->modified(false);
          return true;
        }
      return !m_map->modified();
    }
}

// *************************************************************************************************
// FIXME a la place de LoaderManager::instance().loaders<SimTaDynMap>()
// Templater la methode puis mettre LoaderManager::instance().loaders<T>()
// Comme ca on fait save et export
// *************************************************************************************************
bool MapEditor::dialogSaveAsMap(SimTaDynMapPtr map)
{
  Gtk::FileChooserDialog dialog("Save a SimTaDyn map", Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(window());
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);

  // Open the dialog window and set the SimTaDyn path as current
  // folder instead of using the "smart-current-folder" strategy
  // thiugh by GTK+ developpers.
  dialog.set_current_folder(config::data_path);

  // Fill filters for selecting type of file. Use the loader manager
  // for filling these filters because its knows all loaders which
  // know file extensions they can load.
  for (auto it: LoaderManager::instance().loaders<SimTaDynMap>())
    {
      auto filter = Gtk::FileFilter::create();
      // loaders() return a map <<file extension>, <loader>>
      std::string extension("*." + it.first);
      filter->add_pattern(extension);
      filter->set_name(it.second->description() + " (" + extension + ')');
      dialog.add_filter(filter);
    }

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      return doSave(dialog.get_filename(), map);
    }
  return false;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::doSave(std::string const& filename, SimTaDynMapPtr map)
{
  try
    {
      // FIXME: manque le renommage de la carte si SaveAs
      LoaderManager::instance().saveToFile(map, filename);
    }
  catch (LoaderException const &e)
    {
      saved_failure.emit(filename, e.message());
      Gtk::MessageDialog dialog(window(), e.what(), false, Gtk::MESSAGE_WARNING);
      dialog.set_secondary_text("Could not save '" + filename + "' as a SimTaDyn map. Reason: " +
                                e.message());
      dialog.run();
      return false;
    }
  catch (std::exception const &e)
    {
      saved_failure.emit(filename, e.what());
      Gtk::MessageDialog dialog(window(), e.what(), false, Gtk::MESSAGE_WARNING);
      dialog.set_secondary_text("Could not save '" + filename + "' as a SimTaDyn map.");
      dialog.run();
      return false;
    }

  return true;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::doOpen(std::string const& filename, const bool new_map, const bool reset_map)
{
  std::string name = (reset_map) ? File::baseName(filename) : filename;
  SimTaDynMapPtr map = (new_map) ? SimTaDynMapManager::instance().create(name) : getMap();

  if ((reset_map) && (nullptr != map))
    {
      map->clear();
    }

  bool res = load(filename, map);
  if (true == res)
    {
      //FIXME if (bool) { selectionner toutes la map pour permettre a l'utilisateur de la placer la ou il vaut }
      //FIXME zoomer sur la fusion des deux bounding box de l'ancinne et nouvelle map }
    }

  return res;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::load(std::string const& filename, SimTaDynMapPtr map)
{
  SimTaDynMapManager &rm = SimTaDynMapManager::instance();

  try
    {
      bool dummy_map = (nullptr == map);
      if ((dummy_map) || (false == rm.exist(map->name())))
        {
          LoaderManager::instance().loadFromFile(filename, map);
          if (dummy_map)
            {
              add(map);
              loaded_success.emit(map);
            }
          else
            {
              map->m_signal_map_modified.emit(map);
            }
        }
    }
  catch (LoaderException const &e)
    {
      loaded_failure.emit(filename, e.message());
      Gtk::MessageDialog dialog(window(), e.what(), false, Gtk::MESSAGE_WARNING);
      dialog.set_secondary_text("Could not load '" + filename + "' as a SimTaDyn map. Reason: "
                                + e.message());
      dialog.run();
      return false;
    }
  catch (std::exception const &e)
    {
      loaded_failure.emit(filename, e.what());
      Gtk::MessageDialog dialog(window(), e.what(), false, Gtk::MESSAGE_WARNING);
      dialog.set_secondary_text("Could not load '" + filename + "' as a SimTaDyn map.");
      dialog.run();
      return false;
    }

  return true;
}

// *************************************************************************************************
//
// *************************************************************************************************
void MapEditor::add(SimTaDynMapPtr p)
{
  if (nullptr == p)
    return ;

  if (SimTaDynMapManager::instance().add(p->name(), p, false))
    {
      use(p);
    }
  else
    {
      // Failed. Ask the user to replace the map
      Gtk::MessageDialog dialog(window(), "ResourceManager", false,
                                Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
      dialog.set_secondary_text("Warning the SimTaDyn map '" + p->name() +
                                "' already exists. Replace it ?");
      int result = dialog.run();
      if (Gtk::RESPONSE_YES == result)
        {
          SimTaDynMapManager::instance().add(p->name(), p, true);
          use(p);
        }
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::exec() // FIXME: Exec(typeCell, nodeID)
{
  SimForth &forth = SimForth::instance();
  SimTaDynSheet *sheet = nullptr;// FIXME map()->sheet();

  if (nullptr == sheet)
  {
    LOGE("No sheet attached to Forth");
    return false;
  }

  // FIXME: should be called outside each cell: optimisation
  // Disable compilation mode
  forth.dictionary().smudge(":");
  forth.dictionary().smudge("INCLUDE");

  sheet->parse(forth);
  std::pair<bool, std::string> res = sheet->evaluate(forth);
  forth.ok(res);

  // Enable compilation mode
  forth.dictionary().smudge("INCLUDE");
  forth.dictionary().smudge(":");

  return res.first;
}

// **************************************************************
// Interface
// **************************************************************
/*Gtk::ToolButton *MapEditor::addButon(const Gtk::BuiltinStockID icon,
                                     const std::string &script,
                                     const std::string &help)
{
  return addForthButon(MapToolbar, icon, script, help);
  }*/
