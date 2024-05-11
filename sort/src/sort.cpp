#include "sort.hpp"

/* 选择排序 */
void SelectSort(vector<int> &v)
{
    int n=v.size();
    for(int i=0;i<n-1;i++)//进行n-1趟选择排序，每一趟选择最小的值与v[i]交换
    {
        int min=i;
        for(int j=i+1;j<n;j++)
        {
            if(v[j]<v[min])
                min=j;
        }
        if(min!=i)
            swap(v[i],v[min]);
    }
}


/* 锦标赛排序 */
void TournamentSort(vector<int> &v)
{
    int m=v.size();//叶子结点个数
    //总的结点数可以确定叶子结点个数，反之不行
    //仅仅通过叶子结点数无法确定总结点数即无法确定完全二叉树的模样
    //为了可以两两比较，我们让该二叉树没有度为1的结点方便比较
    //而真正的锦标赛排序的参赛选手个数必须的是在2^i的中选取以保证每一轮都偶数个参赛选手
    int n=2*m-1;
    vector<int> t(n+1,0);//分配n+1个空间,只使用1～n
    for(int i=m-1;i>=0;--i)
    {
        t[n-m+i]=v[i];
    }
    for(int i=n/2;i>=1;--i)//第一趟比较选出最小关键字
    {
        t[i]=t[2*i]<t[2*i+1]?t[2*i]:t[2*i+1];
    }
    v[0]=t[1];//此处第一轮比较得到最小关键字
    int k=1;//看这个最小关键字是属于哪个叶子结点
    for(int i=1;i<m;++i)
    {
        while(2*k<=n)
        {
            if(t[k]==t[2*k])
                k=2*k;
            else
                k=2*k+1;
        }
        t[k]=INT_MAX;//修改从叶子结点到根结点上到关键字得到次小关键字
        while(k!=1)
        {
            k=k/2;
            t[k]=t[2*k]<t[2*k+1]?t[2*k]:t[2*k+1];
        }//得到k==1
        v[i]=t[1];//得到次小关键字
    }
}


/* 堆调整算法 */
void static HeapAdjust(vector<int> &v,int cur,int tail)
{
    int val=v[cur];
    for(int k=2*cur;k<=tail;k*=2)
    {
        if(k<tail&&v[k]<v[k+1])//k指向cur左右子树较大值，k<tail是确保cur有右子树
            ++k;
        if(val>v[k])//没有必要继续调整，已经是大根堆了
            break;
        else{
            v[cur]=v[k];
            cur=k;//重新以k==cur为根节点调整树堆
        }
    }
    v[cur]=val;
}

/* 堆排序 */
void HeapSort(vector<int> &v)
{
    int n=v.size()-1;//0位置不用，符合完全二叉树堆位置对应关系
    for(int i=n/2;i>=1;--i)//开始对完全二叉树第一个非叶子节点进行堆调整
    {
        HeapAdjust(v,i,n);
    }//完成初始建堆
    for(int i=n;i>1;i--)//在此基础上只需要n-1次调整即可完成排序
    {
        swap(v[1],v[i]);//将堆顶记录和当前还未经排序的最后一个元素记录进行替换
        HeapAdjust(v,1,i-1);//堆调整,将vector[1~i-1]调整成一个新堆
    }
}


/* 冒泡排序 */
void BubbleSort(vector<int> &v)
{
    for(int i=1;i<v.size();i++)//只进行n-1趟排序,第i趟将最小值或者最大值放到v[v.size-i]处
    {
        for(int j=0;j<v.size()-i;j++)
        {
            if(v[j]>v[j+1])
                swap(v[j],v[j+1]);//交换
        }
    }
}

/* 快速划分 */
int static partition(vector<int> &v,int low,int high)
{
    int pivotkey=v[low];
    while(low<high)
    {
        while(low<high&&v[high]>=pivotkey) --high;
        v[low]=v[high];
        while(low<high&&v[low]<=pivotkey) ++low;
        v[high]=v[low];
    }
    v[low]=pivotkey;
    return low;
}

/* 快速排序 */
void QSort(vector<int> &v,int low,int high)
{
    if(low<high)
    {
        int loc=partition(v,low,high);
        QSort(v,low,loc-1);
        QSort(v,loc+1,high);
    }
}

