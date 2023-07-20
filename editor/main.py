import numpy as np
import pygame
from pygame.locals import *
import pygame.freetype


def main():
	pygame.init()

	screen = pygame.display.set_mode((800, 600))
	pygame.display.set_caption("Bevy py GUI")

	# my_font = pygame.freetype.Font('pixel-clear-condensed.ttf', 30)       x

	looping = True
	while looping:
		for event in pygame.event.get():
			if event.type == QUIT:
				looping = not looping

		screen.fill((32, 32, 32))


		pygame.display.update()

main()
