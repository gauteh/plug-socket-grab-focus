#include <iostream>
#include <vector>
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

    }

    bool plug_removed()
    {
      cout << "A Plug was removed" << endl;
      return true;
    }

    void btn_clicked () {
      cout << "grab focus " << endl;
      socket->grab_focus ();
      socket->get_plug_window()->focus (-1);

      /*
      vector<Widget*> ch = socket->get_children ();
      ch[0]->grab_focus ();
      */

    }

    MySocketWindow()
    {
      ifstream infile(id_filename);
      if (infile)
      {
        Gtk::Button *btn  = Gtk::manage (new Gtk::Button("focus plug"));
        Gtk::Entry *entry = Gtk::manage (new Gtk::Entry());
        Gtk::Box *box     = Gtk::manage (new Gtk::Box());
        socket = Gtk::manage(new Gtk::Socket());
        box->pack_start (*entry, true, 5);
        box->pack_start (*btn, true, 5);
        box->pack_start (*socket, true, 5);
        add(*box);

        btn->signal_clicked().connect(sigc::mem_fun(*this, &MySocketWindow::btn_clicked));

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
