#include "SimTaDynContext.hpp"

// Create the context as singleton
SimTaDynContext SimTaDynContext::s_SimTaDyn = SimTaDynContext();

int main(int argc, char** argv)
{
  const Gtk::Main kit(argc, argv);
  Gtk::GL::init(argc, argv);

  SimTaDynWindow main_window("SimTaDyn");
  SimTaDynContext& SimTaDyn = SIMTADYN();
  SimTaDyn.m_window = &main_window;

  // FIXME: temporary, just for testing
  SimTaDyn.m_forth.addForthButon(Gtk::Stock::COLOR_PICKER, "FOO"); // FIXME "42 42 FOO" ne fonctionne pas // FIXME2: interdire une compilation

  kit.run(*(SimTaDyn.m_window));
  return 0;
}