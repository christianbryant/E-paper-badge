from PIL import Image, ImageOps, ImageFilter
import numpy as np
import struct

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
    new_image = new_image.transpose(Image.ROTATE_90).save(str(colors[current_color]) + ".jpg", dpi=[300,300], quality=95)
    # ImageOps.contain(new_image, (600,448)).save(str(colors[current_color]) + ".jpg")

    current_color += 1

# Open each image and convert it to a bitmap
black_img = Image.open("black.jpg").convert('1').save("black.bmp")
black_bmp = Image.open("black.bmp").convert('1')
width, height = black_bmp.size

image_data = list(black_bmp.getdata())
print(image_data)
print(len(image_data))



with open ("black.c", "w") as file:
    file.write(f"const unsigned char gImage_black[{width * height // 8}] = {{\n")

    bit_list = []
    bit_index = 8
    byte_counter = 0

    for x in range(0, len(image_data)):
        bit_value = 0 if image_data[x] > 0 else 1

        bit_list.append(bit_value)
        print(bit_list)
        bit_index -= 1

        if bit_index == 0:
            byte = 0
            for bit in bit_list:
                byte = (byte << 1) | bit
            if byte != 0:
                print(f"0x{byte:02x}")
            file.write(f"0x{byte:02x}")
            if x + 1 != len(image_data):
                file.write(", ")
            else:
                print("This is the last byte")
            bit_list = []
            bit_index = 8
            if x+1 == len(image_data):
                bit_index = 0
            byte_counter += 1
            if byte_counter % 16 == 0:
                file.write("\n")
        
        if x + 1 == len(image_data) and bit_index != 0:
            byte = 0
            print("This is the last byte")
            for index in range(len(bit_list), 8):
                bit_list.append(0)
            for bit in bit_list:
                byte = (byte << 1) | bit
            file.write(f"0x{byte:02x}")
            bit_list = []
            bit_index = 7
            byte_counter += 1
    print(byte_counter)
    file.write("};\n")
print("Black image saved successfully.")



print("Images saved successfully.")


def convert_bmp_to_array(bmp_file):
    bmp = Image.open(bmp_file).convert('1')
    width, height = bmp.size

    image_data = list(bmp.getdata())
    print(image_data)
    print(len(image_data))



    with open ("black.c", "w") as file:
        file.write(f"const unsigned char gImage_black[{width * height // 8}] = {{\n")

        bit_list = []
        bit_index = 8
        byte_counter = 0

        for x in range(0, len(image_data)):
            bit_value = 0 if image_data[x] > 0 else 1

            bit_list.append(bit_value)
            print(bit_list)
            bit_index -= 1

            if bit_index == 0:
                byte = 0
                for bit in bit_list:
                    byte = (byte << 1) | bit
                if byte != 0:
                    print(f"0x{byte:02x}")
                file.write(f"0x{byte:02x}")
                if x + 1 != len(image_data):
                    file.write(", ")
                else:
                    print("This is the last byte")
                bit_list = []
                bit_index = 8
                if x+1 == len(image_data):
                    bit_index = 0
                byte_counter += 1
                if byte_counter % 16 == 0:
                    file.write("\n")
            
            if x + 1 == len(image_data) and bit_index != 0:
                byte = 0
                print("This is the last byte")
                for index in range(len(bit_list), 8):
                    bit_list.append(0)
                for bit in bit_list:
                    byte = (byte << 1) | bit
                file.write(f"0x{byte:02x}")
                bit_list = []
                bit_index = 7
                byte_counter += 1
        print(byte_counter)
        file.write("};\n")
    print("Black image saved successfully.")