// AIPathfinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<math.h>

struct Cavern
{
	int cav_num = 0;
	int xcoord = 0;
	int ycoord = 0;
	std::vector<int> connections;
};

void display_all_caverns();
std::vector<int> clean_input(std::string);
void build_matrix(std::vector<int>*);
void setup_caverns();
void a_star();
double calculate_distance(Cavern, Cavern);

int number_of_caverns = 0;
std::vector<Cavern> caverns;
std::vector<int> coords;
std::vector<std::vector<int>> matrix;

int main()
{
	std::ifstream input_file("../../generated30-1/generated30-1.cav"); //@cleanup set this to open the correct file location

	std::string line;

	if (input_file.is_open())
	{
		getline(input_file, line);

	
		std::vector<int>cleaned_input = clean_input(line);
		number_of_caverns = cleaned_input[0];
		cleaned_input.erase(cleaned_input.begin());
		
		//Loop thorugh all the coords in the caverns vector
		for (int i = 0; i < number_of_caverns * 2; ++i)
		{
			coords.push_back(cleaned_input[i]);
		}

		build_matrix(&cleaned_input);
		
		setup_caverns();
	}
	else
	{
		std::cout << "Did not manage to open the file";//@cleanup remove this
	}

	//display_all_caverns();
	a_star();
	return 0;
}

//Display all the cavern data
void display_all_caverns()
{
	for (int i = 0; i < caverns.size(); ++i) 
	{
		Cavern c = caverns[i];
		std::cout << "num: " << c.cav_num << " x " << c.xcoord << " y " << c.ycoord << " Connections ";
		for (int j = 0; j < c.connections.size(); ++j)
		{
			std::cout << c.connections[j] << " ";
		}
		std::cout << std::endl;
	}
}

//Separate all the values of the file by comma and place them in a vector
std::vector<int> clean_input(std::string line)
{
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

	return cleaned_input;
}

//Create a matrix of all the oonnections
void build_matrix(std::vector<int>* cleaned_input)
{
	//Put all the matrix values into the matrix vector
	//This creates a table similar to the one in coursework spec
	//Each vector within the table vector is a row in the table
	std::vector<int> v;
	std::vector<std::vector<int>> table;
	std::vector<int>& vector_ref = *cleaned_input;//dereference the poitner so that the indexes of the vector can be accessed
	for (int i = number_of_caverns * 2; i < cleaned_input->size(); ++i)
	{
		v.push_back(vector_ref[i]);
		if (v.size() == 30)
		{
			table.push_back(v);
			v.clear();
		}
	}

	//set matrix vector to be conections for each to the caverns
	std::vector<int> temp;
	for (int i = 0; i < table.size(); ++i)
	{
		for (int j = 0; j < number_of_caverns - 1; ++j)
		{
			temp.push_back(table[j][i]);
		}
		matrix.push_back(temp);
		temp.clear();
	}
}

//Set up caverns with ids, coords and connections
void setup_caverns()
{
	//Set up the Cavern structs with numbers and coords
	int next_cav_num = 1;
	for (int i = 0; i < number_of_caverns * 2; i += 2)
	{
		struct Cavern* c = new Cavern;
		c->cav_num = next_cav_num;
		c->xcoord = coords[i];
		c->ycoord = coords[i + 1];
		//std::cout << "num " << c->cav_num << " x " << c->xcoord << " y " << c->ycoord << std::endl;
		++next_cav_num;
		caverns.push_back(*c);
	}

	//set up cavern connections
	for (int i = 0; i < number_of_caverns - 1; ++i)
	{
		for (int j = 0; j < number_of_caverns - 1; ++j)
		{
			if (matrix[i][j] == 1)
			{
				caverns[i].connections.push_back(j + 1);
			}
		}
	}
}

void a_star()
{
	std::vector<Cavern> open_list;
	std::vector<Cavern> closed_list;

	Cavern current = caverns[0];
	open_list.push_back(current);

	Cavern goal_node = caverns.back();

	while (current.cav_num != goal_node.cav_num)
	{
		for (int i = 0; i < current.connections.size(); ++i)
		{
			std::cout << current.connections[i] - 1 << std::endl;
			open_list.push_back(caverns[current.connections[i] - 1]);
		}
		break; //This will need to be removed later
	}
	
}

double calculate_distance(Cavern a, Cavern b)
{
	double x_dist = 0;
	if (a.xcoord > b.xcoord) 
	{
		x_dist = a.xcoord - b.xcoord;
	}
	else
	{
		x_dist = b.xcoord - a.xcoord;
	}
	//std::cout << "xdist " << x_dist << std::endl;
	double y_dist = 0;
	if (a.ycoord > b.ycoord)
	{
		y_dist = a.ycoord - b.ycoord;
	}
	else
	{
		y_dist = b.ycoord - a.ycoord;
	}
	//std::cout << "ydist " << y_dist << std::endl;
	
	return sqrt((x_dist*x_dist) + (y_dist*y_dist));
}
