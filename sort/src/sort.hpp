#ifndef SORT_H
#define SORT_H

#include <vector>
#include <iostream>

void SelectSort(vector<int> &v);                    // 选择排序
void TournamentSort(vector<int> &v);                // 锦标赛排序
void HeapSort(vector<int> &v);                      // 堆排序

void BubbleSort(vector<int> &v);                    // 冒泡排序
void QSort(vector<int> &v,int low,int high);        // 快速排序

void InsertSort(vector<int> &v);                    // 非递减直接插入排序
void BinarySearchInsertSort(vector<int> &v);        // 折半查找直接插入排序
void TwoWayInsertSort(vector<int> &v);              // 二路插入排序
void ShellSort(vector<int> &v,int dlta[],int t);    // 希尔排序

void MergeSort(vector<int> &v); // 归并排序
#endif
