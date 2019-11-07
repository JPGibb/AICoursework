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

	int g = 0;
	int h = 0;
	int f = 0;

	int parent = 0;
	std::vector<int> connections;
};

void display_all_caverns();
std::vector<int> clean_input(std::string);
void build_matrix(std::vector<int>*);
void setup_caverns();
void a_star();
double calculate_distance(Cavern, Cavern);
bool check_list(std::vector<Cavern>*, int);
int get_lowest_f(std::vector<Cavern>*);

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

	display_all_caverns();
	//a_star();
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
	std::vector<int>& vector_ref = *cleaned_input;//dereference the pointer so that the indexes of the vector can be accessed
	for (int i = number_of_caverns * 2; i < cleaned_input->size(); ++i)
	{
		//std::cout << i <<std::endl;
		v.push_back(vector_ref[i]);
		std::cout << vector_ref[i] <<std::endl;
		if (v.size() == number_of_caverns)
		{
			table.push_back(v);
			v.clear();
		}
	}

	//set matrix vector to be conections for each to the caverns
	std::vector<int> temp;
	for (int i = 0; i < table.size(); ++i)
	{
		for (int j = 0; j < number_of_caverns -1; ++j)
		{
			temp.push_back(table[j][i]);
			if (temp.back() > 1)
			{
				std::cout << "??????\n";
			}
			std::cout << temp.back();
		}
		std::cout << std::endl;
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
	for (int i = 0; i < matrix.size(); ++i)
	{
		for (int j = 0; j < matrix.size(); ++j)
		{
			//std::cout << "i " << i << " j " << j << std::endl;
			if (matrix[i][j] == 1)
			{
				caverns[i].connections.push_back(j + 1);
				//std::cout << "c: " << caverns[i].cav_num << " conntects to " << j + 1 << std::endl;
			}
		}
	}

	/*for (int i = 0; i < number_of_caverns - 1; ++i)
	{
		for (int j = 0; j < number_of_caverns - 1; ++j)
		{
			if (matrix[i][j] == 1)
			{
				caverns[i].connections.push_back(j + 1);
				std::cout << "c: " << caverns[i].cav_num << " conntects to " << j + 1 << std::endl;
			}
		}
	}*/
}

void a_star()
{
	std::vector<Cavern> open_list;
	std::vector<Cavern> closed_list;

	open_list.push_back(caverns[0]);

	Cavern goal_node = caverns.back();

	bool found = false;
	while (open_list.size() != 0)
	{

		int current_index = get_lowest_f(&open_list);
		Cavern current = open_list[current_index];
		open_list.erase(open_list.begin() + current_index);
		closed_list.push_back(current);

		std::cout << current.parent << " -> " << current.cav_num << std::endl;


		if (current.cav_num == goal_node.cav_num)
		{
			break;
		}

		for (int i = 0; i < current.connections.size(); ++i)
		{

			//std::cout << current.connections[i] - 1 << std::endl;
			if (!check_list(&closed_list, current.connections[i]))
			{

				Cavern child = caverns[current.connections[i] - 1];

				child.g = current.g + calculate_distance(current, child);
				child.h = calculate_distance(child, goal_node);
				child.f = child.g + child.h;
				child.parent = current.cav_num;

				if (child.cav_num == 30)
				{
					std::cout << "end here" << std::endl;
				}

				if (check_list(&open_list, child.cav_num))
				{
					//std::cout << "in the open list already" << std::endl;
				}
				else
				{
					//std::cout << "inserting " << current.connections[i] << std::endl;
					open_list.push_back(child);
				}

				
			}
		}
		//break; //This will need to be removed later
	}
	if (found)
	{
		std::cout <<  "Found the goal node" << std::endl;
	}
	else
	{
		std::cout <<  "Did not find the goal node" << std::endl;
	}
	for (int i = 0; i < closed_list.size(); ++i)
	{
		std::cout << closed_list[i].cav_num << std::endl;
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

bool check_list(std::vector<Cavern>* list, int n)
{
	std::vector<Cavern>& vec_ref = *list;

	if (vec_ref.size() > 0)
	{
		for (int i = 0; i < vec_ref.size() - 1; ++i)
		{
			if (vec_ref[i].cav_num == n)
			{
				//found the value
				//std::cout << "found: " << n << std::endl;
				return true;
			}
		}
	}
	//std::cout << "not found " << n << std::endl;
	return false;
}

int get_lowest_f(std::vector<Cavern>* open_list)
{
	std::vector<Cavern>& vec_ref = *open_list;

	int lowest_f = 9999999999999;
	int lowest_index = 0;
	for (int i = 0; i < vec_ref.size(); ++i)
	{
		if (vec_ref[i].f < vec_ref[lowest_index].f)
		{
			lowest_f = vec_ref[i].f;
			lowest_index = i;
		}
	}

	return lowest_index;
}
