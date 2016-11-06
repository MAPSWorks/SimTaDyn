#include "SimTaDynWindow.hpp"
#include "SimTaDynContext.hpp"

// *************************************************************************************************
// When terminating the SimTaDyn application
// *************************************************************************************************
bool SimTaDynWindow::quitting()
{
  std::cout << "SimTaDynWindow said: 'bye bye'" << std::endl;
  return false;
}

// *************************************************************************************************
// SimTaDyn main window
// *************************************************************************************************
SimTaDynWindow::SimTaDynWindow(const std::string& title)
  : Gtk::Window(),
    m_fortheditor(m_textview[0], m_statusbar[0], m_textview[1], false)
{
#warning "Bug: constructor tres tres lent"
  std::cout << "SimTaDynWindow::SimTaDynWindow\n";
  set_title(title);
  set_default_size(1024, 800);
  set_position(Gtk::WIN_POS_CENTER);

  // FIXME: m_simForth qui contient un bout de fenetre menu/notebook/statusbar ...
  // comme ca on pourra detacher et ajouter de nouvelles fenetres ?

  // Menus:
  // * _Map: Import/export/save/load/... geographic maps.
  // * _Forth: Import/export/save/load/... Forth scripts
  // * _Plugins: TBD: Let the user to add an menu calling it's on fprth scripts.
  // * _Help: TBD: add About/help/interactive tutorials
  {
    m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Map", m_menu[0]));
    m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Forth", m_menu[1]));
    m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Plugins", m_menu[2]));
    m_menubar.items().push_back(Gtk::Menu_Helpers::MenuElem("_Help", m_menu[3]));
  }

  // Submenus 'Forth'
  {
    Gtk::Menu::MenuList& menulist = m_menu[1].items();

    m_menuimage[3].set(Gtk::Stock::NEW, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_New File",
       m_menuimage[3], sigc::mem_fun(m_fortheditor, &ForthEditor::newEmptyDocument)));

    m_menuimage[4].set(Gtk::Stock::NEW, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_New Template",
       m_menuimage[4], sigc::mem_fun(*this, &SimTaDynWindow::addTemplateTab))); // TODO

    m_menuimage[6].set(Gtk::Stock::NEW, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_New Prompt",
       m_menuimage[6], sigc::mem_fun(m_fortheditor, &ForthEditor::newEmptyDocument))); // FIXME TBD TODO

    menulist.push_back(separator[0]);

    m_menuimage[0].set(Gtk::Stock::OPEN, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_Open",
       m_menuimage[0], sigc::mem_fun(m_fortheditor, &ForthEditor::newDocument)));

    m_menuimage[1].set(Gtk::Stock::SAVE_AS, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_SaveAs",
       m_menuimage[1], sigc::mem_fun(m_fortheditor, &ForthEditor::saveAsCurrentDocument)));

    m_menuimage[2].set(Gtk::Stock::SAVE, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_Save",
       m_menuimage[2], sigc::mem_fun(m_fortheditor, &ForthEditor::saveCurrentDocument)));

    menulist.push_back(separator[1]);

    m_menuimage[5].set(Gtk::Stock::FIND, Gtk::ICON_SIZE_MENU);
    menulist.push_back(Gtk::Menu_Helpers::ImageMenuElem("_Find",
       m_menuimage[5], sigc::mem_fun(m_fortheditor, &ForthEditor::find)));
  }

  // Map toolbar
  {
    //m_toolbar[0].append(m_toolbutton[0]);
    //m_toolbar[0].append(m_separator[0]);
  }

  // Forth toolbar
  {
    m_toolbutton[0].set_label("New");
    m_toolbutton[0].set_stock_id(Gtk::Stock::NEW);

    m_toolbutton[1].set_label("Exec");
    m_toolbutton[1].set_stock_id(Gtk::Stock::EXECUTE);

    m_toolbar[1].append(m_toolbutton[0], sigc::mem_fun(m_fortheditor, &ForthEditor::newDocument));
    m_toolbar[1].append(m_separator[1]);
    m_toolbar[1].append(m_toolbutton[1], sigc::mem_fun(m_fortheditor, &ForthEditor::execForth));
  }

  // Horizontal split
  {
    add(m_hpaned[0]);
    m_hpaned[0].set_position(800);
    m_hpaned[0].pack1(m_drawing_area); // FIXME ajouter l'editeur de proprietes des cellules sous la map
    m_hpaned[0].pack2(m_hbox[0]);
  }

  // Rigth side
  {
    m_hbox[0].pack_start(m_toolbar[0], Gtk::PACK_SHRINK);
    m_hbox[0].pack_start(m_vpaned[0]);
    m_vpaned[0].set_position(350);

    m_vpaned[0].pack1(m_vbox[0]);
    m_vbox[0].pack_start(m_menubar, Gtk::PACK_SHRINK);
    m_vbox[0].pack_start(m_fortheditor.m_notebook, Gtk::PACK_EXPAND_WIDGET);

    m_vpaned[0].pack2(m_vbox[1]);
    m_vbox[1].pack_start(m_toolbar[1], Gtk::PACK_SHRINK);
    m_vbox[1].pack_start(m_notebook[0], Gtk::PACK_EXPAND_WIDGET);
    m_vbox[1].pack_start(m_statusbar[0], Gtk::PACK_SHRINK);

    m_scrolledwindow[0].add(m_textview[0]);
    m_scrolledwindow[0].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolledwindow[1].add(m_textview[1]);
    m_scrolledwindow[1].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolledwindow[2].add(m_textview[2]);
    m_scrolledwindow[2].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolledwindow[3].add(m_textview[3]);
    m_scrolledwindow[3].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  }

  // Forth notebooks
  {
    m_notebook[0].append_page(m_scrolledwindow[0], "Result");
    m_notebook[0].append_page(m_scrolledwindow[1], "Historic");
    m_notebook[0].append_page(m_scrolledwindow[2], "Debug");
    m_notebook[0].append_page(m_scrolledwindow[3], "Dico");
  }

  // Statusbar
  {
    m_statusbar[0].push("Welcome to SimTaDyn !");
  }

  // OpenGL area
  {
    add_events(Gdk::KEY_RELEASE_MASK);
    signal_key_press_event().connect_notify(sigc::mem_fun(*this, &SimTaDynWindow::onKeyPressed));
    signal_key_release_event().connect_notify(sigc::mem_fun(*this, &SimTaDynWindow::onKeyReleased));
    //signal_scroll_event().connect(sigc::mem_fun(*this, &SimTaDynWindow::on_scroll_event));
  }

  // When terminating the SimTaDyn application
  Gtk::Main::signal_quit().connect(sigc::mem_fun(this, &SimTaDynWindow::quitting));

  show_all_children();
}

