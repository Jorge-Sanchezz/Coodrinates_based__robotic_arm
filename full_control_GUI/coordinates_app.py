import pygame

pygame.init()

# Screen dimensions
screen_width = 800
screen_height = 600
screen = pygame.display.set_mode((screen_width, screen_height))

# Colors
black = (0, 0, 0)
red = (255, 0, 0)
green = (0, 255, 0)
blue = (0, 0, 255)
white = (255, 255, 255)
gray = (128, 128, 128)

# Initial positions
mouse_x, mouse_y = screen_width // 2, screen_height // 2
slider_x = screen_width - 20
slider_height = 300
slider_y = 50  # Using slider_y instead of slider_z for clarity
z_value = slider_y

# Flags for button presses
mouse_button_down = False
slider_button_down = False

# Font for text
font = pygame.font.Font(None, 24)

# Main game loop
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.pos[0] >= slider_x - 5 and event.pos[0] <= slider_x + 5 and event.pos[1] >= slider_y and event.pos[1] <= slider_y + slider_height:
                slider_button_down = True
            else:
                mouse_button_down = True
                mouse_x, mouse_y = event.pos
        elif event.type == pygame.MOUSEBUTTONUP:
            mouse_button_down = False
            slider_button_down = False
        elif event.type == pygame.MOUSEMOTION:
            if slider_button_down:
                z_value = event.pos[1] - slider_y
                if z_value < 0:
                    z_value = 0
                elif z_value > slider_height:
                    z_value = slider_height
            elif mouse_button_down:
                mouse_x, mouse_y = event.pos

    # Clear the screen
    screen.fill(black)

    # Draw fixed axes (white)
    pygame.draw.line(screen, white, (0, screen_height // 2), (screen_width, screen_height // 2), 1)  # X axis
    pygame.draw.line(screen, white, (screen_width // 2, 0), (screen_width // 2, screen_height), 1)  # Y axis

    # Draw movable axes (red and green)
    pygame.draw.line(screen, red, (0, mouse_y), (screen_width, mouse_y), 2)  # X movable axis
    pygame.draw.line(screen, green, (mouse_x, 0), (mouse_x, screen_height), 2)  # Y movable axis

    # Draw the slider
    pygame.draw.rect(screen, gray, (slider_x - 5, slider_y, 10, slider_height))
    pygame.draw.circle(screen, blue, (slider_x, slider_y + z_value), 7)  # Increased circle radius

    # Draw the center dot (unchanged)
    pygame.draw.circle(screen, blue, (mouse_x, mouse_y), 5)

    # Create the text surface with X, Y, and Z values
    text = font.render(f"X: {mouse_x}  Y: {mouse_y}  Z: {z_value}", True, white)

    # Draw the text in the upper left corner
    screen.blit(text, (10, 10))

    pygame.display.flip()

pygame.quit()
