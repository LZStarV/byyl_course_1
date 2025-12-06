/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：cpp2.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
// Demo: add two integers and print the result
#include <iostream>

// Pure function: returns the sum of two ints
int add(int a, int b)
{
    return a + b;
}

int main()
{
    int s = add(1, 2);
    // Only print positive results
    if (s > 0)
    {
        std::cout << s << std::endl;
    }
    return 0;
}