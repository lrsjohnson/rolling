// generated by Fast Light User Interface Designer (fluid) version 1.0302

#ifndef rollingui_h
#define rollingui_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include "rollingview.h"
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>

class RollingUI {
public:
  RollingUI();
  Fl_Double_Window *window;
  Fl_Group *back;
  RollingView *scene_view;
  static Fl_Menu_Item menu_[];
  static Fl_Menu_Item *file_menu;
  static Fl_Menu_Item *file_new_button;
  Fl_Group *status_bar;
  Fl_Group *splash;
private:
  inline void cb_Start_i(Fl_Button*, void*);
  static void cb_Start(Fl_Button*, void*);
  static Fl_Menu_Item menu_1[];
public:
  Fl_Group *mode_group;
  Fl_Group *mode_display;
  void show();
};
#endif