#if 0
// Mouse button pressed : process mouse button event
bool SimTaDynWindow::on_button_press_event(GdkEventButton *event)
{
  // Check if the event is a left button click.
  if (1 == event->button)
    {
      // Memorize pointer position
      lastXMouse = event->x;
      lastYMouse = event->y;
      // Start moving the view
      moveFlag = true;
      // Event has been handled
      return true;
    }

#if 0
  // Check if the event is a right button click.
  if (3 == event->button)
    {
      // Memorize mouse coordinates
      lastXMouse = event->x;
      lastYMouse = event->y;
      // Display the popup menu
      m_Menu_Popup.popup(event->button, event->time);
      // The event has been handled.
      return true;
    }
#endif

  // Event has not been handled
  return false;
}

// Mouse button released : process mouse button event
bool GlDrawingArea::on_button_release_event(GdkEventButton *event)
{
  // Check if it is the left button
  if ((1 == event->button) && moveFlag)
    {
      // End of motion
      moveFlag = false;
      // Update image focus
      imgFocusX -= (event->x - lastXMouse) / scale;
      imgFocusY -= (event->y - lastYMouse) / scale;
      return true;
    }
  // Event has been handled
  return false;
}

// Mouse pointer moved : process event
bool GlDrawingArea::on_motion_notify_event(GdkEventMotion*event)
{
  // If the left button is pressed, move the view
  if (moveFlag)
    {
      // Get mouse coordinates
      int XMouse = event->x;
      int YMouse = event->y;

      // Update image focus
      imgFocusX -= (XMouse - lastXMouse) / scale;
      imgFocusY -= (YMouse - lastYMouse) / scale;

      // Memorize new position of the pointer
      lastXMouse = XMouse;
      lastYMouse = YMouse;

      // Update view
      camera.moveAt(imgFocusX, imgFocusY);
      applyViewport();
      return true;
    }
  // Event has been handled
  return false;
}
#endif

void SimTaDynWindow::onKeyPressed(GdkEventKey* evenement)
{
  switch (evenement->keyval)
    {
    case GDK_KEY_Escape:
      break;
    case GDK_KEY_F1:
      {
        SimTaDynContext& simtadyn = SimTaDynContext::getInstance();
        m_drawing_area.zoomFitPage(simtadyn.m_graph);
      }
      break;
    case GDK_KEY_Page_Up:
      m_drawing_area.keyPressed(GlDrawingArea::Forward);
      break;
    case GDK_KEY_Page_Down:
      m_drawing_area.keyPressed(GlDrawingArea::Backward);
      break;
    case GDK_KEY_Up:
      m_drawing_area.keyPressed(GlDrawingArea::Up);
      break;
    case GDK_KEY_Down:
      m_drawing_area.keyPressed(GlDrawingArea::Down);
      break;
    case GDK_KEY_Right:
      m_drawing_area.keyPressed(GlDrawingArea::Right);
      break;
    case GDK_KEY_Left:
      m_drawing_area.keyPressed(GlDrawingArea::Left);
      break;
    default:
      break;
    }
}

void SimTaDynWindow::onKeyReleased(GdkEventKey* evenement)
{
  switch (evenement->keyval)
    {
    case GDK_KEY_Page_Up:
      m_drawing_area.keyReleased(GlDrawingArea::Forward);
      break;
    case GDK_KEY_Page_Down:
      m_drawing_area.keyReleased(GlDrawingArea::Backward);
      break;
    case GDK_KEY_Up:
      m_drawing_area.keyReleased(GlDrawingArea::Up);
      break;
    case GDK_KEY_Down:
      m_drawing_area.keyReleased(GlDrawingArea::Down);
      break;
    case GDK_KEY_Right:
      m_drawing_area.keyReleased(GlDrawingArea::Right);
      break;
    case GDK_KEY_Left:
      m_drawing_area.keyReleased(GlDrawingArea::Left);
      break;
    default:
      break;
    }
}
