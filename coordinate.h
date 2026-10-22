#ifndef COORDINATE_H
#define COORDINATE_H

//Each node holds one xy point and links to the nodes around
struct _coordinate
{
    float x;
    float y;
    int coord_id;
    struct _coordinate *next;
    struct _coordinate *previous;
};

//Just lets me write Coordinate instead of calling struct_coordinate
typedef struct _coordinate Coordinate;

void add_coordinate(Coordinate *list_end, float x, float y);
void forward_display(Coordinate *list_beginning);
void backward_display(Coordinate *list_end);
void delete_coordinate(Coordinate *list_beginning,
                       int coord_id_to_delete);
int list_length(Coordinate *list_beginning);
void closest_to(Coordinate *list_beginning, float x, float y);

#endif
