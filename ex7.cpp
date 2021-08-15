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
  void add_label(const string &s, Point p);
  void add_bar_label(const string &s, int i);
  void set_bar_color(Color c, int i);

private:
  vector<double> numbers;
  Vector_ref<Rectangle> vr;
  Vector_ref<Text> vt;
  Vector_ref<Text> vlabels;
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
  x0y0 = x1y1;
}

void Bar_graph::add_label(const string &s, Point p)
{
  vlabels.push_back(new Text{p, s});
}

void Bar_graph::add_bar_label(const string &s, int i)
{
  Point p = vr[i].point(0);
  vt.push_back(new Text{Point{p.x + 1, p.y - 1}, s});
}

void Bar_graph::set_bar_color(Color c, int i)
{
  vr[i].set_fill_color(c);
}

void Bar_graph::draw_lines() const
{
  if (color().visibility())
  { // lines on top of fill
    for (int i = 0; i < vr.size(); i++)
      vr[i].draw_lines();
    for (int i = 0; i < vt.size(); i++)
      vt[i].draw_lines();
    for (int i = 0; i < vlabels.size(); i++)
      vlabels[i].draw_lines();
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
  {
    int n = rand() % 17 + 1;
    bg.add(n);
    bg.add_bar_label(to_string(n), i);
    bg.set_bar_color(Color{rand() % 256}, i);
  }
  bg.add_label("Test",Point{20,20});

  win.attach(x);
  win.attach(y);
  win.attach(bg);

  win.wait_for_button();
}
