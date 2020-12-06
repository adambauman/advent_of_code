import pygame
from time import time_ns

class Color:
    tree = (18, 150, 27)
    snow = (240, 240, 240)
    collision = (186, 9, 9)

def load_data(file_path):
    assert 0 != len(file_path)

    print("Loading data...")
    load_start_ticks = pygame.time.get_ticks()

    data = []
    with open(file_path, 'r') as file:
        for count, line in enumerate(file):
            assert 0 != len(line)
            
            if '\n' == line[-1]:
                # Peel the newline off
                line = line[:-1]
            
            data.append(line)

    load_time_ticks = pygame.time.get_ticks() - load_start_ticks
    print("Finished, loaded {} lines in {} ticks".format(len(data), load_time_ticks))

    return data

def draw_cell_line(line_data):
    assert(0 != len(line_data))
    
    line_surface = pygame.Surface((len(line_data), 1))
    px_array = pygame.PixelArray(line_surface)
    current_x = 0
    for character in line_data:
        if '.' == character:
            px_color = Color.snow
        elif '#' == character:
            px_color = Color.tree
        else:
            raise Exception("bad map data: {}".format(character))

        px_array[current_x, 0] = px_color
        current_x += 1

    return line_surface

def draw_map(map_data, slope):
    assert 0 != len(map_data)

    print("Drawing the map...")
    
    # Moving 1 line down for every 3 columns right, create the base surface
    map_size = (len(map_data) * slope[0], len(map_data))
    map_surface = pygame.Surface(map_size)

    if __debug__:
        print("Map will be {} by {} pixels".format(map_size[0], map_size[1]))
    
    # Create our map cell, this will repeat across the width of the map surface
    cell_size = (len(map_data[0]), len(map_data))
    cell_surface = pygame.Surface(cell_size)
    current_cell_y = 0
    for line in map_data:
        cell_surface.blit(draw_cell_line(line), (0, current_cell_y))
        current_cell_y += 1

    #pygame.image.save(cell_surface, "cell_output.png")

    # Blit down the map cell until we fill the whole map
    current_map_x = 0
    while map_surface.get_width() > current_map_x:
        map_surface.blit(cell_surface, (current_map_x, 0))
        current_map_x += cell_surface.get_width()

    print("Map is complete")

    return map_surface

def solve_for_slope(map_data, slope):
    assert 0 != len(map_data)

    map_surface = draw_map(map_data, slope)
    collision_count = 0
    px_array = pygame.PixelArray(map_surface)
    cursor_x = 0
    cursor_y = 0
    while cursor_x < map_surface.get_width() and cursor_y < map_surface.get_width():
        if map_surface.map_rgb(Color.tree) == px_array[cursor_x, cursor_y]:
            collision_count += 1
            px_array[cursor_x, cursor_y] = Color.collision
     
        cursor_x += slope[0]
        cursor_y += slope[1]

    px_array.close()
    print("Saving map PNG...")
    map_name = "map_{}x{}.png".format(slope[0], slope[1])
    pygame.image.save(map_surface, map_name)

    return collision_count


def solve_for_multi_slope(map_data, slopes):
    assert 0 != len(slopes)
    assert 0 != len(map_data)

    # Right 1, down 1.
    # Right 3, down 1. (This is the slope you already checked.)
    # Right 5, down 1.
    # Right 7, down 1.
    # Right 1, down 2.
    # Multiply all collision counts, that is the answer

    print("Starting solve for Solution B...")

    solution_answer = 0
    for slope in slopes:
        collision_count = solve_for_slope(map_data, slope)
        if 0 == solution_answer:
            solution_answer = collision_count
            continue

        if __debug__:
            print("Current solution answer: {}".format(solution_answer))
            print("Collision count: {}".format(collision_count))

        solution_answer *= collision_count

    return solution_answer


def main():
    pygame.init()

    print("")
    print("+--------------------------------------------+")
    print("|  A D V E N T  O F  C O D E     D A Y  0 3  |")
    print("+--------------------------------------------+")
    print("")
    print("Solution A: Using forest map determine toboggan collisions moving right 3, down 1")
    print("Solution B: Same thing but with variable slopes")
    print("")

    file_path = "data.txt"
    map_data = load_data(file_path)
    assert(0 != len(map_data))

    # Solution A: 3 right, 1 down
    collision_count = solve_for_slope(map_data, (3,1))
    print("========================================")
    print("Solution A: {} collisions".format(collision_count))
    print("========================================")

    # Solution B: Variable, multiple all collision counts for answer
    print("")
    slopes = [(1,1), (3,1), (5,1), (7,1), (1,2)]
    solution_b_answer = solve_for_multi_slope(map_data, slopes)
    print("========================================")
    print("Solution B: {}".format(solution_b_answer))
    print("========================================")

    pygame.quit()

if __name__ == "__main__":
    main()

