# 湖南大学跃鹿战队 制导飞镖开源

## 硬件层

<font size = 4>

* 镖体构型在经过许多尝试后，最终决定使用**X**布局的四舵面空气舵导弹构型，适配长空御风战队22年开源飞镖发射架
* 尾部使用涵道推进增加动力，对控制无太大作用，涵道使用2s 14000kv的群汐涵道
* 数据回传使用匿名数传
* IMU使用维特智能的JY61P
* 视觉识别部分尝试过使用FPGA,但稳定性欠佳，修改麻烦，最后仍使用openmv作为识别模块
* 舵机使用蓝剑舵机
* 电池使用规则参数限制下的最高规格电池

## 软件层

* 主要工作流程为：由IMU检测是否发射，发射后，全程开启涵道定速输出，非制导段进行滚转通道控制，使得滚转为0，进入制导段后，运行PNG任务开始制导，结束任务采用计时一段时间，关闭电机输出，结束所有任务