---
tags: 科學視算
---

# Visualization

[toc]

## Implement Marching Cube Method

- 使用 gradient 作為該點的 normal 來呈現光影
- 使用 VAO、VBO 加速 render
- 原本的 data 就是 regular grid 再用其他加速法感覺不太必要


| <img src=https://i.imgur.com/JdODyzZ.png width=100%> | <img src=https://i.imgur.com/KnU49Lt.png width=100%> |
| ---------------------------------------------------- | ---------------------------------------------------- |
| <img src=https://i.imgur.com/Ty5nhwt.png width=100%> | <img src=https://i.imgur.com/5seVq8G.png width=100%> |



## Extra

- Cross Section
- multiple iso-surfaces 以及 clipping plane


| <img src=https://i.imgur.com/vRkFdwr.png width=100%> | <img src=https://i.imgur.com/frA19Pk.png width=100%> |
| ---------------------------------------------------- | ---------------------------------------------------- |
| <img src=https://i.imgur.com/s9r3StZ.png width=100%> | <img src=https://i.imgur.com/LgJChtn.png width=100%> |


<!-- <img src=https://i.imgur.com/3M1FXIz.png width=100%> -->



| ![](https://i.imgur.com/tKlKxZ5.png)       |
| ------------------------------------------ |
| (把硬度為 180 的跟硬度為 80 的一起 render) |

## Histogram

紀錄每個 voxel 的 value 後建成一個 table，然後用 imgui 的 histogram 畫出

<img src="https://i.imgur.com/5hjHhE8.png" width=50% />

## M x K Table

紀錄每個 voxel 的 gradient，算出長度後跟 voxel value 一起組成一組座標，累加每一組座標出現過幾次來決定顏色的深淺

使用 implot 的 canvas 來畫出類似熱力圖的樣子，調整 gMax 可以調整 canvas 縮放的比例

<img src="https://i.imgur.com/AMLfqOG.png" width=50% />

## Ray Trace Volume Rendering

![](https://i.imgur.com/ozk0c94.jpg)

覺得有 phong shading 反而沒有比較好


| ![](https://i.imgur.com/WU78rNw.jpg) | ![](https://i.imgur.com/VBhXYUC.png) |
| ------------------------------------ | ------------------------------------ |


## 取樣頻率的差異

取樣頻率過低會導致出現斷層，有一圈一圈的痕跡。

```
低     中


高
```

| ![](https://i.imgur.com/m29oEWG.png) | ![](https://i.imgur.com/jbbBA7T.png) |
| ------------------------------------ | ------------------------------------ |
| ![](https://i.imgur.com/tS9Xa8H.png) |  |

## Leg_CT

皮膚 -> 軟組織 -> 軟骨 -> 骨頭

![](https://i.imgur.com/irHRve5.jpg)

## Uniform Streamline

| 1.vec                                | 11.vec                               | 20.vec                               |
| ------------------------------------ | ------------------------------------ | ------------------------------------ |
| ![](https://i.imgur.com/FwHreVm.png) | ![](https://i.imgur.com/YomnNPp.png) | ![](https://i.imgur.com/Tontzj6.png) |


| rect1.vec                            | rect2.vec                            |
| ------------------------------------ | ------------------------------------ |
| ![](https://i.imgur.com/BvIQtAE.png) | ![](https://i.imgur.com/SY0uT0P.png) |



- test_not_unit.vec

![](https://i.imgur.com/rRtMXVC.png)

- $\vec{U}(x,y)=[xy^2\ x^2y],-5\le x,y\ge 5$

![](https://i.imgur.com/pbHcMmf.png)

## High Dimensional Data Visualization

使用PCA method

將 data normalize 後乘自己的 transpose 然後取 eigen value 最大的兩個 eigen vector 當 x y 軸然後將每個點投影到這兩個軸上

cluster detection 是使用原始 data 的歐式距離作 union find
距離低於 threshold(GUI中可調整) 視為同一類

![](https://i.imgur.com/yObvYRQ.png)