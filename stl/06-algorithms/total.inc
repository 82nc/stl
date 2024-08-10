
/*

STL 算法总览

//不在STL标准算法之列的SGI专属算法均以*标记
//质变栏意指mutating,意思是会改变其操作对象之内容
//质变算法通常提供版本: 一个是in-place(就地进行)版本,就地改变其操作对象,另一个是copy(另地进行)版,
//将操作对象的内容复制一份副本,然后在副本上进行修改并返回该副本copy版本总是以_copy作为函数名称尾词,
//但不是所有质变算法都有copy版本,如sort.

//所有的数值(numeric)算法,包括adjacent_difference(),accumulate(),inner_product(),
//partial_sum()等等,都实现于<stl_numeric.h>之中,这是个内部文件,STL规定用户必须包含上层的
//<numeric>.其他STL算法都实现在<stl_algo.h>和<stl_algobase.h>文件中,也都是内部文件,欲使用这些
//算法,必须包含上层相关头文件<algorithm.h>

--------------------------------------------------------------------------------------
算法名称                     算法用途                       质变              所在文件
--------------------------------------------------------------------------------------
accumulate                  元素累计                                       <stl_numeric.h>
adjacent_difference         相邻元素的差额                  Y if in-place   <stl_numeric.h>
adjacent_find               查找相邻而重复/                                 <stl_algo.h>
                            符合某种条件的元素               
binary_search               二分查找                                       <stl_algobase.h>
Copy                        复制                          Y if in-place   <stl_algobase.h>
Copy_backward               逆向复制                       Y if in-place   <stl_algobase.h>
Copy_n *                    复制n个元素                    Y if in-place   <stl_algobase.h>
count                       计数                                          <stl_algo.h>
count_if                    在特定条件下计数                                <stl_algo.h>
equal                       判断区间相等                                   <stl_algobase.h>
equal_if                    有序区间内寻找指定值的范围                       <stl_algo.h>
fill                        改填元素值                     Y               <stl_algobase.h>
fill_n                      改填元素值,n次                 Y               <stl_algobase.h>
find                        循环查找                                       <stl_algo.h>
find_if                     循环查找符合特定条件者                           <stl_algo.h>
find_end                    查找某个子序列的最后出现点                       <stl_algo.h>
find_first_of               查找某个子序列的首次出现点                       <stl_algo.h>
for_each                    对区间内每个元素施行某操作                       <stl_algo.h>
generate                    以特定操作之运算结果填充         Y               <stl_algo.h>
                            特定区间的元素
generate_n                  以特定操作之运算结果填充         Y               <stl_algo.h>
                            n个元素内容
includes                    是否涵盖于某序列之中                            <stl_algo.h>
inner_product               内积                                          <stl_numeric.h>
inplace_merge               合并并就地替换                  Y              <stl_algo.h>
Iota *                      在某个区间填入某指定值的         Y               <stl_numeric.h>
                            递增序列
is_heap *                   判断某个区间是否为一个heap                      <stl_algo.h>
is_sorted *                 判断某个区间是否已排序                          <stl_algo.h>
iter_swap                   元素互换                       Y              <stl_algobase.h>
lexicographical_compare     以字典序进行比较                               <stl_numeric.h>
lower_bound                 将指定元素插入区间之内而不影                     <stl_algo.h>
                            响区间之原本排序的最低位置
max                         最大值                                        <stl_algobase.h>
max_element                 最大值所在位置                                 <stl_algo.h>
merge                       合并两个序列                   Y if in_place   <stl_algo.h>
min                         最小值                                        <stl_algobase.h>
min_element                 最小值所在位置                                 <stl_algo.h>
mismatch                    找出不匹配点                                   <stl_algobase.h>
next_permutation            获得下一个排列组合              Y               <stl_algo.h>
nth_element                 重新安排序列中的第n             Y               <stl_algo.h>
                            个元素的左右两端
partial_sort                局部排序                       Y              <stl_algo.h>
partial_sort_copy           局部排序并复制到他处             Y if in-place  <stl_algo.h>
partial_sum                 局部求和                       Y if in-place  <stl_numeric.h>
partition                   分割                          Y              <stl_algo.h>
prev_permutation            获得前一个排列组合              Y              <stl_algo.h>
power *                     幂次方,表达式可指定
random_shuffle              随机重排元素                   Y               <stl_algo.h>
random_sample *             随机取样                      Y if in-place   <stl_algo.h>
random_sample_n *           随机取样                      Y if in-place   <stl_algo.h>

remove                      删除某类元素(但不删除)          Y              <stl_algo.h>
remove_copy                 删除某类元素并将结果复制         Y              <stl_algo.h>
                            到另外一个容器
remove_if                   有条件的删除某类元素            Y              <stl_algo.h>
remove_copy_if              有条件地删除某类元素并将        Y              <stl_algo.h>
                            结果复制到另一个容器
replace                     替换某类元素                   Y             <stl_algo.h>
replace_copy                替换某类元素,并将结果复制       Y              <stl_algo.h>
                            到另外一个容器
replace_if                  有条件地替换                  Y              <stl_algo.h>
replace_copy_if             有条件地替换,并将结果复制       Y              <stl_algo.h>
                            到另外一个容器
reverse                     反转元素次序                  Y              <stl_algo.h>
reverse_copy                反转元素次序并将结果复制到      Y              <stl_algo.h>
                            另外一个容器
rotate                      旋转                         Y              <stl_algo.h>
rotate_copy                 旋转,并将结果复制到另一个容器   Y              <stl_algo.h>
search                      查找某个子序列
search_n                    查找连续发生n次的子序列
set_difference              差集                        Y if in-place   <stl_algo.h>       
set_intersection            交集                        Y if in-place   <stl_algo.h>
set_symmetric_difference    对称差集                     Y if in-place   <stl_algo.h>
set_union                   并集                        Y if in-place   <stl_algo.h>
sort                        排序                        Y               <stl_algo.h>
stable_partition            分割并保持元素的相对次序       Y               <stl_algo.h>
stable_sort                 排序并保持等值元素的相对次序   Y               <stl_algo.h>
swap                        交换(对调)                  Y               <stl_algobase.h>
swap_ranges                 交换(以指定区间)             Y               <stl_algo.h>
transform                   以两个序列为基础,交互作用产生  Y               <stl_algo.h>
                            第三个序列
unique                      将重复元素折叠缩编,使其成唯一  Y               <stl_algo.h>
unique_copy                 将重复元素折叠缩编,使其成唯一  Y if in-place   <stl_algo.h>
                            并复制到他处
upper_bound                 将指定元素插入区间之内而不影响
                            区间之原本排序的最高位置
make_heap                   制造一个heap                Y               <stl_heap.h>
pop_heap                    从heap取出一个元素           Y               <stl_heap.h>
push_heap                   将一个元素推进heap内         Y               <stl_heap.h>
sort_heap                   对heap排序                 Y               <stl_heap.h>

 */