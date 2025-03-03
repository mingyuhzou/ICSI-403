# ICSI 403

# Fundamentals

design指的是：**Provides structure to any product**，**Decomposes system into parts**， **refers to both an activity and the result of  the activity**(activity意为work as a bridge between requirement and implementation of the software，give a structure to the product)



 software architectureis produced **先于** a software design

两个重要的目标：**Design for change**，**Product families**



Module指的是： **A well-defined component of a software system**，**A part of a system that provides a set of services  to other modules**



**Coupling耦合，Cohesion内聚力**这两个指标可以决定一个结构的质量，模块化设计的原则是**高内聚、低耦合**，即每个模块需要**highly cohesive**(module understandable as a meaningful unit以及模块的组件紧密相关)，**Iow coupling**(与其他模块交流较少，可以单独理解）

<img src="./assets/image-20250302184742296.png" alt="image-20250302184742296" style="zoom: 50%;" />



USES关系指的**是某个模块依赖另一个模块的关系**





从**USES**图上理解：一个好的结构需要**low fan-in, high fan-out**，fan-in指模块被使用的频次，high fan-in表示模块经常被其他模块使用，**fan-out指模块调用其他模块的数量**，高扇出表示模块依赖过多的其他模块，缺乏内聚性，是设计不良的表现。

