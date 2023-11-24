from PIL import Image, ImageOps, ImageFilter
import numpy as np

# Define the color palette
color_palette = [
    (0, 0, 0),       # Black
    (255, 255, 255),  # White
    (0, 255, 0),   # Green
    (0, 0, 255),   # Blue
    (255, 0, 0),   # Red
    (255, 255, 0),   # Yellow
    (255, 128, 0)    # Orange
]
colors = [
    "black",
    "white",
    "green",
    "blue",
    "red",
    "yellow",
    "orange"
]

# Open the input image
input_image = Image.open("input_image.png")
input_image = input_image.convert('RGB')
loaded_input = input_image.load()

# Find each color in the image, create an new image for only that color and save it
current_color = 0
for color in color_palette:
    new_image = Image.new('1', input_image.size)
    for x in range(input_image.size[0]):
        for y in range(input_image.size[1]):
            r, g, b = input_image.getpixel((x,y))
            if (r, g, b) == color:
                new_image.putpixel((x,y), 0)
            else:
                new_image.putpixel((x,y), 1)
    new_image = new_image.transpose(Image.ROTATE_90).save(str(colors[current_color]) + ".jpg")
    # ImageOps.contain(new_image, (600,448)).save(str(colors[current_color]) + ".jpg")

    current_color += 1




print("Images saved successfully.")