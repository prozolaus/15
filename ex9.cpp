#include "Simple_window.h"
#include "Graph.h"

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
  if (y == 0)
    y = 1;
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
  int symbol_length = 5;
  vt.push_back(new Text{Point{int(p.x + xscale / 2 - symbol_length * s.size() / 2), p.y - 1}, s});
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

struct Data
{
  int height, number;
};

istream &operator>>(istream &is, Data &d)
{
  char ch1, ch2, ch3;
  Data dd;

  if (is >> ch1 >> dd.height >> ch2 >> dd.number >> ch3)
  {
    if (ch1 != '(' || ch2 != ',' || ch3 != ')')
    {
      is.clear(ios_base::failbit);
      return is;
    }
  }
  else
    return is;
  d = dd;
  return is;
}

//-------------------------------------------------------------------------

constexpr int xmax = 1200;
constexpr int ymax = 600;

constexpr int xoffset = 100;
constexpr int yoffset = 100;

constexpr int xspace = 100;
constexpr int yspace = 100;

constexpr int xlength = xmax - xoffset - xspace;
constexpr int ylength = ymax - yoffset - yspace;

constexpr Point x0y0{xoffset, ymax - yoffset};

//-------------------------------------------------------------------------

int main()
{
  using namespace Graph_lib; //our graphics facilities are in Graph_lib (defined in Graph.h)

  Point top_left{0, 0}; //will be top left corner of window

  Simple_window win{top_left, xmax, ymax, "Estimated number of women aged 18-74 years by height: United States 1971-74"};

  vector<int> heights, numbers;
  vector<int> numbers2, nmbrs;

  string fname = "ex8.txt";
  ifstream ifs{fname};
  if (!ifs)
    error("Can't open a file", fname);

  for (Data d; ifs >> d;)
  {
    if (d.number < 0 || d.height < 0)
      error("Error! Negative numbers!");
    if (d.height % 5)
      error("Steps of heights should be multiple of 5");
    heights.push_back(d.height);
    numbers.push_back(d.number);
  }
  if (!ifs.eof())
    error("Error! Wrong input data!");

  int count = 1, m = *max_element(numbers.begin(), numbers.end());
  if (m > ylength)
  {
    while (true)
    {
      if ((m / ylength) % 10 > 0)
      {
        count *= 10;
        m /= 10;
      }
      else
        break;
    }

    for (int i = 0; i < numbers.size(); i++)
      numbers2.push_back(round(numbers[i] / count));
  }
  nmbrs = (numbers2.size() > 0) ? numbers2 : numbers;

  const double x_scale = double(xlength / nmbrs.size());
  const double y_scale = double(ylength) / (*max_element(nmbrs.begin(), nmbrs.end()));
  ostringstream osx, osy;
  osx << "Height, cm "
      << "(scale: 5 cm == " << (int)x_scale << " pixels)";
  osy << "Number of people "
      << "(scale: " << count << " person == " << (int)y_scale << " pixels)";
  Axis x{Axis::x, x0y0, xlength, int(xlength / x_scale), osx.str()};
  Axis y{Axis::y, x0y0, ylength, int(ylength / y_scale), osy.str()};
  x.set_color(Color::red);
  y.set_color(Color::red);

  srand(static_cast<unsigned int>(time(0)));

  Bar_graph hnp{x0y0, x_scale, y_scale}; //height and number of people

  for (int i = 0; i < heights.size(); i++)
  {
    hnp.add(nmbrs[i]);
    hnp.add_bar_label(to_string(numbers[i]), i);
    hnp.set_bar_color(Color(rand() % 256), i);
    hnp.add_label(to_string(heights[i]),
                  Point{int(xoffset + i * x_scale + x_scale / 2 - 10), int(ymax - yoffset / 2)});
  }

  win.attach(x);
  win.attach(y);
  win.attach(hnp);

  win.wait_for_button();
}
