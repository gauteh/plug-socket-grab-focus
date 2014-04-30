#include <iostream>
#include <fstream>
#include <gtkmm.h>
#include <gtkmm/plug.h>
#include <glib/gstdio.h>

using namespace std;

const char* id_filename = "plug.id";

void on_embed()
{
  cout << "I've been embedded." << endl;
}

class MyPlug : public Gtk::Plug
{
  public:
    MyPlug()
      
  {
    set_size_request(150, 100);
    add(m_label);
    signal_embedded().connect(sigc::ptr_fun(on_embed));
    show_all_children();

    m_label.set_text ("asd");
  }

  private:
    Gtk::Entry m_label;
};


int main(int argc, char** argv)
{
  // The plug and the socket have different application ids, so they can run
  // simultaneously.
  Glib::RefPtr<Gtk::Application> app =
    Gtk::Application::create(argc, argv, "org.gtkmm.example.plug");
  MyPlug plug;
  plug.show();

  ofstream out(id_filename);
  out << plug.get_id();
  out.close();
  cout << "The window ID is: " << plug.get_id() << endl;

  app->run(plug);

  // remove the ID file when the program exits
  g_remove(id_filename);
  return 0;
}
