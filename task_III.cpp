#include "Simple_window.h"
#include "Graph.h"

struct Person
{
  Person()
  {
    first_name = "Noname";
    second_name = "";
    age = 0;
  };
  Person(string person_first_name, string person_second_name, int person_age);
  istream &operator>>(istream &is);
  string get_first_name() const { return first_name; }
  string get_second_name() const { return second_name; }
  int get_age() const { return age; }

private:
  string first_name;
  string second_name;
  int age;
  void check();
};
//---------------------------------------------------------------------
void Person::check()
{
  const string er_symbols = ";:\"'[]*&^%$#@!";
  const int min_age = 0;
  const int max_age = 150;
  if (age < min_age || age > max_age)
    error("Wrong age! Age can be from 0 to 150 years old!");
  string name;
  for (int n = 0; n < 2; n++)
  {
    name = (!n) ? first_name : second_name;
    for (int i = 0; i < name.size(); i++)
      for (int j = 0; j < er_symbols.size(); j++)
        if (name[i] == er_symbols[j])
          error("Forbidden symbols are ;:\"'[]*&^%$#@!");
  }
}

Person::Person(string person_first_name, string person_second_name, int person_age)
    : first_name{person_first_name}, second_name{person_second_name}, age{person_age}
{
  check();
}

istream &Person::operator>>(istream &is)
{
  cout << "Enter first name, second name and age (separated by a space): ";
  is >> first_name >> second_name >> age;
  if (!is)
    error("Input error!");
  check();
  return is;
}

ostream &operator<<(ostream &os, const Person &p)
{
  os << p.get_first_name() << ' ' << p.get_second_name() <<" is " << p.get_age() << " years old.\n";
  return os;
}

int main()
{
  //Person p1{"Goofy", 63};
  //p1.operator>>(cin);
  //cout << p1;

  vector<Person> vp;
  for (int i = 0; i < 3; i++)
  {
    Person p;
    p.operator>>(cin);
    vp.push_back(p);
  }

  for (int i = 0; i < vp.size(); i++)
    cout << vp[i];

}
