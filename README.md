# traffic transportation
## 交通科技大赛 -- 基于Wifi信令数据的地铁客流特征分析

## 代码实现:C++

## 使用说明
- 在traffic.h文件中修改subway_data passenger_data path_result scan_result路径 将subway.txt passenger.txt放到指定路径
- 第一次运行程序时 将CALCULATEPATH的宏定义值改为1 最短路径将保存为文件path_result 之后每次运行可将其置0 并从文件path_result中读取 免去计算所需时间
- 运行程序后 自动分析并计算出最短路径 保存在scan_result文件中(纯文本文件 可使用记事本打开)

- 在之后的命令行界面中可选择实现后续功能
1. 计算断面客流 按要求输入要计算的相邻两站所对应编号 即可得数据中面客流之和
2. 计算换乘数 按要求输入要计算的相邻三站所对应编号 即可得数据中换乘数之和
3. 计算分流比 按要求输入要计算的两站所对应编号 即可得数据中经过该起始点的路径和人数累加值及其占比

## 函数功能说明
### traffic.h文件
进行调用库声明 宏定义 全局函数声明 全局变量声明 全局结构体声明 全局类声明
### main.cpp文件
进行全局变量定义 全局结构体定义 全局类定义 主要函数调用
### data_read.cpp文件
- read_subway_data函数 格式化读取subway.txt数据并存储在sta类中
- read_passenger_data函数 格式化读取passenger.txt数据并存储在via类中
- read_path函数 在CALCULATEPATH宏定义值为0时 读取path_result中之前计算好的最短路数据并存储
### initialize.cpp文件
- init_adjacency_matrix函数 初始化邻接矩阵
- init_MGraph函数 初始化图
- calculate_path函数 计算最短路 当DEBUGPATH时 输出中间结果
- cut函数 将via类中的数据以trip_id为单位分片储存
### data_analysis.cpp文件
- Floyd函数 求网G（用邻接矩阵表示）中任意两点间最短路径(D是最短路径长度矩阵，path最短路径标志矩阵)
- find_next_sta函数 传入当前站和目标站 输出下一个要走的站
- scan_n函数 新的主干函数 逻辑见函数内注释
- short_line函数 传入trip_id head和tail指针和num指针 计算最短路并写入trip_path数组中
- complete_trip函数 计算原路径信息是否完整
### debug.cpp文件
- test_print_scan函数 将trip_path中保存的路径信息写入文件scan_result中
- interaction函数 命令行人机交互界面 计算断面客流 换乘数 分流比
