#pragma once
#include "geo.h"
#include <string>
#include <deque>
#include <unordered_map>
#include <tuple>
#include <set>
#include <utility>

namespace transport_catalogue
{
	enum class ERouteType
	{
		Circular,
		Pendulum
	};

	struct Stop
	{
		Coordinates coords;
	};

	struct Bus
	{
		std::string name;
		ERouteType routeType;
		size_t uniqueStops;
		size_t stopsOnRoute;
		unsigned int facticalRouteLength;
		double routeCurvature;
	};


	struct CmpBuses
	{
	public:
		bool operator() (const Bus* lhs, const Bus* rhs) const
		{
			return lhs->name < rhs->name;
		}
	};

	// Используем "magic" hashing constants 0x9e3779b9
	struct PairHash
	{
		template <class T1, class T2>
		std::size_t operator() (const std::pair<T1, T2>& pair) const
		{
			std::size_t seed = 0;
			auto hash1 = std::hash<const void*>{}(pair.first);
			auto hash2 = std::hash<const void*>{}(pair.second);
			seed ^= hash1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= hash2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};

	class TransportCatalogue
	{
	private:
		std::unordered_map<std::string, const Bus*> buses_;
		std::deque<Bus> busesInfo_;
		std::unordered_map<std::string, const Stop*> stops_;
		std::deque<Stop> stopsCoords_;
		std::unordered_map<const Stop*, std::set<const Bus*, CmpBuses>> busesRelatedToStop_;
		std::unordered_map<std::pair<const Stop*, const Stop*>, unsigned int, PairHash> stopsDistances_;


	public:
		void AddBus(std::string busNumber, ERouteType routeType, size_t uniqueStops, size_t stopsOnRoute,
			unsigned int facticalRouteLength, double routeCurvature);
		void AddStop(std::string name, double latitude, double longitude);
		void AddBusRelatedToStop(const Stop* stopPtr, const std::string& busNumber);
		const Bus* FindBus(const std::string& name) const;
		const Stop* FindStop(const std::string& name) const;
		std::tuple<size_t, size_t, unsigned int, double> GetRouteInfo(const Bus* ptr) const;
		const std::set<const Bus*, CmpBuses>* GetBusesRelatedToStop(const Stop* ptr) const;

		void AddDistanceBetweenStops(const std::string& fromStop, const std::string& toStop, unsigned int distance);
		unsigned int GetDistanceBetweenStops(const std::string& fromStop, const std::string& toStop);
	};
}