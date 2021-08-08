#include "Simple_window.h"
#include "Graph.h"

constexpr int xmax = 600;
constexpr int ymax = 600;

constexpr int x_orig = xmax / 2;
constexpr int y_orig = ymax / 2;
constexpr Point orig{x_orig, y_orig};

constexpr int r_min = -10;
constexpr int r_max = 11;

constexpr int n_points = 400;

constexpr int x_scale = 20;
constexpr int y_scale = 20;

constexpr int xlength = xmax - 200;
constexpr int ylength = ymax - 200;

//-------------------------------------------------------------------------

double one(double x) { return 1; }

double slope(double x) { return x / 2; }

double square(double x) { return x * x; }

double sloping_cos(double x) { return cos(x) + slope(x); }

//-------------------------------------------------------------------------

int main()
{
  using namespace Graph_lib; //our graphics facilities are in Graph_lib (defined in Graph.h)

  Point top_left{0, 0}; //will be top left corner of window
  Simple_window win{top_left, xmax, ymax, "Function graphs"};

  Axis x{Axis::x, Point{100, 300}, xlength, xlength / x_scale, "1 == 20 pixels"};
  Axis y{Axis::y, Point{300, 500}, ylength, ylength / y_scale, "1 == 20 pixels"};
  x.set_color(Color::red);
  y.set_color(Color::red);
  x.label.move(200, 0);

  Function f1{one, r_min, r_max, orig, n_points, x_scale, y_scale};
  Function f2{slope, r_min, r_max, orig, n_points, x_scale, y_scale};
  Text tf2{Point{100, 380}, "x/2"};
  Function f3{square, r_min, r_max, orig, n_points, x_scale, y_scale};
  Function f4{cos, r_min, r_max, orig, n_points, x_scale, y_scale};
  Function f5{sin, r_min, r_max, orig, n_points, x_scale, y_scale};
  f5.set_color(Color::blue);
  Function f6{sloping_cos, r_min, r_max, orig, n_points, x_scale, y_scale};


  win.attach(x);
  win.attach(y);
  win.attach(f1);
  win.attach(f2);
  win.attach(tf2);
  win.attach(f3);
  win.attach(f4);
  win.attach(f5);
  win.attach(f6);
  win.wait_for_button();
}
