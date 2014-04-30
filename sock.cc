#include <iostream>
#include <fstream>
#include <gtkmm.h>
#include <gtkmm/socket.h>

using namespace std;

const char* id_filename = "plug.id";


class MySocketWindow : public Gtk::Window
{
  public:
    Gtk::Socket *socket;

    void plug_added()
    {
      cout << "A plug was added" << endl;

      socket->grab_focus ();
    }

    bool plug_removed()
    {
      cout << "A Plug was removed" << endl;
      return true;
    }
    MySocketWindow()
    {
      ifstream infile(id_filename);
      if (infile)
      {
        Gtk::Entry *entry = Gtk::manage (new Gtk::Entry());
        Gtk::Box *box     = Gtk::manage (new Gtk::Box());
        socket = Gtk::manage(new Gtk::Socket());
        box->pack_start (*entry, true, 5);
        box->pack_start (*socket, true, 5);
        add(*box);

        socket->signal_plug_added().connect(sigc::mem_fun(*this, &MySocketWindow::plug_added));
        socket->signal_plug_removed().connect(sigc::mem_fun(*this, &MySocketWindow::plug_removed));
        ::Window plug_id = 0;
        infile >> plug_id;
        infile.close();
        socket->add_id(plug_id);
      }
      else
      {
        Gtk::Label* label = Gtk::manage(
            new Gtk::Label(
              "Plug id file not found.\n Make sure plug is running."));
        add(*label);
        set_size_request(150, 50);
      }
      show_all();
    }
};

int main(int argc, char** argv)
{
  // The plug and the socket have different application ids, so they can run
  // simultaneously.
  Glib::RefPtr<Gtk::Application> app =
    Gtk::Application::create(argc, argv, "org.gtkmm.example.socket");
  MySocketWindow win;
  app->run(win);
  return 0;
}
