from PIL import Image
import time
import serial
import matplotlib.pyplot as plt

def main():
    ser = serial.Serial(port='COM3', baudrate=115200, timeout=0.0)

    image = Image.open('5 Как работать с дисплеем/Задания/pics/get.jpg')
    width, height = image.size

    ser.write("disp_screen 0x000000\n".encode('ascii'))

    time.sleep(0.1)

    for y in range(height):
        for x in range(width):
            # Получаем цвет пикселя (RGB)
            r, g, b = image.getpixel((x, y))

            color = (r << 16) | (g << 8) | b
            
            ser.write("disp_px {x} {y} {color:06X}\n".encode('ascii'))

    time.sleep(0.1)
    ser.close()

if __name__ == "__main__":
    main()