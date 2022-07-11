#include "transport_catalogue.h" 

#include "input_reader.h" 

#include "stat_reader.h" 


using namespace transport_catalogue;


void InLines(TransportCatalogue& transportCatalogue)
{
	std::vector<std::string> data = input_requests_processing::ReadLines(stoi(input_requests_processing::ReadLine()));
	input_requests_processing::UpdateTransportCatalogue(transportCatalogue, data);
}

void OutLines(TransportCatalogue& transportCatalogue)
{
	std::vector<std::string> data = input_requests_processing::ReadLines(stoi(input_requests_processing::ReadLine()));
	output_requests_processing::TransportCatalogueRequests(transportCatalogue, data);
}

int main()
{
	TransportCatalogue transportCatalogue;
	InLines(transportCatalogue);
	OutLines(transportCatalogue);

	return 0;
}