/*
#include <iostream>
#include <string>
#include <sstream>
#include "Tools.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <vector>

std::map<std::string,std::string> values;


int main() {

	std::cout <<"hello"<< std::endl;
	system("pause");
}


/*
int main() {
	char * file = Tools::LoadWholeStringFile("../data/Fonts/bodoni_mt_black.fnt");
	std::stringstream ss(file);
	std::string line;


	if (file != NULL) {
		while (std::getline(ss, line, '\n')) {
			std::stringstream ss2(line);
			std::string word;

			while (std::getline(ss2, word, ' ')) {
				std::stringstream ss3(word);
				std::string to;
				std::string arr[2];
				std::vector<std::string> vec;

				while (std::getline(ss3, to, '=')) {
					vec.push_back(to);
				}
				if (vec.size() == 2) {
					values.insert(std::pair<std::string, std::string>(vec.at(0),vec.at(1)));
				}
			}
		}
	}

	for (auto elem : values)
	{
		std::cout << elem.first << " " << elem.second<< "\n";
	}


	system("pause");
	return 0;
}
*/
