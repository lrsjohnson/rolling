// generated by Fast Light User Interface Designer (fluid) version 1.0302

#include "rollingui.h"

Fl_Menu_Item RollingUI::menu_[] = {
 {"File", 0,  0, 0, 64, FL_NORMAL_LABEL, 0, 14, 0},
 {"New", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0}
};
Fl_Menu_Item* RollingUI::file_menu = RollingUI::menu_ + 0;
Fl_Menu_Item* RollingUI::file_new_button = RollingUI::menu_ + 1;

RollingUI::RollingUI() {
  { window = new Fl_Double_Window(775, 596, "Rolling Sphere");
    window->user_data((void*)(this));
    { scene_view = new RollingView(0, 54, 775, 516, "OpenGL View");
      scene_view->box(FL_BORDER_BOX);
      scene_view->color(FL_FOREGROUND_COLOR);
      scene_view->selection_color(FL_BACKGROUND_COLOR);
      scene_view->labeltype(FL_NORMAL_LABEL);
      scene_view->labelfont(0);
      scene_view->labelsize(14);
      scene_view->labelcolor((Fl_Color)55);
      scene_view->align(Fl_Align(FL_ALIGN_CENTER));
      scene_view->when(FL_WHEN_RELEASE);
      Fl_Group::current()->resizable(scene_view);
    } // RollingView* scene_view
    { Fl_Menu_Bar* o = new Fl_Menu_Bar(0, 0, 775, 25);
      o->box(FL_BORDER_BOX);
      o->menu(menu_);
    } // Fl_Menu_Bar* o
    { status_bar = new Fl_Group(0, 570, 775, 25);
      status_bar->box(FL_BORDER_BOX);
      status_bar->end();
    } // Fl_Group* status_bar
    window->end();
  } // Fl_Double_Window* window
}

void RollingUI::show() {
  window->show();
  scene_view->show();
}
