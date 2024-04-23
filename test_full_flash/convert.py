from PIL import Image

# Open an image file
input_image = Image.open('22.png')

# Invert the colors of the image
# inverted_image = Image.eval(input_image, lambda x: 255 - x)

# Convert the inverted image to black and white

# transform_matrix = (-1, 0, input_image.width, -1, input_image.height, 0)
#
# # 使用变换矩阵对图像进行左右和上下镜像翻转
# flipped_image = input_image.transform(input_image.size, Image.AFFINE, transform_matrix, Image.BICUBIC)


bw_image = input_image.convert('1')

# Get the pixel data
pixels = list(bw_image.getdata())

# Group the pixels into bytes
byte_array = []
byte = 0
for i, pixel in enumerate(pixels):
    if pixel:
        byte |= 1 << (7 - (i % 8))  # Set the corresponding bit if the pixel is on
    if (i + 1) % 8 == 0:
        byte_array.append(byte)
        byte = 0

i = 0
# Write the pixel data to a .c file in the specified format
with open('output_image.c', 'w') as file:
    file.write('const unsigned char gImage_1[] = { /* 0X10,0X01,0X00,0X98,0X01,0X28, */\n')
    for i, byte in enumerate(byte_array):
        if i % 16 == 0:
            file.write('\n')
        file.write(f'0X{byte:02X},')
        i = i+1
    file.write('\n};')
print(i)
