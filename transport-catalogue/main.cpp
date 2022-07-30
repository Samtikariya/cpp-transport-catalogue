#include <iostream>
#include <fstream>
#include <string>
#include "transport_catalogue.h"
#include "json_reader.h"
#include "svg.h"

#include <iostream>

using namespace std;
using namespace std::literals;
using namespace transport;

int main()
{
	std::ifstream in_file("C:\\Yandex Practicum\\S10 Final project\\input1.txt");
	std::ofstream out_file("C:\\Yandex Practicum\\S10 Final project\\out1.txt");
	TransportCatalogue transport_cat;
	json_reader::Reader reader(transport_cat);
	reader.ReadJSON(in_file);
	reader.ParseRequests();
	reader.GetResponses(out_file);
}