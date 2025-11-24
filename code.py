import numpy as np
import cv2
import cv2 as cv
import matplotlib.pyplot as plt

#获取图像
img=cv.imread("/home/gan/opencv/photo.jpg")
#print(img.size)

# 转换为灰度图
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  

# 图像旋转
rows,cols = img.shape[:2]
M = cv.getRotationMatrix2D((cols/2,rows/2),45,1) #生成旋转矩阵
dst = cv.warpAffine(img,M,(cols,rows))  #进行旋转彼变换

#转换背景图
_, mask = cv2.threshold(gray, 200, 255, cv2.THRESH_BINARY)   # 创建白色背景的掩码
result = img.copy()   # 将白色区域设为黑色
result[mask == 255] = [0, 0, 0]  

#描边操作
_, binary=cv2.threshold(gray,128,255,cv2.THRESH_BINARY_INV);  #二值化操作 会返回两个值 ，所以设置 "_,"
kernel=cv2.getStructuringElement(cv2.MORPH_RECT,(5,5)); #设置扫描元素
dilated= cv2.dilate(binary,kernel); #膨胀操作
stroke=img.copy();  #c++用clone,python用copy
contours,hierarchy=cv2.findContours(dilated,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE);
cv2.drawContours(stroke,contours,-1,(0,0,255),3);  

#展示图像
plt.figure(figsize=(18,10))

plt.subplot(2,3,1)
plt.imshow(img[:,:,::-1])  #输出原图
plt.title("img input")

plt.subplot(2,3,2)
plt.imshow(gray,cmap='gray')  #输出灰度图
plt.title("gray input")

plt.subplot(2,3,3)
plt.imshow(dst[:,:,::-1])  #输出旋转图
plt.title("rotate input")

plt.subplot(2,3,4)
plt.imshow(img[:,:,:])  #不旋转通道即为RGB输出******输出RGB图
plt.title("RGB input")

plt.subplot(2,3,5)
plt.imshow(result[:,:,::-1]) #输出黑色背景图
plt.title("black background")

plt.subplot(2,3,6)
plt.imshow(stroke[:,:,::-1]) #输出描边图
plt.title("stroke input")

plt.tight_layout
plt.show()


