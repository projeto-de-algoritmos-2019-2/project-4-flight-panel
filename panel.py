import pygame
import requests
import time

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
            'x': ((each[5] + 47.451401) * 1280)/x_factor,
            'y': ((each[6] + 24.100386) * 720)/y_factor
        })

    return flights

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

        for flight in flights:
            text = font.render(flight['id'], True, (255, 0, 0))
            screen.blit(text, (int(flight['x']), int(flight['y']) + text.get_height() + 3))
            pygame.draw.circle(screen, (255, 0, 0), (int(flight['x']), int(flight['y'])), 7)



        pygame.display.flip()