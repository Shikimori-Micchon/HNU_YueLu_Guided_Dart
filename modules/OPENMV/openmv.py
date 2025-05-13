# Untitled - By: HUAWEI - Sat Apr 12 2025
G_TH = (54, 100, -128, -15, -128, 123)
import sensor, image, time, math
from machine import UART
EXPOSURE_TIME_SCALE = 0.3
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)  # 320 x240

sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

sensor.skip_frames(500)

current_exposure_time_in_microseconds = sensor.get_exposure_us()
print("Current Exposure == %d" % current_exposure_time_in_microseconds)

sensor.set_auto_exposure(False, \
    exposure_us = int(current_exposure_time_in_microseconds * EXPOSURE_TIME_SCALE))
print("New exposure == %d" % sensor.get_exposure_us())

uart = UART(3,115200)

clock = time.clock()

# 定义通信协议
FRAME_HEAD = 0xAA    # 帧头
FRAME_END = 0x55     # 帧尾

def find_led_ring(img):
    bright_blobs = img.find_blobs([(0, 98, -71, -24, 11, 68)],
                                  x_stride=1, y_stride=1)

    for bb in bright_blobs:
        # 计算blob的实际边界
        x_end = bb.x() + bb.w()
        y_end = bb.y() + bb.h()

        # 定义环形检测区域（排除中心过曝区）
        margin = 1
        outer_roi = (
            max(bb.x() + margin, 0),
            max(bb.y() + margin, 0),
            max(bb.w() - 2*margin, 1),  # 保证宽度至少1像素
            max(bb.h() - 2*margin, 1)   # 保证高度至少1像素
        )

#        # 颜色验证
#        outer_stats = img.get_statistics(roi=outer_roi)
#        if not (-70 < outer_stats.a_mean() < -20 and 10 < outer_stats.b_mean() < 40):
#            continue

        # 几何验证（使用实际边界坐标）
        cx, cy = bb.cx(), bb.cy()
        edge_points = []
        for angle in range(0, 360, 30):
            r = bb.w()//4
            x = int(cx + r * math.cos(math.radians(angle)))
            y = int(cy + r * math.sin(math.radians(angle)))

            # 验证点是否在blob区域内
            if (bb.x() <= x < x_end) and (bb.y() <= y < y_end):
                edge_points.append((x,y))

        # green_count = 0
        # for x,y in edge_points:
        #     pix = img.get_pixel(x,y)
        #     lab = image.rgb_to_lab(pix)
        #     if (G_TH[0] < lab[0] < G_TH[1] and
        #         G_TH[2] < lab[1] < G_TH[3] and
        #         G_TH[4] < lab[2] < G_TH[5]):
        #         green_count +=1

        # if green_count >= 8:
        return bb.cx(), bb.cy()

    # return None

while True:
    clock.tick()
    img = sensor.snapshot()
    img.median(1)
    print(clock.fps())
    print(sensor.get_exposure_us())

    result = find_led_ring(img)
    if result:
        cx, cy = result
        img.draw_cross(cx, cy, size=20, color=(255, 0, 0))
        print(f"LED中心: ({cx},{cy})")
        cx_high = (cx >> 8) & 0xFF  # 高字节
        cx_low = cx & 0xFF         # 低字节
        cy_high = (cy >> 8) & 0xFF  # 高字节
        cy_low = cy & 0xFF         # 低字节

        # 构建数据帧
        checksum = (cx_high + cx_low + cy_high + cy_low) & 0xFF
        data_packet = bytearray([
            FRAME_HEAD,
            cx_high, cx_low,
            cy_high, cy_low,
            checksum,
            FRAME_END
        ])
        uart.write(data_packet)
        # print(f"Sent: {[hex(b) for b in data_packet]}")
    else:
        checksum = (0) & 0xFF
        data_packet = bytearray([
            FRAME_HEAD,
            0, 0,
            0, 0,
            checksum,
            FRAME_END
        ])
        uart.write(data_packet)
        # print("未检测到LED环")
