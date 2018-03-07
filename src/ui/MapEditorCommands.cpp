#include "MapTools.hpp"
#include "Logger.hpp"

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
class AddCellTool: public IMouseClickEvent
{
public:

  virtual void button1PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Ajoute1 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button2PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Ajoute2 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button3PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Ajoute3 une cellule a la position %f/%f/%f", x, y, z);
  }
};

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
class RemoveCellTool: public IMouseClickEvent
{
public:

  virtual void button1PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Remove1 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button2PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Remove2 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button3PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Remove3 une cellule a la position %f/%f/%f", x, y, z);
  }
};

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
class SelectCellTool: public IMouseClickEvent
{
public:

  virtual void button1PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Select1 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button2PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Select2 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button3PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Select3 une cellule a la position %f/%f/%f", x, y, z);
  }
};

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
class MoveCellTool: public IMouseClickEvent
{
public:

  virtual void button1PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Move1 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button2PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Move2 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button3PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Move3 une cellule a la position %f/%f/%f", x, y, z);
  }
};

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
MapEditorCommands::MapEditorCommands()
  : m_action_type(m_toolbar),
    m_action_on(m_toolbar)
{
  // Init map edition tool to dummy action
  m_edition_tools[ActionType::Add] = new AddCellTool(*this);
  m_edition_tools[ActionType::Remove] = new RemoveCellTool(*this);
  m_edition_tools[ActionType::Select] = new SelectCellTool(*this);
  m_edition_tools[ActionType::Move] = new MoveCellTool(*this);

  using namespace std::placeholders;
  m_toolbar.set_property("orientation", Gtk::ORIENTATION_VERTICAL);
  m_toolbar.set_property("toolbar-style", Gtk::TOOLBAR_ICONS);

  // Cells
  m_action_on.append(ActionOn::Node, "Switch to Node mode", Gtk::Stock::YES,
                     sigc::mem_fun(*this, &MapEditorCommands::onActionOnSelected_));
  m_action_on.append(ActionOn::Arc, "Switch to Arc mode", Gtk::Stock::NO,
                     sigc::mem_fun(*this, &MapEditorCommands::onActionOnSelected_));
  m_action_on.append(ActionOn::Zone, "Switch to Zone mode", Gtk::Stock::HOME,
                     sigc::mem_fun(*this, &MapEditorCommands::onActionOnSelected_));
  m_action_on.append(m_toolbar_separator[0]);

  // Operations on cells
  m_action_type.append(ActionType::Add, "Remove a cell", Gtk::Stock::ADD,
                       sigc::mem_fun(*this, &MapEditorCommands::onActionTypeSelected_));
  m_action_type.append(ActionType::Remove, "Remove a cell", Gtk::Stock::REMOVE,
                       sigc::mem_fun(*this, &MapEditorCommands::onActionTypeSelected_));
  m_action_type.append(ActionType::Select, "Select a cell", Gtk::Stock::JUMP_TO,
                       sigc::mem_fun(*this, &MapEditorCommands::onActionTypeSelected_));
  m_action_type.append(ActionType::Move, "Move a cell", Gtk::Stock::JUMP_TO,
                       sigc::mem_fun(*this, &MapEditorCommands::onActionTypeSelected_));
  m_toolbar.append(m_toolbar_separator[1]);
}

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
MapEditorCommands::~MapEditorCommands()
{
  delete m_edition_tools[ActionType::Add];
  delete m_edition_tools[ActionType::Remove];
  delete m_edition_tools[ActionType::Select];
  delete m_edition_tools[ActionType::Move];
}

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
void MapEditorCommands::onActionOnSelected(const ActionOn id)
{
  LOGI("ActionOnSelected %u", id);
  // TODO: afficher les id du type de cellule selectionnee
}

// *************************************************************************************************
//!
// *************************************************************************************************
void MapEditorCommands::onActionTypeSelected(const ActionType id)
{
  LOGI("ActionTypeSelected %u", id);
  // TODO changer le curseur
}
