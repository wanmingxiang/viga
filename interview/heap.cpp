/***********************************************
*      filename: heap.cpp
*      version: ---
*      
*      @author: wanmx@foxmail.com
*      @description: ---
*      @create: 2017-05-15 23:05:59
*      @last Modified: 2017-05-16 01:54:09
************************************************/

#include <iostream>

class MyHeap {
public:
	MyHeap() {

	}
	
	MyHeap(int arr[], int num) {
		heap_len = num;
		h = new int[num];
		for (int i = 0; i < num; ++i)
			h[i] = arr[i];
	}

	~MyHeap() {

	}

	void print_content() {
		for (int i = 0; i < heap_len; ++i) 
			std::cout << h[i] << " ";
		std::cout << std::endl;
	}

	void swap(int* a, int* b) {
		*a = *a^*b;
		*b = *a^*b;
		*a = *a^*b;
	}

	void shift_down(int i) {
		int t = i;
		int flag = 1;
		while ((2*i + 1) < heap_len && flag == 1) {	
			if ((2*i + 2) > heap_len) {
				if (h[2*i + 1] < h[i]) {
					t = 2*i + 1;
				} else {
					t = i;
				}
			} else {
				if (h[2*i + 1] < h[i]) {
					t = 2*i + 1;
				} else {
					t = i;
				}
				if (h[2*i + 2] < h[t]) {
					t = 2*i + 2;
				}
			}
			if (t != i) {
				swap(&h[t], &h[i]);
				i = t;
				flag = 1;
			} else {
				flag = 0;
			}
		}
	}

        void build_heap() {
		for (int i = (heap_len/2 - 1); i >= 0; i--) {
			std::cout << "num:" << i << std::endl;
			shift_down(i);
			print_content();
		}
	}

private:
	int* h;
	int heap_len;

};


int main() {
	int test_in[] = {23,43,32,12,56,39,2,93,25,53,87,123,4};
	MyHeap myheap(test_in, 13);
	myheap.print_content();
	myheap.build_heap();
	myheap.print_content();

}
