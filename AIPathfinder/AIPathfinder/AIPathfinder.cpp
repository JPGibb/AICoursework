// AIPathfinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>

int number_of_caverns = 0;

int main()
{
	std::cout << "Program started!" << std::endl;

	std::ifstream input_file("../../generated30-1/generated30-1.cav"); //@cleanup set this to open the correct file location

	std::string line;

	if (input_file.is_open())
	{
		std::cout << "File opened" << std::endl; //@cleanup remove this

		while (!input_file.eof())
		{
			getline(input_file, line);
		}

		//std::vector<int> caves;
		std::vector<std::string> caves;
		std::string s;
		for (int i = 0; i < line.size(); ++i)
		{
			if (line[i] != ',')
			{
				s.push_back(line[i]);
			}
			else
			{
				
				caves.push_back(s);
				//caves.push_back(std::stoi(s));
				
			}
		}

		for (int i = 0; i < caves.size(); ++i)
		{
			std::cout << caves[i] << std::endl;
		}
		
	}
	else
	{
		std::cout << "Did not manage to open the file";//@cleanup remove this
	}

	return 0;
}
