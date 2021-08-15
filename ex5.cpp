#include "Simple_window.h"
#include "Graph.h"

constexpr int xmax = 1160;
constexpr int ymax = 560;

constexpr int x_orig = xmax / 2;
constexpr int y_orig = ymax / 2;
constexpr Point orig{x_orig, y_orig};

constexpr int r_min = 0;
constexpr int r_max = 16;

constexpr int x_scale = 60;
constexpr int y_scale = 180;

constexpr int xlength = xmax - 200;
constexpr int ylength = ymax - 200;

constexpr int n_points = xlength;

//-------------------------------------------------------------------------

double leibniz_formula(double n)
{
  double sum = 0;
  for (double i = 0; i < n; i++)
    sum += double(pow(-1, i) / (2 * i + 1));
  return sum;
}

int main()
{
  using namespace Graph_lib; //our graphics facilities are in Graph_lib (defined in Graph.h)

  Point top_left{0, 0}; //will be top left corner of window

  Simple_window win{top_left, xmax, ymax, "Canvas"};

  ostringstream osx, osy;
  osx << "1 == " << x_scale << " pixels";
  osy << "1 == " << y_scale << " pixels";
  Point x0y0{100, ymax - 100};
  Axis x{Axis::x, x0y0, xlength, xlength / x_scale, osx.str()};
  Axis y{Axis::y, x0y0, ylength, ylength / y_scale, osy.str()};
  x.set_color(Color::red);
  y.set_color(Color::red);
  x.label.move(200, 0);
  win.attach(x);
  win.attach(y);

  for (int i = 0; i < r_max; i++)
  {
    ostringstream os;
    os << "Leibniz approximation; n==" << i;
    win.set_label(os.str());
    Function fleibniz{leibniz_formula, r_min, double(i + 1), x0y0, n_points, x_scale, y_scale};
    win.attach(fleibniz);
    win.wait_for_button();
    win.detach(fleibniz);
  }
}
