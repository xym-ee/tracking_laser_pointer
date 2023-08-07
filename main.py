# Hello World Example
#
# Welcome to the OpenMV IDE! Click on the green run arrow button below to run the script!

import sensor, image, time
import pyb

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.skip_frames(time = 2000)     # Wait for settings take effect.


sensor.set_auto_gain(False)         # 关闭自动自动增益。默认开启的，在颜色识别中，一定要关闭白平衡。
sensor.set_auto_whitebal(False)     #关闭白平衡。白平衡是默认开启的，在颜色识别中，一定要关闭白平衡。


clock = time.clock()                # Create a clock object to track the FPS.


led = pyb.LED(3)

red_point = (24, 100, 14, 127, -128, 127)

i = 1

while(True):
    clock.tick()                    # Update the FPS clock.
    img = sensor.snapshot()         # Take a picture and return the image.
    
    
    blobs = img.find_blobs([red_point])
    
    if blobs:
    
        led.on()
    
        for b in blobs:

            img.draw_rectangle(b[0:4]) # Draw a rect around the blob.

            img.draw_cross(b[5], b[6]) # (x, y) 画十字形标记

            print(b[5],b[6])
            
    
    # print(clock.fps()) # 注意: 你的OpenMV连到电脑后帧率大概为原来的一半
    
    else:
        led.off()
    
    
    
    
    
    
    
    
    
    