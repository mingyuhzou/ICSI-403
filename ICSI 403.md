# ICSI 403

# Fundamentals

software architectureis produced **先于** a software design

两个重要的目标：**Design for change**应对未来可能出现的变化，**Product families**将正在设计的系统视为一个更大程序家族的一部分，而不是一个孤立的、单一的产品。



Module指的是： **A well-defined component of a software system**，**A part of a system that provides a set of services  to other modules**

模块之间的关系可以用集合描述：

1. **模块集合**：定义了一个模块集合S，其中包含多个模块，表示为 $$S = \{M_1, M_2, M_3, \ldots\} $$。
2. **二元关系**：在集合 S上定义了一个二元关系 r。这个关系是集合 S 与自身的笛卡尔积 $$S \times S $$)的一个子集。也就是说，如果 $M_i $ 和 $M_j$ 是集合 S 中的模块，那么有序对 $\langle M_i, M_j \rangle  $可以属于关系 r，表示为 $M_i \, r \, M_j $。
3. **传递闭包 ( $r^+$)**：传递闭包 $r^+ $是关系 $r$ 的扩展，包含所有通过 $r$ 传递的路径。形式上，$$M_i \, r^+ \, M_j$$  当且仅当：$$M_i \, r^+ \, M_j$$（即 $M_i$  直接与 $M_j$ 有关系），或者存在一个中间模块 $M_k$使得 $$M_i \, r \, M_k $$且 $$M_k \, r^+ \, M_j $$。
4. **层次结构**(hierarchy)：关系r 被称为层次结构，如果不存在两个不同的元素 $M_i $和 $M_j$ 使得 $ M_i \, r^+ \, M_j $且 $ M_j \, r^+ \, M_i $。即不存在循环依赖，关系是单向的，形成一个有向无环图（DAG）。

<img src="./assets/image-20250303224711981.png" alt="image-20250303224711981" style="zoom:67%;" />



USES关系：一个模型元素需要另一个模型元素——A uses B，则A需要B的**正确结果**，依赖于B来提供**他自己的服务**，A可以通过B的**接口**来访问B的服务，这种关系是“**静态**”定义的，即它在设计或编译时就已经确定，而不是在运行时动态建立的。**这里的A是B的客户，而B是服务端。**

USES关系需要有**层次结构**，这样能让软件更容易被理解，建立和测试。

​	









