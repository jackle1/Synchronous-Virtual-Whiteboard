# https://www.file-recovery.com/bmp-signature-format.htm
# https://www.barth-dev.de/about-rgb565-and-how-to-convert-into-it/
import sys

VGA_X = 640
VGA_Y = 480

"""
Will take in a picture.bmp (640 x 480) file and convert it to a .bin file read for the VGA
Will convert from RGB888 to RGB565 necessary for the DE1
Also will reflect the picture due to some weirdness on the DE1
"""
if __name__ == "__main__":
    with open(sys.argv[1], "rb") as file:
        file.seek(54)
        with open(sys.argv[1].replace("bmp", "bin"), "wb+") as saved:
            data = file.read()
            pic = int(len(data) / 3) * [0]
            for i in range(0, len(data), 3):
                red = data[i + 2]
                green = data[i + 1]
                blue = data[i]
                # pixel = ((red & 248) << 8) | ((green & 252) << 3) | (blue >> 3)
                pixel = (red << 16) | (green << 8) | (blue)
                offset = int(i / 3)
                x = offset % VGA_X
                y = int(offset / VGA_X)
                new_x = x 
                new_y = VGA_Y - y - 1
                idx = new_x + VGA_X * new_y
                # pic[idx] = pixel.to_bytes(length=2, byteorder='little')
                pic[idx] = pixel.to_bytes(length=4, byteorder='little')
            for i, pixel in enumerate(pic):
                try:
                    saved.write(pixel)
                except TypeError:
                    print("Idx", i)
                    break