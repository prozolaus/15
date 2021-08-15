#include "Simple_window.h"
#include "Graph.h"

//-------------------------------------------------------------------------

struct Data
{
  int month;
  double temperature;
};

istream &operator>>(istream &is, Data &d)
{
  char ch1, ch2, ch3;
  Data dd;

  if (is >> ch1 >> dd.month >> ch2 >> dd.temperature >> ch3)
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

class Scale
{
  int cbase;
  int vbase;
  double scale;

public:
  Scale(int b, int vb, double s) : cbase{b}, vbase{vb}, scale{s} {}
  int operator()(double v) const { return cbase + int((v - vbase) * scale); }
};

//-------------------------------------------------------------------------
void read(istream &ifs, const Scale &xs, const Scale &ys, Open_polyline &city)
{
  for (Data d; ifs >> d;)
  {
    if (d.month < 1 || d.month > 12)
      error("Error! Wrong month's number!");
    if (d.temperature < -30 || d.temperature > 30)
      error("Error! Wrong temperature!");
    city.add(Point{xs(d.month), ys(d.temperature)});
  }
  if (!ifs.eof())
    error("Error! Wrong input data!");
}
//-------------------------------------------------------------------------

constexpr int xmax = 1160;
constexpr int ymax = 700;

constexpr int xoffset = 100;
constexpr int yoffset = 100;

constexpr int xspace = 100;
constexpr int yspace = 100;

constexpr int xlength = xmax - xoffset - xspace;
constexpr int ylength = ymax - yoffset - yspace;

constexpr Point x0y0{xoffset, ymax - yoffset - int(ylength / 2)};

//-------------------------------------------------------------------------

int main()
{
  using namespace Graph_lib; //our graphics facilities are in Graph_lib (defined in Graph.h)

  Point top_left{0, 0}; //will be top left corner of window

  Simple_window win{top_left, xmax, ymax, "Average monthly temperatures in a few cities"};

  const double x_scale = double(xlength / 12);
  const double y_scale = double(ylength) / 60;
  ostringstream osx, osy;
  osx << "Month (from Jan. to Dec.) "
      << "(scale: 1 month == " << (int)x_scale << " pixels)";
  osy << "Temperature (from -30 to 30 degree Celsius) "
      << "(scale: 1 d.C. == " << (int)y_scale << " pixels)";
  Axis x{Axis::x, x0y0, xlength, int(xlength / x_scale), osx.str()};
  Axis y{Axis::y, Point{x0y0.x, x0y0.y + int(ylength / 2)},
         ylength, int(ylength / y_scale), osy.str()};
  win.attach(x);
  win.attach(y);

  Scale xs{xoffset, 1, x_scale};
  Scale ys{x0y0.y, 0, -y_scale};

  vector<string> cities, cities_names{"Kyiv", "Cairo", "Murmansk", "Rome"};
  for (int i = 0; i < cities_names.size(); i++)
    cities.push_back("ex11_" + cities_names[i] + ".txt");

  vector<Open_polyline> cities_lines(cities.size());
  Vector_ref<Text> vt;
  ifstream ifs;

  for (int i = 0; i < cities.size(); i++)
  {
    ifs.open(cities[i]);
    if (!ifs)
      error("Can't open a file", cities[i]);
    read(ifs, xs, ys, cities_lines[i]);
    cities_lines[i].set_color(Color(i + 1));
    cities_lines[i].set_style(Line_style(Line_style::solid, 3));
    vt.push_back(new Text{Point{20, cities_lines[i].point(0).y}, cities_names[i]});
    win.attach(cities_lines[i]);
    win.attach(vt[i]);
    ifs.close();
  }

  win.wait_for_button();
}
