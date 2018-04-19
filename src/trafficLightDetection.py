# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import cv2
import numpy as np
import matplotlib.pyplot as plt
import queue

# define for visiable
IMAGESHOW = 0;
DEBUG4YCbCr = 0;
resizeFactor = 0.4;

def VideoLoad(FilePath):
    cap = cv2.VideoCapture(FilePath);
    return cap;


def VideoDetection(cap):
    while(1):
        ret, frame = cap.read();
        if(ret == False):
            print("Video Detection finished!\n");
            break;
        Detection(frame);
        
def Detection(Image):
    resizeImg = ImageResize(Image, resizeFactor);
    
    ImgYCbCr = CaluYCbCr(resizeImg);
    #ImageShow("ImgYCbCr", ImgYCbCr, 1);
    # low for green light, high for red light
    ImgCb = ImageThreshold(ImgYCbCr, 190, 60);
    #ImageShow("ImgCb", ImgCb, 0);
    ClosedImage = CloseOperationImage(ImgCb);
    #ImageShow("ClosedImage", ClosedImage, 0);
    circlePoint = BFSImage(ClosedImage);
    #StableCirclePoint = BoundingBoxStable(circlePoint);
    #circleImage = CirclePlot(Image, StableCirclePoint);
    circleImage = CirclePlot(Image, circlePoint);
    ImageShow("circleImage", circleImage, 0);
            
def ImageLoad(FilePath):
    img = cv2.imread(FilePath);
    if(IMAGESHOW):
        cv2.namedWindow("Image");
        cv2.imshow("Image", img);
        cv2.waitKey(0);
        cv2.destroyAllWindows()
    return img;

def CirclePlot(Image, circlePoint):
    WHITE = (0,0,0)
    width = 30;
    height = 30;
    CircleImage = Image.copy();
    for point in circlePoint:
        cv2.circle(CircleImage,(int(point[1]/resizeFactor),int(point[0]/resizeFactor)), 3, WHITE,-1);
    for point in circlePoint:
        pt1 = (int(point[1]/resizeFactor) - width, int(point[0]/resizeFactor) - height);
        pt2 = (int(point[1]/resizeFactor) + width, int(point[0]/resizeFactor) + height);
        cv2.rectangle(CircleImage, pt1, pt2, WHITE);
    return CircleImage


#def DilateImage(Image):     
#def ErodeImage(Image):  
def CloseOperationImage(Image):
    kernel_erode = cv2.getStructuringElement(cv2.MORPH_CROSS,(1, 1));
    kernel_dilate = cv2.getStructuringElement(cv2.MORPH_RECT,(5, 5));
    closedImage = cv2.morphologyEx(Image, cv2.MORPH_ERODE, kernel_erode);
    closedImage = cv2.morphologyEx(closedImage, cv2.MORPH_DILATE, kernel_dilate);
    return closedImage;


def BFSImage(Image):
    height, width = Image.shape;
    ImgBFS = np.zeros((height, width), dtype = "uint8");
    q = queue.Queue();
    lightPoint = [];
    for i in range(0, height):
        for j in range(0, width):
            if(ImgBFS[i,j] == 0) and (Image[i,j] == 255):
                q.put([i,j]);
                count = 0;
                node_x = 0;
                node_y = 0;
                while not q.empty():
                    node = q.get();
                    ImgBFS[node[0],node[1]] = 1;
                    if(node[0]+1 < height):
                        if(ImgBFS[node[0]+1,node[1]] == 0) and (Image[node[0]+1,node[1]] == 255):
                            q.put([node[0]+1,node[1]]);
                    if(node[1]+1 < width):
                        if(ImgBFS[node[0],node[1]+1] == 0) and (Image[node[0],node[1]+1] == 255):
                            q.put([node[0],node[1]+1]);
                    if(node[0]-1 > 0):
                        if(ImgBFS[node[0]-1,node[1]] == 0) and (Image[node[0]-1,node[1]] == 255):
                            q.put([node[0]-1,node[1]]);
                    if(node[1]-1 > 0):
                        if(ImgBFS[node[0],node[1]-1] == 0) and (Image[node[0],node[1]-1] == 255):
                            q.put([node[0],node[1]-1]);
                    count = count + 1;
                    node_y = node_y + node[0];
                    node_x = node_x + node[1];
                #print(node_y/count, node_x/count);
                lightPoint.append([node_y/count, node_x/count]);
            ImgBFS[i,j] = 1;
    return lightPoint;


FirstEnter = 1;
circlePoint_1 = [];
circlePoint_2 = [];
circlePoint_3 = [];
nodeList = [];
ID = 0;

