#include "maze_control.h"

extern handler(ogg_grid_canvas, OGG_PAINT_EVENT);

handler(maze_control, OGG_MOUSE_MOVE_EVENT);

handler(maze_control, OGG_MOUSE_EVENT);

def_vtable(maze_control)(
    [OGG_PAINT_EVENT] = ogg_handler(ogg_grid_canvas, OGG_PAINT_EVENT),
    [OGG_MOUSE_MOVE_EVENT] = ogg_handler(maze_control, OGG_MOUSE_MOVE_EVENT),
    [OGG_MOUSE_EVENT] = ogg_handler(maze_control, OGG_MOUSE_EVENT),
    [OGG_MOUSE_DRAG_EVENT] = ogg_handler(maze_control, OGG_MOUSE_MOVE_EVENT),
);

//default_startup_inh(maze_control, ogg_grid_canvas)(
//    .src_color = OGG_RED,
//    .dest_color = OGG_BLUE,
//)

def_constructor(maze_control, parent != 0)
{
    memcpy(&self->src_color, &args->src_color, sizeof(ogg_color));
    memcpy(&self->dest_color, &args->dest_color, sizeof(ogg_color));
    self->cursor.x = self->cursor.y = -1;
    self->labyrinth = 0;
    self->set_src = ogg_true;
}

def_destructor(maze_control)
{
    if (self->labyrinth) {
        free(self->labyrinth->data);
        free(self->labyrinth);
    }
}

static void update_maze(maze_control* self)
{
    coordinate pos;
    map* maze = self->labyrinth;
    if (maze) {
        for (pos.y = 0; pos.y != maze->height; ++pos.y) {
            for (pos.x = 0; pos.x != maze->width; ++pos.x) {
                switch (get_land_type(maze, pos)) {
                case ROAD: case BEEN_ROAD: 
                    set_canvas_color(self, coord(pos.x, pos.y), light_green); break;
                case WALL: 
                    set_canvas_color(self, coord(pos.x, pos.y), green); break;
                case LAND: 
                    set_canvas_color(self, coord(pos.x, pos.y), green/*OGG_GREEN*/); break;
                case ENTRANCE: 
                    set_canvas_color(self, coord(pos.x, pos.y), OGG_RED); break;
                case EXIT: 
                    set_canvas_color(self, coord(pos.x, pos.y), OGG_BLUE/*OGG_YELLOW*/); break;
                case SOLUTION_ROAD:
                    set_canvas_color(self, coord(pos.x, pos.y), OGG_YELLOW/*OGG_BLACK*//*ogg_make_color(227, 76, 5)*/); break;
                default: 
                    set_canvas_color(self, coord(pos.x, pos.y), OGG_CYAN);
                }
            }
        }
    }
    ogg_send_event(self, OGG_PAINT_EVENT);
    ogg_flush_screen();
}

def_handler(maze_control, OGG_MOUSE_MOVE_EVENT)
{
    if (self->labyrinth) {
        ogg_anchor anchor;
        get_component_real_coord_anchor(self, &anchor);
        const int width = anchor.coord.right - anchor.coord.left,
                height = anchor.coord.bottom - anchor.coord.top;
        const int xpos = x - anchor.coord.left,
                ypos = y - anchor.coord.top;
        coordinate pix = make_coordinate(
                (int)(xpos / ((float)width / self->super.size.x)),
                (int)(ypos / ((float)height / self->super.size.y))
            );
        if (is_in_map(self->labyrinth, pix)) {
            int land_type = get_land_type(self->labyrinth, pix);
            if (land_type == ROAD || land_type > 100) {
                if (self->cursor.x != pix.x || self->cursor.y != pix.y) {
                    if (~self->cursor.x && ~self->cursor.y) {
                        memcpy(&self->super.sub[self->super.size.x * self->cursor.y + self->cursor.x],
                            &self->temp_unit, 
                            sizeof(self->temp_unit));
                        paint_grid_canvas_elem(self, self->cursor);
                    }
                    memcpy(&self->temp_unit,
                        &self->super.sub[self->super.size.x * pix.y + pix.x],
                        sizeof(self->temp_unit));
                    self->cursor.x = pix.x; self->cursor.y = pix.y;
                    set_canvas_color(self, self->cursor, self->set_src ? 
                        self->src_color : self->dest_color);
                    paint_grid_canvas_elem(self, self->cursor);
                }
            }
        }
    }
}

