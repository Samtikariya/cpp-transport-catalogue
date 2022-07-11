#include "stat_reader.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <list>

namespace transport_catalogue
{
	namespace output_requests_processing
	{
		using namespace std::string_literals;

		void TransportCatalogueRequests(const TransportCatalogue& transportCatalogue, std::vector<std::string> requests)
		{
			for (std::string& request : requests)
			{
				std::string delimiter = " "s;
				size_t pos = request.find(delimiter);
				std::string requestName = request.substr(0, pos);
				std::string requestParam = request.substr(pos + delimiter.length());
				if (requestName == "Bus"s)
				{
					const Bus* bus = transportCatalogue.FindBus(requestParam);
					if (bus != nullptr)
					{
						auto [uniqueStops, stopsOnRoute, routeLength, curvature] = transportCatalogue.GetRouteInfo(bus);
						std::cout << std::setprecision(6) << "Bus " << requestParam << ": " << stopsOnRoute << " stops on route, "
							<< uniqueStops << " unique stops, " << routeLength << " route length, "
							<< curvature << " curvature" << std::endl;
					}
					else
					{
						std::cout << "Bus " << requestParam << ": not found" << std::endl;
					}
				}
				else if (requestName == "Stop"s)
				{
					const Stop* stop = transportCatalogue.FindStop(requestParam);
					if (stop != nullptr)
					{
						auto busesByStop = transportCatalogue.GetBusesRelatedToStop(stop);
						if (busesByStop != nullptr)
						{
							std::cout << "Stop " << requestParam << ": buses";
							for (const auto& bus : *busesByStop)
							{
								std::cout << " " << bus->name;
							}
							std::cout << std::endl;
						}
						else
						{
							std::cout << "Stop " << requestParam << ": no buses" << std::endl;
						}
					}
					else
					{
						std::cout << "Stop " << requestParam << ": not found" << std::endl;
					}
				}
			}
		}
	}
}