/* 非递减直接插入排序 */
void InsertSort(vector<int> &v)     
{
    //对从i=1到i=v.size()-1的元素，插入到i位置之前的有序队列里
    for(int i=1;i<=v.size()-1;i++)//v.size()个元素,v.size()-1趟排序
    {
        int value=v[i];//待插入值
        int j;
        for(j=i-1;j>=0;j--)//从v[i]的前一个数开始往前找
        {
            if(value>=v[j])//不用继续往前比较，找到插入位置,退出当前for循环
            {
                break;
            }    
            else{
                v[j+1]=v[j];//后移
            }
        }
        v[j+1]=value;
    }
}

/* 折半查找直接插入排序 */
void BinarySearchInsertSort(vector<int> &v)
{
    for(int i=1;i<=v.size()-1;i++)//v.size()个元素,v.size()-1趟排序
    {
        if(v[i-1]>v[i])
        {
            int val=v[i];
            int l=0,h=i-1;
            while(l<=h)//折半查找
            {
                int m=(l+h)/2;
                if(v[m]>val)
                {
                    h=m-1;
                }else{
                    l=m+1;
                }
            }//l为插入位置
            for(int j=i-1;j>=l;--j)
            {
                v[j+1]=v[j];
            }
            v[l]=val;//插入元素
        }
    }
}

/* 二路插入排序 */
void TwoWayInsertSort(vector<int> &v)
{
    int n=v.size();
    vector<int> t(n);
    t[0]=v[0];//将t[0]看作是排好序的序列处在中间位置的记录
    int fr=0,bk=0;//前后指针
    for(int i=1;i<=v.size()-1;i++)//v.size()个元素,v.size()-1趟排序
    {
        if(v[i]>=t[0])//插入到t[0]之后的有序序列
        {
            for(int j=bk;j>=0;--j)
            {
                if(t[j]>v[i])
                {
                    t[j+1]=t[j];
                }else{
                    t[j+1]=v[i];
                    break;
                }
            }
            ++bk;
        }else{//插入到t[0]之前的有序序列
            if(v[i]<t[fr])
            {
                fr=(fr-1+n)%n;
                t[fr]=v[i];
            }else{
                for(int j=fr;j<n;++j)
                {
                    if(t[j]<=v[i])
                    {
                        t[j-1]=t[j];
                    }else{
                        t[j-1]=v[i];
                        break;
                    }
                }
                --fr;
            }
        }
    }
}



void static ShellInsert(vector<int> &v,int k)
{
    for(int i=k;i<v.size();i++)
    {
        if(v[i-k]>v[i])
        {
            int val=v[i];
            for(int j=i-k;j>=0;j=j-k)//类似直接插入排序，但是增量是k不是1
            {
                if(v[j]>val)
                {
                    v[j+k]=v[j];
                }else{
                    v[j+k]=val;
                    break;
                }
            }
        }
    }
}

/* 希尔排序 */
void ShellSort(vector<int> &v,int dlta[],int t)
{
    //按增量序列dlta对v做希尔排序
    for(int k=0;k<t;++k)
    {
        ShellInsert(v,dlta[k]);//增量为dlta[k]
    }
}


/* 归并有序数组 */
void static Merge(vector<int> &v,int start,int mid,int end)//归并算法
{
    int n=end-start+1;
    vector<int> t(n);
    int k=0;
    int j=mid+1;
    int i=start;
    for(;i<=mid&&j<=end;++k)
    {
        if(v[i]<=v[j])
        {
            t[k]=v[i++];
        }else{
            t[k]=v[j++];
        }
    }
    while(i<=mid)
    {
        t[k++]=v[i++];
    }
    while(j<=end)
    {
        t[k++]=v[j++];
    }
    for(int i=0;i<n;i++)//将归并后的有序序列重新赋值回去v
    {
        v[start+i]=t[i];
    }
}

void static MSort(vector<int> &v,int i,int j)
{
    if(i<j){
        int k=(i+j)/2;
        MSort(v,i,k);
        MSort(v,k+1,j);
        Merge(v,i,k,j);
    }
}

/* 归并排序 */
void MergeSort(vector<int> &v)
{
    MSort(v,0,v.size()-1);
}