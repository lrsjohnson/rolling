// generated by Fast Light User Interface Designer (fluid) version 1.0302

#include "rollingui.h"

RollingUI::RollingUI() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = new Fl_Double_Window(643, 458);
    w = o;
    o->user_data((void*)(this));
    { VChange = new Fl_Group(20, 65, 50, 245);
      { vrot = new Fl_Roller(20, 65, 25, 245);
      } // Fl_Roller* vrot
      { ypan = new Fl_Slider(45, 65, 25, 245);
      } // Fl_Slider* ypan
      VChange->end();
    } // Fl_Group* VChange
    { HChange = new Fl_Group(165, 365, 300, 50);
      { hrot = new Fl_Roller(165, 365, 300, 25);
        hrot->type(1);
      } // Fl_Roller* hrot
      { xpan = new Fl_Slider(165, 390, 300, 25);
        xpan->type(1);
      } // Fl_Slider* xpan
      HChange->end();
    } // Fl_Group* HChange
    { MainView = new Fl_Group(25, 25, 510, 327);
      { scene_view = new RollingView(115, 63, 420, 289, "label");
        scene_view->box(FL_NO_BOX);
        scene_view->color(FL_BACKGROUND_COLOR);
        scene_view->selection_color(FL_BACKGROUND_COLOR);
        scene_view->labeltype(FL_NORMAL_LABEL);
        scene_view->labelfont(0);
        scene_view->labelsize(14);
        scene_view->labelcolor(FL_FOREGROUND_COLOR);
        scene_view->align(Fl_Align(FL_ALIGN_CENTER));
        scene_view->when(FL_WHEN_RELEASE);
      } // RollingView* scene_view
      MainView->end();
    } // Fl_Group* MainView
    { zoom = new Fl_Value_Slider(115, 10, 380, 20, "slider:");
      zoom->type(1);
      zoom->textsize(14);
    } // Fl_Value_Slider* zoom
    o->end();
  } // Fl_Double_Window* o
}