#include "Simple_window.h"
#include "Graph.h"

struct Flex_Function : Shape
{
  Flex_Function(Fct f, double r1, double r2, Point orig,
                int count = 100, double xscale = 25, double yscale = 25);

  Flex_Function(double (*f)(double), double r1, double r2, Point orig,
                int count = 100, double xscale = 25, double yscale = 25);

  void add(Point p) { points.push_back(p); }
  void draw_lines() const override;
  int number_of_points() const { return int(points.size()); }
  
  void change_range(double d1, double d2);
  void change_scales(double xscale, double yscale);
  void change_center(Point p);
  void change_count(int count);
  void change_function(Fct f);
  void change_function(double (*f)(double));

private:
  Fct fnct;
  double x1, x2, xs, ys;
  Point center;
  int n;
  vector<Point> points;
  void reset();
  void clean_points() { points.clear(); }
};

//-----------------------------------------------------------------------------

Flex_Function::Flex_Function(Fct f, double r1, double r2, Point xy,
                             int count, double xscale, double yscale)
    : fnct{f}, x1{r1}, x2{r2}, center{xy}, n{count}, xs{xscale}, ys{yscale}
// graph f(x) for x in [r1:r2) using count line segments with (0,0) displayed at xy
// x coordinates are scaled by xscale and y coordinates scaled by yscale
{
  reset();
}

Flex_Function::Flex_Function(double (*f)(double), double r1, double r2, Point xy,
                             int count, double xscale, double yscale)
    : Flex_Function{static_cast<Fct>(f), r1, r2, xy, count, xscale, yscale}
{
}

void Flex_Function::reset()
{
  if (x2 - x1 <= 0)
    error("bad graphing range");
  if (n <= 0)
    error("non-positive graphing count");
  double dist = (x2 - x1) / n;
  double x = x1;
  for (int i = 0; i < n; ++i)
  {
    add(Point{center.x + int(x * xs), center.y - int(fnct(x) * ys)});
    x += dist;
  }
}

void Flex_Function::change_range(double d1, double d2)
{
  x1 = d1;
  x2 = d2;
  clean_points();
  reset();
}

void Flex_Function::change_scales(double xscale, double yscale)
{
  xs = xscale;
  ys = yscale;
  clean_points();
  reset();
}

void Flex_Function::change_center(Point p)
{
  center = p;
  clean_points();
  reset();
}

void Flex_Function::change_count(int count)
{
  n = count;
  clean_points();
  reset();
}

void Flex_Function::change_function(Fct f)
{
  fnct = f;
  clean_points();
  reset();
}

void Flex_Function::change_function(double (*f)(double))
{
  fnct = f;
  clean_points();
  reset();
}

void Flex_Function::draw_lines() const
{
  if (fill_color().visibility())
  {
    fl_color(fill_color().as_int());
    fl_begin_complex_polygon();
    for (int i = 0; i < number_of_points(); ++i)
    {
      fl_vertex(point(i).x, point(i).y);
    }
    fl_end_complex_polygon();
    fl_color(color().as_int()); // reset color
  }

  if (color().visibility() && 1 < points.size()) // draw sole pixel?
    for (unsigned int i = 1; i < points.size(); ++i)
      fl_line(points[i - 1].x, points[i - 1].y, points[i].x, points[i].y);
}

//------------------------------------------------------------------------------------

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

  Flex_Function f1{sin, r_min, r_max, orig, n_points, x_scale, y_scale};

  win.attach(x);
  win.attach(y);
  win.attach(f1);

  win.wait_for_button();
  //f1.change_range(-5,5);
  //f1.change_scales(10, 10);
  //f1.change_center(Point{x_orig, y_orig - 100});
  //f1.change_function(cos);
  f1.change_count(20);
  win.wait_for_button();
}