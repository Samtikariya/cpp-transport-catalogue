// напишите решение с нуля
// код сохраните в свой git-репозиторий
#include "input_reader.h"
#include "geo.h"
#include <iostream>
#include <unordered_set>
#include <forward_list>

namespace transport_catalogue
{
	namespace input_requests_processing
	{
		using namespace std::string_literals;


		std::string WithdrawToken(std::string& line, const std::string& delimiter)
		{
			size_t pos = line.find(delimiter);
			std::string token = std::move(line.substr(0, pos));
			line.erase(0, pos == std::string::npos ? pos : pos + delimiter.length());
			return token;
		}

		double ComputeDistanceBetweenStopsInRoute(const TransportCatalogue& transportCatalogue,
			const std::string& prevStopName, const std::string& currStopName)
		{
			return ComputeDistance(transportCatalogue.FindStop(prevStopName)->coords,
				transportCatalogue.FindStop(currStopName)->coords);
		}

		void UpdateTransportCatalogue(TransportCatalogue& transportCatalogue)
		{
			// Обрабатываем остановки
			std::forward_list<std::pair<std::string, std::string>> stopsRequests;
			std::forward_list<std::string> busRequests;
			std::string line;
			std::getline(std::cin, line);
			size_t numberOfRequests = std::stoul(line);
			for (size_t i = 0; i < numberOfRequests; ++i)
			{
				std::getline(std::cin, line);
				std::string requestType = /*detail::*/WithdrawToken(line, " "s);

				if (requestType == "Stop"s)
				{
					std::string stopName = WithdrawToken(line, ": "s);
					double latitude = std::stod(WithdrawToken(line, ", "s));
					double longitude = std::stod(WithdrawToken(line, ", "s));
					transportCatalogue.AddStop(stopName, latitude, longitude);
					stopsRequests.emplace_front(std::move(stopName), std::move(line));
				}
				else if (requestType == "Bus"s)
				{
					busRequests.emplace_front(std::move(line));
				}
			}

			// Обрабатываем расстояние между остановками
			for (auto& stopRequest : stopsRequests)
			{
				while (stopRequest.second.find("m "s) != std::string::npos)
				{
					unsigned int distance = std::stoul(WithdrawToken(stopRequest.second, "m to "s));
					std::string toStop = WithdrawToken(stopRequest.second, ", "s);
					transportCatalogue.AddDistanceBetweenStops(stopRequest.first, toStop, distance);
				}
			}

			// Обрабатываем маршруты автобусов
			for (std::string& busRequest : busRequests)
			{
				std::string busNumber = WithdrawToken(busRequest, ": "s);
				ERouteType routeType;
				std::string delimiter;
				std::string firstStopName;
				if (busRequest.find(" - "s) == std::string::npos)
				{
					routeType = ERouteType::Circular;
					delimiter = " > "s;
				}
				else
				{
					routeType = ERouteType::Pendulum;
					delimiter = " - "s;
				}
				std::unordered_set<const Stop*> route;
				double geographicRouteLength = 0.0;
				unsigned int facticalRouteLength = 0;
				std::string prevStopName;
				std::string currStopName;
				size_t stopsOnRoute = 0;
				for (; busRequest.find(delimiter) != std::string::npos; ++stopsOnRoute)
				{
					prevStopName = std::move(currStopName);
					currStopName = WithdrawToken(busRequest, delimiter);
					route.emplace(transportCatalogue.FindStop(currStopName));
					if (stopsOnRoute)
					{
						geographicRouteLength += ComputeDistanceBetweenStopsInRoute(transportCatalogue, prevStopName, currStopName);;
						facticalRouteLength += transportCatalogue.GetDistanceBetweenStops(prevStopName, currStopName);
						if (routeType == ERouteType::Pendulum)
						{
							facticalRouteLength += transportCatalogue.GetDistanceBetweenStops(currStopName, prevStopName);
						}
					}
					else
					{
						firstStopName = currStopName;
					}
				}

				if (routeType == ERouteType::Circular)
				{
					++stopsOnRoute;
					geographicRouteLength += ComputeDistanceBetweenStopsInRoute(transportCatalogue, currStopName, firstStopName);
					facticalRouteLength += transportCatalogue.GetDistanceBetweenStops(currStopName, firstStopName);
				}
				else if (routeType == ERouteType::Pendulum)
				{
					stopsOnRoute = stopsOnRoute * 2 + 1;
					prevStopName = std::move(currStopName);
					currStopName = WithdrawToken(busRequest, delimiter);
					route.emplace(transportCatalogue.FindStop(currStopName));
					geographicRouteLength += ComputeDistanceBetweenStopsInRoute(transportCatalogue, prevStopName, currStopName);
					facticalRouteLength += transportCatalogue.GetDistanceBetweenStops(prevStopName, currStopName) +
						transportCatalogue.GetDistanceBetweenStops(currStopName, prevStopName);
					geographicRouteLength *= 2;
				}

				transportCatalogue.AddBus(busNumber, routeType, route.size(), stopsOnRoute, facticalRouteLength,
					static_cast<double>(facticalRouteLength) / geographicRouteLength);

				for (const auto& stop : route)
				{
					transportCatalogue.AddBusRelatedToStop(stop, busNumber);
				}
			}
		}
	}
}