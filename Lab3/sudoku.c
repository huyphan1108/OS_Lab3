#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMBER_OF_THREADS 11
#define PUZZLE_SIZE 9

typedef struct {
	int row;
	int col;
} parameter;

bool validate[11];
int subgrid_validate = 2;

int puzzle[9][9] = {
	{5,3,4,6,7,8,9,1,2},
	{6,7,2,1,9,5,3,4,8},
	{1,9,8,3,4,2,5,6,7},
	{8,5,9,7,6,1,4,2,3},
	{4,2,6,8,5,3,7,9,1},
	{7,1,3,9,2,4,8,5,6},
	{9,6,1,5,3,7,2,8,4},
	{2,8,7,4,1,9,6,3,5},
	{3,4,5,2,8,6,1,7,9}
};

void *check_column(void* param) {
	parameter* col = (parameter*)param;
	int initial_col = col->col;
	int initial_row = col->row;

	for (int i = initial_col; i < PUZZLE_SIZE; i++) {
		int column[9] = { 0 };
		for (int j = initial_row; j < PUZZLE_SIZE; j++) {
			int temp = puzzle[i][j];
			if (column[temp - 1] == 0 && temp > 0) {
				column[temp - 1] = temp;
			}
			else if (temp <= 0) {
				validate[0] = 0;
				printf("Invalid Sudoku");
				pthread_exit(0);
			}
		}
	}
	validate[0] = 1;
	pthread_exit(0);
}

void* check_row(void* param) {
	parameter* row = (parameter*)param;
	int initial_col = row->col;
	int initial_row = row->row;

	for (int i = initial_col; i < PUZZLE_SIZE; i++) {
		int rows[9] = 0;
		for (int j = initial_row; j < PUZZLE_SIZE; j++) {
			int temp = puzzle[i][j];
			if (rows[temp - 1] == 0 && temp > 0) {
				rows[temp - 1] = temp;
			}
			else if (temp <= 0) {
				validate[0] = 0;
				printf("Invalid Sudoku");
				pthread_exit(0);
			}
		}
	}
	validate[1] = 1;
	pthread_exit(0);
}


void* check_subgrid(void* param) {
	parameter* subgrid = (parameter*)param;
	int initial_col = subgrid->col;
	int initial_row = subgrid->row;
	int square[PUZZLE_SIZE] = { 0 };

	for (int i = initial_row; i < initial_col + 3; i++) {
		for (int j = initial_col; j < initial_col + 3; j++) {
			int temp = puzzle[i][j];
			if (square[temp - 1] == 0 && temp > 0) {
				square[temp - 1] = temp;
			}
			else {
				validate[subgrid_validate] = 0;
				subgrid_validate++;
				pthread_exit(0);
			}
		}
	}
	validate[subgrid_validate] = 1;
	subgrid_validate++;
	pthread_exit(0);
}

int main() {
	parameter* check_row = (parameter*)malloc(sizeof(parameter));
	check_row->row = 0;
	check_row->col = 0;

	parameter* check_column = (parameter*)malloc(sizeof(parameter));
	check_column->row = 0;
	check_column->col = 0;


}