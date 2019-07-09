#ifndef GRID_H
#define GRID_H

class Grid
{
public:
	int row;
	int col;

	Grid(int _row = 0, int _col = 0):
		row(_row), col(_col) {};

	~Grid()	{};

	//Operators
	bool operator ==(const Grid& grid2)
	{
		return grid2.row == row && grid2.col == col;
	}

	bool operator !=(const Grid& grid2)
	{
		return grid2.row != row || grid2.col != col;
	}

	bool operator <(const Grid& grid2)
	{
		return row < grid2.row && col < grid2.col;
	}

	bool operator >(const Grid& grid2)
	{
		return row > grid2.row && col > grid2.col;
	}

	bool operator <=(const Grid& grid2)
	{
		return row <= grid2.row && col <= grid2.col;
	}

	bool operator >=(const Grid& grid2)
	{
		return row >= grid2.row && col >= grid2.col;
	}
};

#endif