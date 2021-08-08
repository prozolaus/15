#include "Simple_window.h"
#include "Graph.h"

int main()
{
  using namespace Graph_lib; //our graphics facilities are in Graph_lib (defined in Graph.h)

  Point top_left{0, 0}; //will be top left corner of window
  Simple_window win{top_left, 600, 600, "Function graphs"};

  Axis x{Axis::x, Point{100, 300}, 400, 400 / 20, "1 == 20 pixels"};
  Axis y{Axis::y, Point{300, 500}, 400, 400 / 20, "1 == 20 pixels"};
  x.set_color(Color::red);
  y.set_color(Color::red);

  win.attach(x);
  win.attach(y);
  win.wait_for_button();
}
