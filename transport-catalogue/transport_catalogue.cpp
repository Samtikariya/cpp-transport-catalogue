#include "transport_catalogue.h"

namespace transport_catalogue
{
	void TransportCatalogue::AddBus(std::string busNumber, ERouteType routeType,
		size_t uniqueStops, size_t stopsOnRoute, unsigned int facticalRouteLength, double routeCurvature)
	{
		busesInfo_.emplace_back(Bus{ busNumber, routeType, uniqueStops, stopsOnRoute, facticalRouteLength, routeCurvature });
		buses_.emplace(std::move(busNumber), &busesInfo_.back());
	}

	void TransportCatalogue::AddStop(std::string name, double latitude, double longitude)
	{
		stopsCoords_.emplace_back(Stop{ latitude, longitude });
		stops_.emplace(std::move(name), &stopsCoords_.back());
	}

	void TransportCatalogue::AddBusRelatedToStop(const Stop* stopPtr, const std::string& busNumber)
	{
		busesRelatedToStop_[stopPtr].emplace(FindBus(busNumber));
	}

	const Bus* TransportCatalogue::FindBus(const std::string& name) const
	{
		auto it = buses_.find(name);
		return it == buses_.end() ? nullptr : it->second;
	}

	const Stop* TransportCatalogue::FindStop(const std::string& name) const
	{
		auto it = stops_.find(name);
		return it == stops_.end() ? nullptr : it->second;
	}

	std::tuple<size_t, size_t, unsigned int, double> TransportCatalogue::GetRouteInfo(const Bus* ptr) const
	{
		return std::make_tuple(ptr->uniqueStops, ptr->stopsOnRoute, ptr->facticalRouteLength, ptr->routeCurvature);
	}

	const std::set<const Bus*, CmpBuses>* TransportCatalogue::GetBusesRelatedToStop(const Stop* ptr) const
	{
		auto it = busesRelatedToStop_.find(ptr);
		return it == busesRelatedToStop_.end() ? nullptr : &it->second;
	}

	void TransportCatalogue::AddDistanceBetweenStops(const std::string& fromStop, const std::string& toStop, unsigned int distance)
	{
		stopsDistances_.emplace(std::make_pair(FindStop(fromStop), FindStop(toStop)), distance);
	}

	unsigned int TransportCatalogue::GetDistanceBetweenStops(const std::string& fromStop, const std::string& toStop)
	{
		auto itForward = stopsDistances_.find(std::make_pair(FindStop(fromStop), FindStop(toStop)));
		if (itForward == stopsDistances_.end())
		{
			auto itBack = stopsDistances_.find(std::make_pair(FindStop(toStop), FindStop(fromStop)));
			return itBack->second;
		}
		else
		{
			return itForward->second;
		}
	}
}