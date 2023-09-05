from math import sin, cos, atan2, pi

import tkinter as tk
import serial
import logging

logging.basicConfig(level=logging.DEBUG)
arduino = serial.Serial('COM3', 115200)


def send_data(x, y):
    data = f"{x-120},{y-120}\n"
    arduino.write(data.encode())


# get angle of mouse to (150, 150) and make sure that the angle is in the correct quadrant
def get_angle(x, y):
    angle = atan2(y - 150, x - 150)
    if angle < 0:
        angle += 2 * pi
    return angle


# make sure that the mouse is within the circle
def clamp(x, y):
    if euclidean_distance(x, y, 150, 150) > 60:
        angle = get_angle(x, y)
        cos_value = cos(angle)
        sin_value = sin(angle)

        if angle < pi / 4:
            return (150 + 60 * cos_value, 150 + 60 * sin_value)
        elif angle < 3 * pi / 4:
            return (150 + 60 * cos_value, 150 + 60 * sin_value)
        elif angle < 5 * pi / 4:
            return (150 + 60 * cos_value, 150 + 60 * sin_value)
        elif angle < 7 * pi / 4:
            return (150 + 60 * cos_value, 150 + 60 * sin_value)
        else:
            return (150 + 60 * cos_value, 150 + 60 * sin_value)
    return (x, y)


def euclidean_distance(x1, y1, x2, y2):
    return ((x1-x2)**2 + (y1-y2)**2)**0.5


def joystick_motion(event):
    global mouse_position
    x, y = clamp(event.x, event.y)
    mouse_position = (x, y)
    canvas.delete("circle")
    canvas.create_oval(x - 20, y - 20, x + 20, y + 20, outline="blue",
                          fill="blue" if mouse_clicked else "white", tags="circle")
    logging.debug(f"Mouse position: {mouse_position}")
    send_data(x, y)


def mouse_toggle(event):
    x, y = clamp(event.x, event.y)

    global mouse_clicked
    mouse_clicked = not mouse_clicked
    canvas.delete("circle")
    canvas.create_oval(x - 20, y - 20, x + 20, y + 20, outline="blue",
                       fill="blue" if mouse_clicked else "white", tags="circle")
    logging.debug(f"Mouse clicked: {mouse_clicked}")


def send_data_continuous():
    if mouse_clicked and mouse_position is not None:
        x, y = mouse_position
        send_data(x, y)
    root.after(100, send_data_continuous)


root = tk.Tk()
root.title("Joystick")

canvas = tk.Canvas(root, width=300, height=300)
canvas.pack()

square_size = 50
canvas.create_oval(100, 100, 200, 200, outline="red")

mouse_clicked = False
mouse_position = (30, 30)

canvas.bind("<Button-1>", mouse_toggle)
canvas.bind("<Motion>", joystick_motion)

root.after(100, send_data_continuous)

root.mainloop()