def BoundingBoxStable(circlePoint):
    global FirstEnter
    global nodeList
    global circlePoint_1
    global circlePoint_2
    global circlePoint_3
    global ID
    circlePointMask = [];
    LOST = 0;
    if(FirstEnter == 1):
        nodeList = [];
        
        for node in circlePoint:
            ID = ID + 1;
            nodeList.append((ID, LOST, node));
            circlePointMask.append((ID, LOST, node));
        circlePoint_1 = circlePointMask;
        circlePoint_2 = circlePoint_1;
        circlePoint_3 = circlePoint_2;
        FirstEnter = 0;
    else:
        MaskID = 0;
        for node in circlePoint:
            MaskID = MaskID + 1;
            circlePointMask.append((MaskID, LOST, node));    
        circlePoint_3 = circlePoint_2;
        circlePoint_2 = circlePoint_1;
        circlePoint_1 = circlePointMask;
        
        for j in range(0, len(nodeList)):
            ret_sum = 0;
            for i in range(0, len(circlePointMask)):
                ret = BoundingBoxNearVerify(nodeList[j], circlePointMask[i], 8);
                print(ret);
                if not ret:
                    ret_sum += 1;
                    node = list(circlePointMask[i]);
                    node[1] += 1;
                    circlePointMask[i] = tuple(node);
                    #node_check[1] = 1;
            print(ret_sum);
            if ret_sum == len(circlePointMask):
                node = list(nodeList[j]);
                node[1] += 1;
                nodeList[j] = tuple(node);
                
                
        DelMask = [];
        for i in range(0, len(nodeList)):
            if nodeList[i][1] > 1:
                DelMask.append(i);
        AddMask = [];
        for i in range(0, len(circlePointMask)):
            if circlePointMask[i][1] == len(nodeList):
                AddMask.append(i);
         
            
        for i in DelMask:
            del(nodeList[i]);
            
        for i in AddMask:
            ID = ID + 1;
            nodeList.append((ID, LOST, circlePointMask[i][2]));
    return nodeList;
            
    
def BoundingBoxNearVerify(node, node2check, gap):
    if(abs(node[2][1] - node2check[2][1]) < gap) and (abs(node[2][0] - node2check[2][0]) < gap):
        return 1;
    else:
        return 0;
    
def ImageResize(Image, factor):
    ResizeImage = cv2.resize(Image, (0,0), fx=factor, fy=factor);
    return ResizeImage
    

def ImageThreshold(Image, ThresholdHigh, ThresholdLow):
    _, ImageCb, _ = ImageSpliteSpace(Image);
    #ImgThreshold = cv2.threshold(ImageCb, Threshold, 255, cv2.THRESH_BINARY);
    #ImgThreshold = cv2.threshold(ImageCb, 255 - Threshold, 255, cv2.THRESH_BINARY_INV);
    height, width = ImageCb.shape;
    ImgThreshold = np.zeros((height, width), dtype = "uint8");
    
    for i in range(0, height):
        for j in range(0, width):
            if (ImageCb[i,j] < ThresholdLow):
                ImgThreshold[i,j] = 255;
            elif (ImageCb[i,j] > ThresholdHigh):
                ImgThreshold[i,j] = 255;
    return ImgThreshold;
    
def ImageShow(Name, Image, flag):
    if(flag == 1):
        chan0, chan1, chan2 = ImageSpliteSpace(Image);
        #cv2.namedWindow("Image_chan0");
        #cv2.imshow("Image_chan0", chan0);
        cv2.namedWindow("Image_chan1");
        cv2.imshow("Image_chan1", chan1);
        #cv2.namedWindow("Image_chan2");
        #cv2.imshow("Image_chan2", chan2);
        cv2.waitKey(20);
        
    #elif(flag == 2):    
        #plt.imshow(Image);
        #plt.show();
    else:
        cv2.namedWindow(Name);
        cv2.imshow(Name, Image);
        cv2.waitKey(20);
        #cv2.destroyAllWindows()


def ImageSpliteSpace(img):
    channel0 = cv2.split(img)[0];
    channel1 = cv2.split(img)[1];
    channel2 = cv2.split(img)[2];
    return channel0,channel1,channel2;


def CaluYCbCr(img):
    YCbCrImg = cv2.cvtColor(img, cv2.COLOR_BGR2YCrCb);
    return YCbCrImg;

def CaluHSV(img):
    HSVImg = cv2.cvtColor(img, cv2.COLOR_BGR2HSV);
    return HSVImg;



def main():
    #Image = ImageLoad("../ImgData/6.png");
    #Detection(Image);
    #CAP = VideoLoad("../VideoData/sample11.avi");
    CAP = VideoLoad("../../../DeepLearningData/trafficLight/sample33.avi");
    VideoDetection(CAP);
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main();