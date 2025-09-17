#include "dslr.h"

// void print_matrix_column_vertically(t_data_frame ***df, int col_index) {
// 	for (int i = 0; df[i]; i++) {
// 		printf("%s\n", df[i][col_index]);
// 		i++;
// 	}
// }

// void print_matrix_vertically(t_data_frame ***df) {
// 	for (int i = 0; df[i]; i++) {
//         printf("Array at index %d:\n", i);
// 		print_array_vertically(df[i]);
// 		i++;
// 	}
// }

void print_matrix_row(t_data_frame **df)
{
	for (int i = 0; df[i]; i++) {
		if (df[i]->type == STRING) printf("%s ", df[i]->s);
		else printf("%f ", df[i]->d);
	}
}
