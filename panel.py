import pygame
import requests
import time
import os

def fetch_flights():
    # url = 'https://opensky-network.org/api/states/all?lamin=-24.100386&lomin=-47.451401&lamax=-23.183942&lomax=-45.691472'
    url = 'https://opensky-network.org/api/states/all?lamin=-24.493696&lomin=-48.177301&lamax=-22.797153&lomax=-45.566508'
    r = requests.get(url)
    data = r.json()

    flights = []

    x_factor = 47.451401 - 45.691472
    y_factor = 24.100386 - 23.183942

    for each in data['states']:
        flights.append({
            'id': each[1],
            'x': int(((each[5] + 47.451401) * 1280)/x_factor),
            'y': int(((each[6] + 24.100386) * 720)/y_factor)
        })

    return flights

def fetch_closest_points(flights):
    in_screen = []
    for flight in flights:
        if flight['x'] >= 0 and flight['x'] <= 1280 and flight['y'] >= 0 and flight['y'] <= 720:
            in_screen.append(
                (flight['x'], flight['y'])
            )

    if len(in_screen) >= 2:
        cpp_param = f"{len(in_screen)}"

        for point in in_screen:
            cpp_param+=f" {point[0]} {point[1]}"

        cmd = f'echo "{cpp_param}" | ./a.out'

        response = os.popen(cmd).readlines()

        dist = response[0][:-1]
        p1 = response[1][:-1]
        p2 = response[2][:-1]

        p1, p2 = [int(a) for a in p1.split()], [int(a) for a in p2.split()]

        p1tuple, p2tuple = (p1[0], p1[1]), (p2[0], p2[1])

        ret_obj = {
            'distance': float(dist),
            'p1': p1tuple,
            'p2': p2tuple
        }

        return ret_obj

    return {'distance': 10000000}

pygame.init()
screen = pygame.display.set_mode((1280, 720))
done = False

font = pygame.font.SysFont("comicsansms", 20)

mapim = pygame.image.load("spmap.png")
mapim.convert()

while not done:
        for event in pygame.event.get():
                if event.type == pygame.QUIT:
                        done = True
        
        flights = fetch_flights()

        screen.blit(mapim, (0, 0))

        dist_obj = fetch_closest_points(flights)

        for flight in flights:
            text = font.render(flight['id'], True, (255, 0, 0))
            screen.blit(text, (flight['x'], flight['y'] + text.get_height() + 3))
            pygame.draw.circle(screen, (255, 0, 0), (flight['x'], flight['y']), 7)

        if dist_obj['distance'] <= 1000:
            text = font.render(str(dist_obj['distance']), True, (0, 0, 0))

            text_pos = (
                (dist_obj['p1'][0] + dist_obj['p2'][0])//2 - text.get_width() + 10,
                (dist_obj['p1'][1] + dist_obj['p2'][1])//2 - text.get_height() - 10
            )

            pygame.draw.line(screen, (0, 0, 255), dist_obj['p1'], dist_obj['p2'], 3)
            pygame.draw.rect(screen, (128, 128, 128), pygame.Rect(text_pos[0] - 5, text_pos[1] - 5, text.get_width() + 10, text.get_height() + 10))
            screen.blit(text, text_pos)

        pygame.display.flip()