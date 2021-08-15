#include "Simple_window.h"
#include "Graph.h"

constexpr int xmax = 1160;
constexpr int ymax = 560;

constexpr int x_orig = xmax / 2;
constexpr int y_orig = ymax / 2;
constexpr Point orig{x_orig, y_orig};

constexpr int r_min = 0;
constexpr int r_max = 40;

constexpr int x_scale = 20;
constexpr int y_scale = 20;

constexpr int xlength = xmax - 200;
constexpr int ylength = ymax - 200;

constexpr int n_points = xlength;

//-------------------------------------------------------------------------

struct Bar_graph : Shape
{
  Bar_graph(Point xy, int x_scale, int y_scale);
  void add(double);
  void draw_lines() const override;

private:
  vector<double> numbers;
  Vector_ref<Rectangle> vr;
  Point x0y0;
  int xscale, yscale;
};

Bar_graph::Bar_graph(Point xy, int x_scale, int y_scale)
    : x0y0{xy}, xscale{x_scale}, yscale{y_scale}
{
}

void Bar_graph::add(double y)
{
  numbers.push_back(y);
  Point x1y1 = Point{x0y0.x + xscale, x0y0.y};
  vr.push_back(new Rectangle{Point{x0y0.x, int(x0y0.y - y * yscale)}, x1y1});
  vr[vr.size()-1].set_fill_color(Color(rand() % 256));
  x0y0 = x1y1;
}

void Bar_graph::draw_lines() const
{
  if (color().visibility())
  { // lines on top of fill
    for (int i = 0; i < vr.size(); i++)
      vr[i].draw_lines();
  }
}

//-------------------------------------------------------------------------

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

  srand(static_cast<unsigned int>(time(0)));

  Bar_graph bg{x0y0, x_scale, y_scale};
  for (int i = 0; i < r_max; i++)
    bg.add(rand() % 17 + 1);

  win.attach(x);
  win.attach(y);
  win.attach(bg);

  win.wait_for_button();
}
