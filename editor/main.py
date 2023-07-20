import numpy as np
import pygame
from pygame.locals import *

a = np.arange(15).reshape(3, 5)
print(a)
print(a.shape)
pygame.init()
vec = pygame.math.Vector2  # 2 for two dimensional

HEIGHT = 512
WIDTH = 512
ACC = 0.5
FRIC = -0.12
FPS = 60

FramePerSec = pygame.time.Clock()

displaysurface = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Game")

while True:
	for event in pygame.event.get():
		if event.type == QUIT:
			pygame.quit()
			exit()
