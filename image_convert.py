from PIL import Image, ImageOps, ImageFilter
import numpy as np
import struct
import os
import shutil

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

palette = [
    0, 0, 0,       # Black
    255, 255, 255,  # White
    0, 255, 0,   # Green
    0, 0, 255,   # Blue
    255, 0, 0,   # Red
    255, 255, 0,   # Yellow
    255, 128, 0   # Orange
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

def convert_image_2_dithered(image):
    # Open the input image
    input_image = Image.open(image).convert('RGB')
    resized_image = input_image.copy()
    resized_image.thumbnail((600, 448), Image.LANCZOS)
    temp = resized_image.copy()
    temp.save("resized.png", dpi=[300,300], quality=95)
    p_img = Image.new('P', resized_image.size)
    p_img.putpalette(palette)
    quantized_image = resized_image.quantize(palette=p_img)
    dithered_image = quantized_image.convert('P')

    dithered_image.save("dithered.png")
def convert_image_2_7_colors(image):
    # Open the input image
    input_image = Image.open(image)
    input_image = input_image.convert('RGB')

    os.makedirs("images", exist_ok=True)
    os.makedirs("images/c_array", exist_ok=True)
    os.makedirs("images/bmp", exist_ok=True)
    os.chdir("images")

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
        new_image.save(str(colors[current_color]) + ".jpg", dpi=[300,300], quality=95)
        os.chdir("bmp")
        new_image.save(str(colors[current_color]) + ".bmp")
        os.chdir("..")
        convert_bmp_to_array("./bmp/" + str(colors[current_color]) + ".bmp", str(colors[current_color]))
        os.chdir("..")
        current_color += 1
    shutil.rmtree("bmp")
    print("Images saved successfully.")


def convert_bmp_to_array(bmp_file, curr_color):
    bmp = Image.open(bmp_file).convert('1')
    width, height = bmp.size

    image_data = list(bmp.getdata())


    os.chdir("c_array")
    with open ("img_info.h", "w") as file:
        file.write(f"#define IMG_WIDTH {width}\n")
        file.write(f"#define IMG_HEIGHT {height}\n")
    with open (curr_color + ".c", "w") as file:
        file.write(f"const unsigned char gImage_{curr_color}[{width * height // 8}] = {{\n")

        bit_list = []

        bit_index = 8
        byte_counter = 0



        for x in range(0, len(image_data)):
            bit_value = 0 if image_data[x] > 0 else 1

            bit_list.append(bit_value)
            bit_index -= 1

            if bit_index == 0:
                byte = 0
                for bit in bit_list:
                    byte = (byte << 1) | bit
                file.write(f"0x{byte:02x}")
                if x + 1 != len(image_data):
                    file.write(", ")
                bit_list = []
                bit_index = 8
                if x+1 == len(image_data):
                    bit_index = 0
                byte_counter += 1
                if byte_counter % 16 == 0:
                    file.write("\n")
            
            if x + 1 == len(image_data) and bit_index != 0:
                byte = 0
                for index in range(len(bit_list), 8):
                    bit_list.append(0)
                for bit in bit_list:
                    byte = (byte << 1) | bit
                file.write(f"0x{byte:02x}")
                bit_list = []
                bit_index = 7
                byte_counter += 1
        file.write("};\n")

convert_image_2_dithered("boykisser.png")

convert_image_2_7_colors("dithered.png")