#ifndef SIMTADYNWINDOW_HPP_
#  define SIMTADYNWINDOW_HPP_

#  include "ForthEditor.hpp"
#  include "DrawingArea.hpp"

class SimTaDynWindow: public Gtk::Window
{
public:
  enum ToolBarNames { MapToolbar, ForthToolbar };
  enum MenuNames { MapMenu, ForthMenu, PlugginsMenu };

  SimTaDynWindow(const std::string& title);
  virtual ~SimTaDynWindow()
  {
      //Gtk::Main::quit();
  }

  void onRealize();
  void onUnrealize();
  bool onRender(const Glib::RefPtr<Gdk::GLContext>& /* context */);

  void addForthMenu(const Gtk::BuiltinStockID icon,
                    const std::string &word);
  Gtk::ToolButton *addForthButon(enum ToolBarNames toolbar,
                                 const Gtk::BuiltinStockID icon,
                                 const std::string &script,
                                 const std::string &help);  // FIXME: Glib::ustring, const Cell16 Forthtoken);
  Gtk::ToolButton *addForthScriptButon(const Gtk::BuiltinStockID icon,
                                       const std::string &script,
                                       const std::string &help);
  Gtk::ToolButton *addMapScriptButon(const Gtk::BuiltinStockID icon,
                                     const std::string &script,
                                     const std::string &help);
protected:
  Gtk::HPaned m_hpaned[1];
  Gtk::VPaned m_vpaned[1];
  Gtk::VBox m_vbox[2];
  Gtk::HBox m_hbox[1];
  Gtk::MenuBar m_menubar;
  Gtk::MenuItem m_menuitem[4];
  Gtk::SeparatorToolItem m_separator[2];
  Gtk::Toolbar m_toolbar[2];
  std::vector<Gtk::ToolButton> m_toolbuttons;

  //GlDrawingArea m_drawing_area; // FIXME a cause d'un bug de gtkmm
  Gtk::GLArea m_drawing_area;
  Renderer m_renderer;// FIXME a retirer
  ForthEditor m_fortheditor;
  FindWindow *m_findwin;
};

#endif /* SIMTADYNWINDOW_HPP_ */
