// AIPathfinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>

struct Cavern 
{
	int cav_num;
	int xcoord;
	int ycoord;
	std::vector<int> connections;
};

int number_of_caverns = 0;
std::vector<Cavern> caverns;
std::vector<int> coords;
//std::vector<int> matrix;
std::vector<std::vector<int>> matrix;

int main()
{
	std::ifstream input_file("../../generated30-1/generated30-1.cav"); //@cleanup set this to open the correct file location

	std::string line;

	if (input_file.is_open())
	{
		getline(input_file, line);

		std::vector<int> cleaned_input;
		std::string s;

		//separate all the values from the file by comma
		for (int i = 0; i < line.size(); ++i)
		{
			if (line[i] != ',')
			{
				s.push_back(line[i]);
			}
			else
			{
				cleaned_input.push_back(std::stoi(s));
				s = "";	
			}
		}

		number_of_caverns = cleaned_input[0];
		cleaned_input.erase(cleaned_input.begin());
		
		//Loop thorugh all the coords in the caverns vector
		for (int i = 0; i < number_of_caverns * 2; ++i)
		{
			coords.push_back(cleaned_input[i]);
		}

		//Put all the matrix values into the matrix vector
		//This creates a table similar to the one in coursework spec
		//Each vector within the matrix vector is a row in the table
		std::vector<int> v;
		for (int i = number_of_caverns * 2; i < cleaned_input.size(); ++i)
		{
			v.push_back(cleaned_input[i]);
			if (v.size() == 30)
			{
				matrix.push_back(v);
				v.clear(); 
			}
		}

		//Set up the Cavern structs
		int next_cav_num = 1;
		for (int i = 0; i < number_of_caverns * 2; i+=2)
		{
			struct Cavern c;
			c.cav_num = next_cav_num;
			c.xcoord = coords[i];
			c.ycoord = coords[i + 1];
			++next_cav_num;
			caverns.push_back(c);
		}
	}
	else
	{
		std::cout << "Did not manage to open the file";//@cleanup remove this
	}

	return 0;
}