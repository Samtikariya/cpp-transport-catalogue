#include "geo.h"

using namespace geo;

static const int earth_radius = 6371000;
static const double dr = 3.1415926535 / 180.;

namespace geo
{

	bool Coordinates::operator==(const Coordinates& other) const
	{
		return lat == other.lat && lng == other.lng;
	}

	bool Coordinates::operator!=(const Coordinates& other) const
	{
		return !(*this == other);
	}

	double ComputeDistance(Coordinates from, Coordinates to)
	{
		using namespace std;
		if (from == to) {
			return 0;
		}
		return acos(sin(from.lat * dr) * sin(to.lat * dr)
			+ cos(from.lat * dr) * cos(to.lat * dr) * cos(abs(from.lng - to.lng) * dr))
			* earth_radius;
	}

} // завершение namespace geo