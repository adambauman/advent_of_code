import pygame
from time import time_ns

class Color:
    tree = (18, 150, 27)
    snow = (255, 255, 255)
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

def draw_map(map_data):
    assert 0 != len(map_data)

    print("Drawing the map...")
    
    # Moving 1 line down for every 3 columns right, create the base surface
    map_size = (len(map_data) * 3, len(map_data))
    map_surface = pygame.Surface(map_size)

    print("Map will be {} by {} pixels".format(map_size[0], map_size[1]))
    
    # Create our map cell, this will repeat across the width of the map surface
    cell_size = (len(map_data[0]), len(map_data))
    cell_surface = pygame.Surface(cell_size)
    current_cell_y = 0
    for line in map_data:
        cell_surface.blit(draw_cell_line(line), (0, current_cell_y))
        current_cell_y += 1

    #pygame.image.save(cell_surface, 'cell_output.png

    # Blit down the map cell until we fill the whole map
    current_map_x = 0
    while map_surface.get_width() > current_map_x:
        map_surface.blit(cell_surface, (current_map_x, 0))
        current_map_x += cell_surface.get_width()

    print("Map is complete")

    return map_surface

def solve_a(map_surface):
    collision_map = map_surface.copy()
    collision_count = 0

    px_array = pygame.PixelArray(collision_map)
    cursor_x = 0
    cursor_y = 0
    while cursor_x < collision_map.get_width():
        if collision_map.map_rgb(Color.tree) == px_array[cursor_x, cursor_y]:
            collision_count += 1
            px_array[cursor_x, cursor_y] = Color.collision
     
        cursor_x += 3
        cursor_y += 1

    return collision_map, collision_count


def main():
    pygame.init()
    pygame.mixer.quit()
    pygame.mouse.set_visible(False)
    pygame.event.set_allowed([pygame.QUIT])

    #display_surface = pygame.display.set_mode(
    #    (Hardware.screen_width, Hardware.screen_height),
    #    pygame.HWSURFACE | pygame.DOUBLEBUF
    #)

    print("")
    print("+--------------------------------------------+")
    print("|  A D V E N T  O F  C O D E     D A Y  0 3  |")
    print("+--------------------------------------------+")
    print("")
    print("Solution A: Using forest map determine toboggan collisions moving right 3, down 1")
    print("Solution B: ???")
    print("")

    file_path = "data.txt"
    map_data = load_data(file_path)
    assert(0 != len(map_data))

    map_surface = draw_map(map_data)
    collision_map, collision_count = solve_a(map_surface)

    print("========================================")
    print("Solution A: {} collisions".format(collision_count))
    print("========================================")
    print("Saving collision map to solution_a_map.png")
    pygame.image.save(collision_map, "./solution_a_map.png")


        # for event in pygame.event.get():
        #    if event.type == pygame.QUIT:
        #        print("User quit")
        #        pygame.quit()
        #        sys.exit()
        #pygame.event.clear()


    pygame.quit()

if __name__ == "__main__":
    main()

