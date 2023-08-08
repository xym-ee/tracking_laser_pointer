import sensor, image, time, pyb

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((180, 180))
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

clock = time.clock()                # Create a clock object to track the FPS.

led     = pyb.LED(3)
uart    = pyb.UART(3,115200)

# red_point = (24, 100, 14, 127, -128, 127)
red_point = (30, 100, 20, 127, -128, 127)

while(True):
    clock.tick()                    # Update the FPS clock.
    img = sensor.snapshot()         # Take a picture and return the image.

    blobs = img.find_blobs([red_point])

    if blobs:
        for b in blobs:

            # 坐标旋转
            x = 179 - b.cy()
            y = b.cx()

            # 串口输出
            uart_output = "%c%c%c%c%c" % (0x55, 0x55, x, y, 0xAA)
            uart.write(uart_output)

            # -----------------------------------------------------------------调试用
            led.on()
            img.draw_rectangle(b[0:4])          # 调试用 Draw a rect around the blob.
            img.draw_cross(b[5], b[6])          # 调试用 (x, y) 画十字形标记

            output_str = "[%d,%d]" % (x, y)     # 调试用输出坐标
            print(output_str)
            print(uart_output)
            # -----------------------------------------------------------------调试用
    else:
        led.off()

    #print(clock.fps()) # 注意: 你的OpenMV连到电脑后帧率大概为原来的一半
