#pragma once
#include "transport_catalogue.h"
#include <string>

namespace transport_catalogue
{
	namespace input_requests_processing
	{
		void UpdateTransportCatalogue(TransportCatalogue& transportCatalogue);


		std::string WithdrawToken(std::string& line, const std::string& delimiter);

		double ComputeDistanceBetweenStopsInRoute(const TransportCatalogue& transportCatalogue,
			const std::string& prevStopName, const std::string& currStopName);
	}
}