**Coupling耦合，Cohesion内聚力**这两个指标可以决定一个结构的质量，模块化设计的原则是**高内聚、低耦合**，即每个模块需要**highly cohesive**(module understandable as a meaningful unit以及模块的组件紧密相关)，**Iow coupling**(与其他模块交流较少，可以单独理解）

<img src="./assets/image-20250302184742296.png" alt="image-20250302184742296" style="zoom: 50%;" />



USES关系指的**是某个模块依赖另一个模块的关系**





从**USES**图上理解：一个好的结构需要**low fan-in, high fan-out**，fan-in指模块被使用的频次，high fan-in表示模块经常被其他模块使用，**fan-out指模块调用其他模块的数量**，高扇出表示模块依赖过多的其他模块，缺乏内聚性，是设计不良的表现。



# C++

C++中**系统定义**的标识符以**下划线**开头，并且对**大小写**敏感。

<img src="./assets/image-20250304210222098.png" alt="image-20250304210222098" style="zoom:67%;" />



<img src="./assets/image-20250304223205471.png" alt="image-20250304223205471" style="zoom:80%;" />





前自增：**先加一再返回**

后自增：**先返回再加一**



可以从**小的数据类型转化到大的数据类型**，反过来需要**显示地定义**，布尔值是例外，会将整数转换为0或1

<img src="./assets/image-20250308180555316.png" alt="image-20250308180555316" style="zoom:67%;" />

单引号包裹字符，双引号包裹字符串。





```C++
const int max_value = 100;  // 正确：声明时初始化
max_value = 200;  // 错误：const 变量不能被重新赋值
```



和使用多个cout输出没有区别

<img src="./assets/image-20250308180941235.png" alt="image-20250308180941235" style="zoom: 67%;" />



下述两个方法设置输出位

```C++
// 设置浮点数输出为定点表示法
cout.setf(ios::fixed);
// 设置小数点后保留 2 位
cout.precision(2);
```

cin可以读取一行(遇到换行符停止)，输入的内容可以用空格分开



不支持/\*\*…*/的注释形式



逻辑判断有**短路**的性质



逻辑判断以下述的形式存在

<img src="./assets/image-20250308182352119.png" alt="image-20250308182352119" style="zoom:50%;" />



枚举类型 enum Color { red, green, blue };  Color r = red;



使用switch时，需要注意在case之后加上**break**

<img src="./assets/image-20250308182432553.png" alt="image-20250308182432553" style="zoom: 50%;" />



三元运算符

<img src="./assets/image-20250308182536264.png" alt="image-20250308182536264" style="zoom:67%;" />



循环

<img src="./assets/image-20250308182658070.png" style="zoom: 67%;" />

<img src="./assets/image-20250308182714415.png" alt="image-20250308182714415" style="zoom:67%;" />



srand()设置随机数种子  rand()设置0到32767之间的随即谁



`exit()`函数定义在`<cstdlib>`头文件中。它的作用是**立即终止程序的执行**，并将控制权返回给操作系统，该函数接受一个参数表示程序的状态退出码，为0表示成功退出。



## 参数

**按引用传值就是传递变量本身**，&名为ampersand

<img src="./assets/image-20250308185143789.png" alt="image-20250308185143789" style="zoom:67%;" />

函数重载必须要有**相同的返回值类型**，以及**不同的参数**，对于不同的重载函数，如果参数不能完美匹配会尝试进行**类型转换**再匹配到正确的函数。



默认参数的设置必须从**从右往左**直到**第一个**，传入参数时必须**从左往右填入**，不能跳过。

```C
void func(int a, int b = 20, int c = 30, int d = 40); // 正确
void func(int a = 10, int b, int c = 30); // 错误
void func(int a, int b, int c = 30) // 错误
```

​	

## 预定义指令

#define**定义** 仅在**编译时可见**的值，一般用于**条件编译**即根据**某个标识符**来决定那些代码需要**被编译**。

\#define TEST 1 定义一个 `TEST` 标识符，它的值为 `1`，这是一个编译时的定义，程序在执行时**不会储存**这个变量。



不同于if else，不满足片段是**不会被编译**的。

<img src="./assets/image-20250308192916229.png" alt="image-20250308192916229" style="zoom: 50%;" />







assertions断言是一种**调试**工具，通过 \#include \<cassert\>导入，如果 assert (condition)的结果为**False**则程序会**退出执行**。

想要禁用断言，需如下操作

```C++
#define NDEBUG  // 在断言之前定义
#include <cassert>
```





## 数组

数组的大小不能用**变量**定义，但是**常量**是允许的。

<img src="./assets/image-20250308190715405.png" alt="image-20250308190715405" style="zoom:67%;" />

数组作为参数是按照**引用**传递的(可以被**修改**)，但单个数组元素是按传**值**的方式进行的，。

在参数中定义**const**可以防止修改数组

![image-20250308191005077](./assets/image-20250308191005077.png)



返回数组的函数的返回类型应该定义为指针，而不是数组

<img src="./assets/image-20250308191212091.png" alt="image-20250308191212091" style="zoom: 50%;" />



对于多维数组，作为参数时必须**指明**除了第一个维度之外的**维度的大小**

<img src="./assets/image-20250308191355230.png" alt="image-20250308191355230" style="zoom:67%;" />



## 指针

指针可以指向**任何对象**，包括不指向**特定的对象**，可以通过p == q判断两个指针是否指向同一个地址，因为指针直接存储的就是**地址**。

<img src="./assets/image-20250308192105409.png" alt="image-20250308192105409" style="zoom:50%;" />

指针的操作演示

<img src="./assets/image-20250308191618990.png" alt="image-20250308191618990" style="zoom:67%;" />

对指针的增加会让指针指向**下一个内存地址**，而不是简单地加 1。它的具体行为取决于指针的 **数据类型**，因为不同类型的变量在内存中的占用大小不同。

​						**<img src="./assets/image-20250308191838990.png" alt="image-20250308191838990" style="zoom:50%;" />**





通过new返回的是一个**地址**，注意要及时**删除**。

<img src="./assets/image-20250308192005137.png" alt="image-20250308192005137" style="zoom: 50%;" />



## 结构体和类

结构体如下，结构体之间的赋值是内容的转移，除非是指针指向的结构体

<img src="./assets/image-20250308221340383.png" alt="image-20250308221340383" style="zoom:50%;" />



通过new创建的结构体即指针类型，需要使用**->**符来访问其中的变量



注意解引用的操作

acct1->balance同(*acct1).balance   *acct1.balance同 *(acct1.balance)



:: 具有**最高的优先级**，在**类外**定义类方法需要使用::，如果在类的内部定义则不需标识符，内部定义的方法是**inline函数**，违背了接口与实现分离的原则。

```C++
class Rectangle {
public:
    int width, height;

    int area(); // 仅声明函数
};

// 在类外部定义函数
int Rectangle::area() {
    return width * height;
}
```



# instruction to algorithm

P类问题：能在**多项式时间**内被**确定性图灵机**解决的问题，比如选择排序



NP类问题：能在**多项式时间**内被**非确定图灵机**解决的问题，**算起来不知道快不快**，但可以在多项式时间内验证这个问题。



NP-Hard类问题：需要花费更长的时间去检查，或者是没有一个有效的解的问题。



NP-Complete问题：存在这样一个NP问题，所有的NP问题都可以**约化**成它。换句话说，只要**解决**了这个问题，那么**所有**的NP问题都解决了。



**所有的NP-Complete都是NP-Hard的子集，但反之不成立。**



# 插入排序

tj表示第j个元素在while循环中所需执行的操作次数

![image-20250318185714012](./assets/image-20250318185714012.png)



注意统计执行次数时，循环的执行次数比实际次数多一



插入排序在最好的情况下while 循环不会执行，时间复杂度可以表示为an+b

<img src="./assets/image-20250318185859486.png" alt="image-20250318185859486" style="zoom:67%;" />



最坏的情况下，while循环会执行j次（比实际多一），这样总的时间复杂度

<img src="./assets/image-20250318190122019.png" alt="image-20250318190122019" style="zoom:67%;" />

<img src="./assets/image-20250318190146690.png" alt="image-20250318190146690" style="zoom:67%;" />

一般时间复杂度都指的是最坏情况下，而平均情况和最坏情况大致一样。



O表示的是渐进时间复杂度，只保留最高项并且去除常数，注意和T不能搞混。



# 二分查找

二分查找的时间复杂度计算涉及到递归，有式子

<img src="./assets/image-20250318190627348.png" alt="image-20250318190627348" style="zoom:80%;" />



# 归并排序



<img src="./assets/image-20250319163834011.png" alt="image-20250319163834011" style="zoom: 50%;" />





<img src="./assets/image-20250319164030091.png" alt="image-20250319164030091" style="zoom: 50%;" />





​	<img src="./assets/image-20250319165039161.png" alt="image-20250319165039161" style="zoom:67%;" />





# 树结构

<img src="./assets/image-20250319165534325.png" alt="image-20250319165534325" style="zoom:67%;" />

感觉不对的地方

![image-20250319165713656](./assets/image-20250319165713656.png)

![image-20250319165644122](./assets/image-20250319165644122.png)



​	

这里的Predecessors and Successors指的是中序



注意二叉树的插入操作，需要快慢指针操作

<img src="./assets/image-20250321141550788.png" alt="image-20250321141550788" style="zoom: 50%;" />





删除操作需要考虑更多



删除根节点，且根节点没有子节点——重新赋值指针

<img src="./assets/image-20250321142406750.png" alt="image-20250321142406750" style="zoom:67%;" />



删除有一个子节点的节点——重新连接

<img src="./assets/image-20250321142436944.png" alt="image-20250321142436944" style="zoom: 50%;" />



删除叶子节点——对于父节点设置指针为ULL

<img src="./assets/image-20250321142302234.png" alt="image-20250321142302234" style="zoom:50%;" />



删除的节点有两个子节点，将该节点的值与其中序后一位节点的值交换然后，再删除交换后的后一位

<img src="./assets/image-20250321142619129.png" alt="image-20250321142619129" style="zoom: 50%;" />







# 堆排序

堆排序结合了归并排序和插入排序的优点，与归并排序一样保证最坏的时间复杂度是**nlog(n)**，它是**原地**排序不需要**额外的空间**，与快排相比也不需要**栈开销**，但一般没有快排快。

堆是一个**完全二叉树**，因此树高是log(n)，堆的操作的时间复杂度也是log(n)

h.heapsize指代有多少元素是属于堆的

对于节点i，它的左子树是**i*2**，右子树是**2\*i+1**，父节点是**floor(i/2)**，注意课件中的版本i从1开始

 MAX-HEAPIFY操作中保证当前节点是子树中最大的(通过交换)，如果节点被交换过，那么继续操作子树

<img src="./assets/image-20250326163856862.png" alt="image-20250326163856862" style="zoom: 50%;" />



建堆的过程是从叶子节点的父节点开始往上，因为叶子节点本身已经是一个根堆了	

<img src="./assets/image-20250326164110315.png" alt="image-20250326164110315" style="zoom:50%;" />



从结尾开始交换来完成排序

<img src="./assets/image-20250326164421883.png" alt="image-20250326164421883" style="zoom:67%;" />



优先队列的功能

<img src="./assets/image-20250326165206189.png" alt="image-20250326165206189" style="zoom:50%;" />



<img src="./assets/image-20250326165312309.png" alt="image-20250326165312309" style="zoom:50%;" />



不允许改小节点的值，将节点与父节点交换

<img src="./assets/image-20250326165531091.png" alt="image-20250326165531091" style="zoom:50%;" />



<img src="./assets/image-20250326165557572.png" alt="image-20250326165557572" style="zoom:50%;" />
