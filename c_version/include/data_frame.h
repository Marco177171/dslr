#ifndef DATA_FRAME_H
#define DATA_FRAME_H

typedef enum {
	DOUBLE,
	STRING,
	DATE,
	OBJECT
}	dtype;

typedef struct s_data_frame {
	int		valid;
	dtype	type;
	union {
		double d;
		char *s;
	};
}	t_data_frame;

t_data_frame*** get_data_frame(char *file);
void print_data_frame(t_data_frame*** df);
void free_data_frame(t_data_frame*** df);

#endif