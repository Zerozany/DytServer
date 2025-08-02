# 载荷服务器接口说明

## 1.设备添加相关
- 添加设备
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/device/add
  3. queryitems:
     1. 视频播放地址：url
     2. 设备地址：address
     3. 设备端口：port
  4. 请求示例：http://127.0.0.1:3300/device/add?url=rtsp://172.16.6.100:554/stream0&address=127.0.0.1&port=23459
- 移除设备
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/device/remove
  3. queryitems:
     1. 设备序号：uid
  4. 请求示例：http://127.0.0.1:3300/device/remove?uid=1

## 2.视频播放相关
- 解析视频
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/video/play
  3. queryitems:
     1. 设备序号：uid
  4. 请求示例：http://127.0.0.1:3300/video/play?uid=1 
- 停止播放
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/video/stop
  3. queryitems:
     1. 设备序号：uid
  4. 请求示例：http://127.0.0.1:3300/video/stop?uid=1 
- 读取视频帧
  1. 请求类型：Get
  2. 请求接口：http://127.0.0.1:3300/video/read
  3. queryitems:
     1. 设备序号：uid
  4. 请求示例：http://127.0.0.1:3300/video/read?uid=1 
- 打开图片
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/image/open
  3. queryitems:
     1. 设备序号：uid
  4. 请求示例：http://127.0.0.1:3300/image/open?uid=1 
- 关闭图片
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/image/close
  3. queryitems:
     1. 设备序号：uid
  4. 请求示例：http://127.0.0.1:3300/image/close?uid=1 

## 3.控制指令相关
### 1.PLD80
- OSD
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/OSD
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`on` or `off`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/OSD?uid=1&order=on 
- 画中画灯光
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/pip-light
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`visible` or `infrared`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/pip-light?uid=1&order=visible 
- 变焦指令
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/zoom
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`-100` to `100`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/zoom?uid=1&order=100 
- 相机切换
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/camera-switch
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`visible-light-1` or `visible-light-2` or `red-infrared-hot` or `red-infrared-cold`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/camera-switch?uid=1&order=red-infrared-cold 
- 目标类型
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/target-type
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`adaptive` or `person` or `vehicle` or `building`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/target-type?uid=1&order=person 
- 目标锁定关
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/target-lock
  3. queryitems:
     1. 设备序号：uid
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/target-lock?uid=1
- 归中
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/center
  3. queryitems:
     1. 设备序号：uid
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/center
- 目标检测
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/target-check
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`on` or `off`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/target-check?uid=1&order=on
- 弹上存储
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/bullet-storage
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`on` or `off`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/bullet-storage?uid=1&order=on
- 夜间攻击
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/night-attack
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`on` or `off`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/night-attack?uid=1&order=on
 - 电机
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/motor-control
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`on` or `off`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/motor-control?uid=1&order=on  
 - 跟随
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/fllow
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`on` or `off`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/fllow?uid=1&order=on  
 - 电锁
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/door-lock
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`on` or `off`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/door-lock?uid=1&order=on  
 - 周扫模式
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/sweep-mode
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`on` or `off`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/sweep-mode?uid=1&order=on  
 - 抑制陀螺仪漂移
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/gyro-suppression
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`-2000` to `2000`}
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/gyro-suppression?uid=1&order=2000 
 - 指定框架角度
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/PLD80-order/frame-angle
  3. queryitems:
     1. 设备序号：uid
     2. 俯仰角：pitch_frame_angle
     3. 方位角：azimuth_frame_angle
  4. 请求示例：http://127.0.0.1:3300/PLD80-order/frame-angle?uid=1&pitch_frame_angle=50&azimuth_frame_angle=30 
### 2.三光导引头
- OSD
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/TBS-order/OSD
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`on` or `off`}
  4. 请求示例：http://127.0.0.1:3300/TBS-order/OSD?uid=1&order=on 
- 画中画灯光
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/TBS-order/pip-light
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`visible` or `infrared`}
  4. 请求示例：http://127.0.0.1:3300/TBS-order/pip-light?uid=1&order=visible  
- 变焦指令
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/TBS-order/zoom
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`-100` to `100`}
  4. 请求示例：http://127.0.0.1:3300/TBS-order/zoom?uid=1&order=100 
- 目标锁定关
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/TBS-order/target-lock
  3. queryitems:
     1. 设备序号：uid
  4. 请求示例：http://127.0.0.1:3300/TBS-order/target-lock?uid=1
- 激光测距
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/TBS-order/laser-distance
  3. queryitems:
     1. 设备序号：uid
     2. 指令前缀：order{`on` or `off`}
  4. 请求示例：http://127.0.0.1:3300/TBS-order/laser-distance?uid=1&order=on 
### 3. 通道
- 地理跟踪
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/pip-order/geographic-tracking
  3. queryitems:
     1. 设备序号：uid
     2. 经度：longitude
     3. 维度：latitude
     4. 高度：height
  4. 请求示例：http://127.0.0.1:3300/pip-order/geographic-tracking?uid=1&longitude=30&latitude=45&height=600

- 控制移动
  1. 请求类型：Post
  2. 请求接口：http://127.0.0.1:3300/control/move
  3. queryitems:
     1. 设备序号：uid
     2. x：x_speed
     3. y：y_speed
  4. 请求示例：http://127.0.0.1:3300/control/move?uid=1&x_speed=30&y_speed=45










































































