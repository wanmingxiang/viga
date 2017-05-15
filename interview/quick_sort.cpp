
/***********************************************
*
*      Filename: sort.h
*
*      Author: wanmx@foxmail.com
*      Description: 
*      Create: 2017-05-15 17:00:44
*      Last Modified: 2017-05-15 18:47:06
************************************************/
#include <iostream>

void print_arr(int arr[], int idx, int arr_len) { 
        for (int i = idx; i < arr_len; i++) {
                std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
}

int partition(int arr[], int low, int high) {
        int first = low;
        int last = high;
        int key = arr[first];

        /* 
         *  arr[first] ---->  key          keep pivot
         *  arr[last]  ---->  arr[first]   get value less than pivot;
         *  arr[first] ---->  arr[last]    get value largert than pivot;
         */
        while (first < last) {
            while (key < arr[last] && first < last) 
                    last--;
            arr[first] = arr[last];
            
            while (key > arr[first] && first < last)
                    first++;
            arr[last] = arr[first];

        }
        arr[first] = key;
        return first;
}

void quick_sort(int arr[], int low, int high) {
   if (low < high) {
        int pivot = partition(arr, low, high); 
        quick_sort(arr, low, pivot - 1);
        quick_sort(arr, pivot + 1, high);
   } 
}


int main() {

    int test_in[] = {10,9,23,43,432,1,2,54};
    int arr_len =  sizeof(test_in)/sizeof(test_in[0]);
    quick_sort(test_in, 0, sizeof(test_in)/sizeof(test_in[0]) - 1); 
    print_arr(test_in, 0, 8);

}
