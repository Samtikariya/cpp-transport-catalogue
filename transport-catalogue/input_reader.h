#pragma once
#include "transport_catalogue.h"
#include <string>

namespace transport_catalogue
{
	namespace input_requests_processing
	{

		std::string ReadLine();
		std::vector<std::string> ReadLines(int);

		void UpdateTransportCatalogue(TransportCatalogue& transportCatalogue, std::vector<std::string>& lines);


		std::string WithdrawToken(std::string& line, const std::string& delimiter);

		double ComputeDistanceBetweenStopsInRoute(const TransportCatalogue& transportCatalogue,
			const std::string& prevStopName, const std::string& currStopName);
	}
}