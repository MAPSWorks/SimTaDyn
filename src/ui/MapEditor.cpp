#include "MapEditor.hpp"
#include "ShapeFile.hpp"
#include "Names.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
MapEditor::MapEditor()
  : MapLoader(), m_listener(*this)
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
    m_submenu[1].signal_activate().connect([this](){MapEditor::newMap();});
    m_menu[simtadyn::MapMenu].append(m_submenu[1]);

    //
    m_submenu[2].set_label("Open Map");
    m_image[2].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[2].set_image(m_image[2]);
    m_submenu[2].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::openMap));
    m_menu[simtadyn::MapMenu].append(m_submenu[2]);

    //
    m_submenu[3].set_label("Save Map");
    m_image[3].set_from_icon_name("document-save", Gtk::ICON_SIZE_MENU);
    m_submenu[3].set_image(m_image[3]);
    m_submenu[3].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::saveMap));
    m_menu[simtadyn::MapMenu].append(m_submenu[3]);

    //
    m_submenu[4].set_label("Save As Map");
    m_image[4].set_from_icon_name("document-save-as", Gtk::ICON_SIZE_MENU);
    m_submenu[4].set_image(m_image[4]);
    m_submenu[4].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::saveAsMap));
    m_menu[simtadyn::MapMenu].append(m_submenu[4]);

    //
    m_menu[simtadyn::MapMenu].append(m_menuseparator[0]);

    //
    m_submenu[5].set_label("Replace Map");
    m_image[5].set_from_icon_name("document-import", Gtk::ICON_SIZE_MENU);
    m_submenu[5].set_image(m_image[5]);
    m_submenu[5].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::replaceMap));
    m_menu[simtadyn::MapMenu].append(m_submenu[5]);

    //
    m_submenu[6].set_label("Add Map");
    m_image[6].set_from_icon_name("document-import", Gtk::ICON_SIZE_MENU);
    m_submenu[6].set_image(m_image[6]);
    m_submenu[6].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::addMap));
    m_menu[simtadyn::MapMenu].append(m_submenu[6]);

    //
    m_submenu[7].set_label("Clear Map");
    m_image[7].set_from_icon_name("user_trash", Gtk::ICON_SIZE_MENU);
    m_submenu[7].set_image(m_image[7]);
    m_submenu[7].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::clearMap));
    m_menu[simtadyn::MapMenu].append(m_submenu[7]);
  }

  // Map toolbar (vertical)
  {
    m_toolbar.set_property("orientation", Gtk::ORIENTATION_VERTICAL);
    m_toolbar.set_property("toolbar-style", Gtk::TOOLBAR_ICONS);

    {
      Gtk::ToolButton *button = Gtk::manage(new Gtk::ToolButton());
      button->set_label("New");
      button->set_stock_id(Gtk::Stock::NEW);
      button->set_tooltip_text("Load and add a map to the current map");
      m_toolbar.append(*button, [this](){ MapEditor::execMap(); });
    }

    {
      Gtk::ToolButton *button = Gtk::manage(new Gtk::ToolButton());
      button->set_label("Open");
      button->set_stock_id(Gtk::Stock::NEW);
      button->set_tooltip_text("Load a map file");
      m_toolbar.append(*button, sigc::mem_fun(*this, &MapEditor::openMap));
    }

    /*{
      Gtk::ToolButton *button = Gtk::manage(new Gtk::ToolButton());
      button->set_label("Open");
      button->set_stock_id(Gtk::Stock::NEW);
      button->set_tooltip_text("+1");
      m_toolbar.append(*button, sigc::mem_fun(*this, &MapEditor::foo));
      }*/
  }

  // Pack all stuffs together
  {
    m_vbox.pack_start(m_inspector.m_scrolledwindow, Gtk::PACK_SHRINK);
    pack_start(m_vbox);
    pack_start(m_toolbar, Gtk::PACK_SHRINK);
  }
}

// *************************************************************************************************
//
// *************************************************************************************************
MapEditor::~MapEditor()
{
  LOGI("Destroying MapEditor");
  // TODO: be sure no Forth script is running on the map before destroying mapq
}

