# LearnOpenGL学习



## start01

1、创建openGL窗口

glad、glfw

2、熟悉openGL

VAO、VBO、EBO(IBO)

vertex shader、fragment shader

自定义shader类，加载顶点、片装着色器

纹理加载 sbi_image.h

向量与矩阵

坐标变换 model view pecjective

## start02

#include <geometry/SphereGeometry.h>

#include <geometry/TorusGeometry.h>

#include <geometry/PlaneGeometry.h>

自定义3d图形 

BasicGeometry.h

*Geometry.h

## start03

添加imgui

定义camera类

## start04

光照

颜色向量(Color Vector)：一个通过红绿蓝(RGB)分量的组合描绘大部分真实颜色的向量。一个物体的颜色实际上是该物体所不能吸收的反射颜色分量。

冯氏光照模型(Phong Lighting Model)：一个通过计算环境光，漫反射，和镜面光分量的值来估计真实光照的模型。

环境光照(Ambient Lighting)：通过给每个没有被光照的物体很小的亮度，使其不是完全黑暗的，从而对全局光照进行估计。

漫反射着色(Diffuse Shading)：一个顶点/片段与光线方向越接近，光照会越强。使用了法向量来计算角度。

法向量(Normal Vector)：一个垂直于平面的单位向量。

法线矩阵(Normal Matrix)：一个3x3矩阵，或者说是没有平移的模型（或者模型-观察）矩阵。它也被以某种方式修改（逆转置），从而在应用非统一缩放时，保持法向量朝向正确的方向。否则法向量会在使用非统一缩放时被扭曲。

镜面光照(Specular Lighting)：当观察者视线靠近光源在表面的反射线时会显示的镜面高光。镜面光照是由观察者的方向，光源的方向和设定高光分散量的反光度值三个量共同决定的。

冯氏着色(Phong Shading)：冯氏光照模型应用在片段着色器。

Gouraud着色(Gouraud shading)：冯氏光照模型应用在顶点着色器上。在使用很少数量的顶点时会产生明显的瑕疵。会得到效率提升但是损失了视觉质量。

GLSL结构体(GLSL struct)：一个类似于C的结构体，用作着色器变量的容器。大部分时间用来管理输入/输出/uniform。

材质(Material)：一个物体反射的环境光，漫反射，镜面光颜色。这些东西设定了物体所拥有的颜色。

光照属性(Light(properties))：一个光的环境光，漫反射，镜面光的强度。可以使用任何颜色值，对每一个冯氏分量(Phong Component)定义光源发出的颜色/强度。

漫反射贴图(Diffuse Map)：一个设定了每个片段中漫反射颜色的纹理图片。

镜面光贴图(Specular Map)：一个设定了每一个片段的镜面光强度/颜色的纹理贴图。仅在物体的特定区域显示镜面高光。

定向光(Directional Light)：只有一个方向的光源。它被建模为不管距离有多长所有光束都是平行而且其方向向量在整个场景中保持不变。

点光源(Point Light)：一个在场景中有位置的，光线逐渐衰减的光源。

衰减(Attenuation)：光随着距离减少强度减小的过程，通常使用在点光源和聚光下。

聚光(Spotlight)：一个被定义为在某一个方向上的锥形的光源。

手电筒(Flashlight)：一个摆放在观察者视角的聚光。

GLSL Uniform数组(GLSL Uniform Array)：一个uniform值数组。它的工作原理和C语言数组大致一样，只是不能动态分配内存。

## start05
加载模型

自定义Mesh、model类

使用aissmp

## start06
天空盒 cubeMaps

## start07
实例化 ,主要用于渲染多物体时提升GPU性能

抗锯齿算法(MSSAA) # todo 自定义抗锯齿


## start08
高级光照处理

blinn-Phong







