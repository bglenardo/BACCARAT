

#ifndef XMLTOVECTOR_HH
#define XMLTOVECTOR_HH 1

//C++ includes
#include <string>
#include <vector>
//#include <cstdlib>

std::vector<double> XMLtoVectorDbl(std::string xml, std::string search_term);
std::vector<int> XMLtoVectorInt(std::string xml, std::string search_term);

#endif
