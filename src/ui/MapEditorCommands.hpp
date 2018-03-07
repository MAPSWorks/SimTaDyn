#ifndef MAP_EDITOR_COMMANDS_HPP
#  define MAP_EDITOR_COMMANDS_HPP

#  include "ToggleButtons.hpp"
#  include "Logger.hpp"

//class IEditor {};

// ***********************************************************************************************
//! \brief Command Pattern for map edition (insertion, move ...)
//! http://gameprogrammingpatterns.com/command.html
// ***********************************************************************************************
class IMouseClickEvent
{
public:

  IMouseClickEvent(/*IEditor& editor*/)
  //: m_editor(editor)
  {
  }

  virtual ~IMouseClickEvent() {}
  virtual void button1PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) = 0;
  virtual void button2PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) = 0;
  virtual void button3PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) = 0;
  virtual void undoClick() {};

  /*private:

    IEditor& m_editor;*/
};

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
class MapEditorCommands
{
public:

  MapEditorCommands();
  ~MapEditorCommands();

  //------------------------------------------------------------------
  //! \brief Return the widget to be attached to a GTK widget.
  //------------------------------------------------------------------
  inline Gtk::Toolbar &toolbar()
  {
    return m_toolbar;
  }

  inline void button1PressEvent(const gdouble x, const gdouble y)
  {
    LOGD("Bouton1 click %d %d", (int) x, (int) y);
    m_edition_tools[actionType()]->button1PressEvent(x, y);
  }

  inline void button2PressEvent(const gdouble x, const gdouble y)
  {
    LOGD("Bouton2 click %d %d", (int) x, (int) y);
    m_edition_tools[actionType()]->button2PressEvent(x, y);
  }

  inline void button3PressEvent(const gdouble x, const gdouble y)
  {
    LOGD("Bouton3 click %d %d", (int) x, (int) y);
    m_edition_tools[actionType()]->button2PressEvent(x, y);
  }

private:

  //! \brief Add, remove a mode (node, arc, zone).
  enum ActionType { Add, Remove, Select, Move, FirstAction = Add, LastAction = Move };

  //! \brief On what kind of cells action is performed.
  enum ActionOn { Node, Arc, Zone, FirstMode = Node, LastMode = Zone };

  inline ActionType actionType() const
  {
    return static_cast<ActionType>(m_action_type.button());
  }

  inline ActionOn actionOn() const
  {
    return static_cast<ActionOn>(m_action_on.button());
  }

  void onActionOnSelected(const ActionOn id);
  void onActionTypeSelected(const ActionType id);

  inline void onActionOnSelected_(const uint32_t id)
  {
    onActionOnSelected(static_cast<ActionOn>(id));
  }
  inline void onActionTypeSelected_(const uint32_t id)
  {
    onActionTypeSelected(static_cast<ActionType>(id));
  }

private:

  IMouseClickEvent      *m_edition_tools[ActionType::LastAction + 1u];
  Gtk::SeparatorToolItem m_toolbar_separator[2];
  Gtk::Toolbar           m_toolbar;
  ToggleButtons          m_action_type;
  ToggleButtons          m_action_on;
};

#endif
