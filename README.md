# task5

考核五为opencv考核，在做考核之前需熟悉opencv是用来做什么的，常用函数有哪些

**结构介绍**

- assets文件夹：
    - example.gif：示例gif动图
    - origin.mp4：需要识别的原始视频
- code文件夹：
    - 存放你的源代码以及CMakeLists.txt文件

- README.md：任务说明文档

## 任务：物体识别

将篮球从视频中识别，并将其框出来。

提示：

想要对篮球进行识别首先需要知道篮球的颜色、形状等特征，显然，篮球是橙色，圆球形；

我们可以自由调节HSV空间阈值，寻找出合适的阈值范围，再生成黑白掩码；

然后做形态学优化，让目标轮廓更完整；最后提取轮廓并可视化标注。

## 上传要求（很重要，必须看）

由于各种原因，将视频上传github是不应该的行为，因此我们想上传视频时，应将视频转为gif动图。

我们需使用ubuntu录屏工具录制运行结果，将录制好的视频转为gif动图,转换工具可参考该网站

```
https://cloudconvert.com/webm-to-gif
```

随后将动图命名为result.gif，存入assets文件夹里，并嵌入该markdown文件**运行结果->result**。

然后将你的源代码以及CMakeLists.txt文件放至code文件夹里。

最后把assets文件夹里的origin.mp4删除，只保留gif动图

## 运行结果

**example:**

![result.gif](/assets/example.gif)

**result:**