def_handler(maze_control, OGG_MOUSE_EVENT)
{
    if (self->labyrinth) {
        if (self->set_src = !self->set_src) {
            set_land_type(self->labyrinth, make_coordinate(
                    self->dest.x, self->dest.y), ROAD);
            set_land_type(self->labyrinth, make_coordinate(
                    self->cursor.x, self->cursor.y), EXIT);
            set_exit(self->labyrinth, make_coordinate(
                    self->cursor.x, self->cursor.y));
            self->dest.x = self->cursor.x; self->dest.y = self->cursor.y;
        } else {
            set_land_type(self->labyrinth, make_coordinate(
                    self->src.x, self->src.y), ROAD);
            set_land_type(self->labyrinth, make_coordinate(
                    self->cursor.x, self->cursor.y), ENTRANCE);
            set_entrance(self->labyrinth, make_coordinate(
                    self->cursor.x, self->cursor.y));
            self->src.x = self->cursor.x; self->src.y = self->cursor.y;
        }
        self->cursor.x = self->cursor.y = -1;
        update_maze(self);
        ogg_send_event(self, OGG_PAINT_EVENT);
        ogg_flush_screen();
    }
}

static map* dfs_maze;

const int dx[] = { 1, -1, 0, 0 }, dy[] = { 0, 0, 1, -1 },
        dsize = sizeof(dx) / sizeof(int);

static ogg_bool dfs(coordinate pix)
{
    set_land_type(dfs_maze, pix, BEEN_ROAD);
    int i = 0;
    for (; i != dsize; ++i) {
        coordinate pos = make_coordinate(pix.x + dx[i], pix.y + dy[i]);
        if (is_in_map(dfs_maze, pos)) {
            switch (get_land_type(dfs_maze, pos)) {
            case ROAD:
                if (!dfs(pos)) break;
            case EXIT:
                set_land_type(dfs_maze, pos, SOLUTION_ROAD);
                return ogg_true;
            }
        }
    }
    return ogg_false;
}

void make_dir(ogg_com_ptr self)
{
    map* maze = dfs_maze = ((maze_control*)self)->labyrinth;
    if (!maze) return;
    int i = 0;
    for (; i != maze->width * maze->height; ++i) {
        switch (maze->data[i].type) {
        case BEEN_ROAD: case SOLUTION_ROAD: 
        case ENTRANCE: case EXIT:
            maze->data[i].type = ROAD;
        }
    }
    //set_land_type(labyrinth, src, ENTRANCE);
    set_land_type(maze, maze->exit_position, EXIT);
    dfs(maze->entrance_position);
    set_land_type(maze, maze->entrance_position, ENTRANCE);
    set_land_type(maze, maze->exit_position, EXIT);
    maze_control* p = ((maze_control*)self);
    p->cursor.x = p->cursor.y = -1;
    p->set_src = ogg_true;
    update_maze(p);
}

coordinate make_coordinate(int x, int y)
{
    coordinate r = { x, y };
    return r;
}

void generate_maze(ogg_com_ptr self, map* (*gen)(), int width, int height)
{
    maze_control* maze = (maze_control*)self;
    if (maze->labyrinth) {
        free(maze->labyrinth->data);
        free(maze->labyrinth);
    }
    maze->src.x = maze->src.y = 0;
    maze->dest.x = width - 1; maze->dest.y = height - 1;
    maze->labyrinth = 
        gen(width, height, maze->src, maze->dest);
    resize_canvas(self, coord(width, height));
    maze->cursor.x = maze->cursor.y = -1;
    maze->set_src = ogg_true;
    update_maze(self);
}
