// Student ID Code Block
// Programmer: Jonathan Kim
// Programmer's ID: 1780039

// C++ Libraries
#include <iostream>
#include <vector>
#include <set>
#include <stdexcept>
#include <algorithm>
using namespace std;

// C Libraries
#include <cstring>

class Leg
{
  const string startCity;
  const string endCity;
  const int distance;

public:
  Leg(const string &start, const string &end, const double dist) : startCity(start), endCity(end), distance(dist) {}
  Leg &operator=(const Leg &); // for swap()
  int getDistance() const { return distance; }
  friend class Route;
  friend class ShortestRoute;
};

class Route
{
  vector<const Leg *> legs; // bag of legs
  const int totalDis;

public:
  Route(const Leg &);
  Route(const Route &, const Leg &);
  Route &operator=(const Route &);
  int getDistance() const { return totalDis; }
  void output(ostream &) const;
  bool isEmpty() const { return legs.empty(); } // Method to check if route is empty
  friend bool operator<(const Route &, const Route &);
};

class ShortestRoute
{
  static const int legSize;
  static const Leg LEGS[];

public:
  static const Route anyRoute(const char *const, const char *const);
  static const Route shortestRoute(const char *const, const char *const);
  static void findRoutes(const string &, const char *const, const Route &, set<Route> &, int);
};

// Function Prototypes
void IDCodeBlock();

int main()
{
  IDCodeBlock();

  try
  {
    const Route anyRoute = ShortestRoute::anyRoute("San Francisco", "New York City");
    cout << "Any Route: ";
    anyRoute.output(cout);

    const Route shortestRoute = ShortestRoute::shortestRoute("San Francisco", "New York City");
    cout << "Shortest Route: ";
    shortestRoute.output(cout);
  }
  catch (const runtime_error &e)
  {
    cerr << "Error: " << e.what() << endl;
  }

  return 0;
}

void IDCodeBlock()
{
  cout << "Programmer: Jonathan Kim" << endl;
  cout << "Programmer's ID: 1780039" << endl;
  cout << "File: " << __FILE__ << endl;
}

const Leg ShortestRoute::LEGS[] = {
    Leg("Fargo", "Chicago", 569),
    Leg("Memphis", "Washington", 763),
    Leg("Chicago", "Pittsburgh", 409),
    Leg("Pittsburgh", "New York City", 315),
    Leg("Helena", "Miles City", 294),
    Leg("Denver", "Omaha", 484),
    Leg("Houston", "Memphis", 485),
    Leg("Memphis", "Philadelphia", 879),
    Leg("Orlando", "Washington", 759),
    Leg("Washington", "New York City", 203),
    Leg("Reno", "Phoenix", 601),
    Leg("Omaha", "St. Louis", 357),
    Leg("San Francisco", "Portland", 536),
    Leg("Reno", "Salt Lake City", 427),
    Leg("Las Angeles", "Phoenix", 357),
    Leg("New Orleans", "Orlando", 533),
    Leg("Denver", "Oklahoma City", 504),
    Leg("San Francisco", "Reno", 186),
    Leg("Phoenix", "El Paso", 345),
    Leg("El Paso", "Houston", 674),
    Leg("St. Louis", "Philadelphia", 809),
    Leg("Chicago", "Cleveland", 307),
    Leg("Minneapolis", "Cleveland", 630),
    Leg("Miles City", "Omaha", 606),
    Leg("Portland", "Helena", 516),
    Leg("Phoenix", "Denver", 586),
    Leg("Fargo", "Minneapolis", 214),
    Leg("Salt Lake City", "Miles City", 494),
    Leg("Omaha", "Chicago", 435),
    Leg("Chicago", "Philadelphia", 664),
    Leg("El Paso", "Oklahoma City", 576),
    Leg("San Francisco", "Las Angeles", 352),
    Leg("Salt Lake City", "Denver", 371),
    Leg("Houston", "New Orleans", 318),
    Leg("Oklahoma City", "Memphis", 421),
    Leg("Helena", "Fargo", 721),
    Leg("Memphis", "Orlando", 684),
    Leg("Philadelphia", "New York City", 80),
    Leg("Miles City", "Fargo", 430),
    Leg("San Francisco", "New York City", 21000)};

void ShortestRoute::findRoutes(const string &currentCity, const char *const to, const Route &currentRoute, set<Route> &allRoutes, int currentDistance)
{
  // Base case: If the current city is the destination, add the route to the set if it's the shortest so far
  if (currentCity == to)
  {
    allRoutes.insert(currentRoute);
    return;
  }

  // Iterate through all legs
  for (const Leg &leg : LEGS)
  {
    // If the starting city matches the current city, continue searching
    if (leg.startCity == currentCity)
    {
      int newDistance = currentDistance + leg.getDistance();
      Route newRoute(currentRoute, leg);

      // Only explore this path if the new distance is less than or equal to the current shortest distance in the set
      if (allRoutes.empty() || newDistance <= allRoutes.begin()->getDistance())
      {
        findRoutes(leg.endCity, to, newRoute, allRoutes, newDistance);
      }
    }
  }
}

const Route ShortestRoute::anyRoute(const char *const from, const char *const to)
{
  // Create a set to store all possible routes
  set<Route> allRoutes;

  // Find the starting leg from San Francisco
  for (const Leg &leg : LEGS)
  {
    if (leg.startCity == from)
    {
      findRoutes(leg.endCity, to, Route(leg), allRoutes, leg.getDistance());
    }
    if (!allRoutes.empty())
    {
      // Check if superhighway is returned as the solution
      if (allRoutes.begin()->getDistance() == 21000)
      {
        throw runtime_error("Superhighway found in the route. Check database.");
      }
      return *allRoutes.begin();
    }
  }
  // If no route is found, throw an exception
  throw runtime_error("No route found between the cities");
}

const Route ShortestRoute::shortestRoute(const char *const from, const char *const to)
{
  // Create a set to store all possible routes
  set<Route> allRoutes;

  // Find the starting leg from San Francisco
  for (const Leg &leg : LEGS)
  {
    if (leg.startCity == from)
    {
      findRoutes(leg.endCity, to, Route(leg), allRoutes, leg.getDistance());
    }
  }

  // If no route is found, throw an exception
  if (allRoutes.empty())
  {
    throw runtime_error("No route found between the cities");
  }

  // Check if superhighway is returned as the solution
  if (allRoutes.begin()->getDistance() == 21000)
  {
    throw runtime_error("Superhighway found in the route. Check database.");
  }

  // Return the route with the minimum distance from the set
  return *allRoutes.begin();
}

Route::Route(const Leg &leg) : totalDis(leg.getDistance())
{
  legs.push_back(&leg);
}

Route::Route(const Route &prevRoute, const Leg &leg) : legs(prevRoute.legs), totalDis(prevRoute.totalDis + leg.getDistance())
{
  legs.push_back(&leg);
}

Route &Route::operator=(const Route &other)
{
  if (this != &other)
  {
    legs = other.legs;
  }
  return *this;
}

void Route::output(ostream &os) const
{
  os << "Route: ";
  for (size_t i = 0; i < legs.size(); ++i)
  {
    os << legs[i]->startCity << " -> ";
    if (i == legs.size() - 1)
      os << legs[i]->endCity;
  }
  os << " Total Distance: " << totalDis << " miles" << endl;
}

bool operator<(const Route &r1, const Route &r2)
{
  return r1.totalDis < r2.totalDis;
}