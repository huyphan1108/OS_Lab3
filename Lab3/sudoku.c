#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMBER_OF_THREADS 11
#define PUZZLE_SIZE 9

typedef struct
{
	int row;
	int col;
	int tnum;
} parameter;

int map[11];
int subgrid_map = 2; // initial subgrid check index

int puzzle[9][9] = {
	{5, 3, 4, 6, 7, 8, 9, 1, 2},
	{6, 7, 2, 1, 9, 5, 3, 4, 8},
	{1, 9, 8, 3, 4, 2, 5, 6, 7},
	{8, 5, 9, 7, 6, 1, 4, 2, 3},
	{4, 2, 6, 8, 5, 3, 7, 9, 1},
	{7, 1, 3, 9, 2, 4, 8, 5, 6},
	{9, 6, 1, 5, 3, 7, 2, 8, 4},
	{2, 8, 7, 4, 1, 9, 6, 3, 5},
	{3, 4, 5, 2, 8, 6, 1, 7, 9}};

void *check_column(void *param)
{
	parameter *col = (parameter *)param;
	int initial_col = col->col;
	int initial_row = col->row;

	for (int i = initial_col; i < PUZZLE_SIZE; i++)
	{
		int column[PUZZLE_SIZE] = {0};
		for (int j = initial_row; j < PUZZLE_SIZE; j++)
		{
			int temp = puzzle[i][j];
			if (column[temp - 1] == 0 && temp > 0)
			{
				column[temp - 1] = temp;
			}
			else
			{
				map[0] = 0;
				pthread_exit(0);
			}
		}
	}
	map[0] = 1;
	pthread_exit(0);
}

void *check_row(void *param)
{
	parameter *row = (parameter *)param;
	int initial_col = row->col;
	int initial_row = row->row;

	for (int i = initial_col; i < PUZZLE_SIZE; i++)
	{
		int rows[PUZZLE_SIZE] = {0};
		for (int j = initial_row; j < PUZZLE_SIZE; j++)
		{
			int temp = puzzle[i][j];
			if (rows[temp - 1] == 0 && temp > 0)
			{
				rows[temp - 1] = temp;
			}
			else
			{
				map[1] = 0;
				pthread_exit(0);
			}
		}
	}
	map[1] = 1;
	pthread_exit(0);
}

void *check_subgrid(void *param)
{
	parameter *subgrid = (parameter *)param;
	int initial_col = subgrid->col;
	int initial_row = subgrid->row;
	int square[PUZZLE_SIZE] = {0};

	for (int i = initial_row; i < initial_col + 3; i++)
	{
		for (int j = initial_col; j < initial_col + 3; j++)
		{
			int temp = puzzle[i][j];
			if (square[temp - 1] == 0 && temp > 0)
			{
				square[temp - 1] = temp;
			}
			else
			{
				map[subgrid_map] = 0;
				subgrid_map++;
				pthread_exit(0);
			}
		}
	}
	map[subgrid_map] = 1;
	subgrid_map++;
	pthread_exit(0);
}

int main()
{

	// pthread_t columns, rows;
	pthread_t workers[NUMBER_OF_THREADS];
	int count = 0;

	parameter *data = (parameter *)malloc(sizeof(parameter));
	data->tnum = count;
	data->row = 0;
	data->col = 0;
	pthread_create(&workers[data->tnum], NULL, check_row, data);
	count++;

	data = (parameter *)malloc(sizeof(parameter));
	data->row = 0;
	data->col = 0;
	data->tnum = count;
	pthread_create(&workers[data->tnum], NULL, check_column, data);

	for (int r = 0; r < 7; r += 3)
	{
		for (int c = 0; c < 7; c += 3)
		{
			data->row = r;
			data->col = c;
			data->tnum = count;
			// printf("%d\n", data->tnum);
			pthread_create(&workers[data->tnum], NULL, check_subgrid, data);
			count++;
		}
	}

	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		pthread_join(workers[i], NULL);
	}

	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		if (map[i] != 1)
		{
			printf("Sudoku is not valid\n");
			exit(0);
		}
	}
	printf("Sudoku is valid\n");
	return 0;
}