void MapEditor::closeMap()
{
  if (nullptr == m_current_map.get())
    return ;

  if (false == m_current_map.get()->dispose())
    return ;

  SimTaDynMap* map = nullptr;
  if (!m_maps.empty())
    {
      map = static_cast<SimTaDynMap*>(m_maps.begin()->second);
    }
  m_current_map.set(map);
}

bool MapEditor::selectMap(const Key id)
{
  auto it = m_maps.find(id);
  if (it != m_maps.end())
    {
      m_current_map.set(static_cast<SimTaDynMap*>(it->second));
      return true;
    }
  return false;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::load(std::string const& filename, SimTaDynMap* &map)
{
  std::string extension = File::extension(filename);

  if (nullptr == map)
    {
      map = newMap();
    }

  if (0 == extension.compare("shp"))
    {
      ShapefileLoader loader;
      return loader.load(filename, map);
    }

  m_error = "Unknown extension file";
  return false;
}

// *************************************************************************************************
//
// *************************************************************************************************
void MapEditor::doOpen(std::string const& filename, const bool new_map, const bool reset_map)
{
  SimTaDynMap* map = (new_map) ? nullptr : m_current_map.get();
  SimTaDynMap* oldmap = map;

  if ((reset_map) && (nullptr != map))
    {
      map->m_graph.BasicGraph::reset();
    }

  bool ret = load(filename, map);
  if (false == ret)
    {
      Gtk::MessageDialog d("Could not load '" + filename + "' as a map.",
                           false, Gtk::MESSAGE_WARNING);
      d.set_transient_for((Gtk::Window&) (*m_menu[simtadyn::MapMenu].get_toplevel()));
      d.set_secondary_text("Reason was: " + error());
      d.run();
    }
  else
    {
      if (reset_map)
        {
          map->m_name = File::shortName(filename);
        }
      if (nullptr == oldmap)
        {
          m_maps.insert(map);
        }
      std::cerr << "Successfully loaded map '" << map->m_name << "'" << std::endl;
      m_current_map.set(map);

      //FIXME if (bool) { selectionner toutes la map pour permettre a l'utilisateur de la placer la ou il vaut }
      //FIXME zoomer sur la fusion des deux bounding box de l'ancinne et nouvelle map }
    }
}
// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::execMap() // FIXME: Exec(typeCell, nodeID)
{
  bool res;

  SimForth &forth = SimForth::instance();
  SimTaDynNode& n1 = map()->m_graph.getNode(1); // FIXME far all cells + faire un ancetre commun aux cell pour eviter de faire un switch
  n1.forthWord("N#1 N#2 N#3 + +");

  // FIXME: should be called outside each cell: optimisation
  // Disable compilation mode
  forth.dictionary().smudge(":");
  forth.dictionary().smudge("INCLUDE");

  res = n1.interprete(forth);

  // Enable compilation mode
  forth.dictionary().smudge("INCLUDE");
  forth.dictionary().smudge(":");

  return res;
}

// *************************************************************************************************
//
// *************************************************************************************************
void MapEditor::openDialog(const bool new_map, const bool reset_map)
{
  Gtk::FileChooserDialog dialog("Choose a binary file to save Forth dictionary",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for((Gtk::Window&) (*m_menu[simtadyn::MapMenu].get_toplevel()));

  // Set to the SimTaDyn path while no longer the GTK team strategy.
  dialog.set_current_folder(Config::instance().m_data_path);

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  auto filter_shapefile = Gtk::FileFilter::create();
  filter_shapefile->set_name("Shape files");
  filter_shapefile->add_pattern("*.shp");
  dialog.add_filter(filter_shapefile);

  auto filter_simtadyn = Gtk::FileFilter::create();
  filter_simtadyn->set_name("SimTaDyn files");
  filter_simtadyn->add_pattern("*.simtadyn");
  dialog.add_filter(filter_simtadyn);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      doOpen(dialog.get_filename(), new_map, reset_map);
    }
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