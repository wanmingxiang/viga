/***********************************************
*      filename: list.h
*      version: ---
*      
*      @author: wanmx@foxmail.com
*      @description: ---
*      @create: 2017-05-16 23:43:46
*      @last Modified: 2017-05-17 01:54:02
************************************************/

#include <glog/logging.h>
#include <iostream>

struct node_t {
	int id;
	node_t* next;
};

void print_list(node_t** head) {
	node_t* current = *head;
	while (current != NULL) {
		std::cout << current->id << " ";
		current = current->next;
	}
	std::cout << std::endl;
}

void reverse(node_t** head) {
	if (head == NULL || *head == NULL || (*head)->next == NULL)
		return;
	node_t* p = *head;
	node_t* cur = (*head)->next;
	node_t* cur_next = cur->next;
	p->next = NULL;
	while (cur != NULL) {
		cur->next = p;
		p = cur;
		if (cur_next != NULL) {
			cur = cur_next;
			cur_next = cur->next;
		} else {
			break;
		}
	}
	*head = p;

}

int main() {
	int test_in[] = {1,2,3,4,5,6,7,8};
	int arr_len = sizeof(test_in)/sizeof(test_in[0]);
	node_t* head = NULL;
	for (int i = 0; i < arr_len; ++i) {
		node_t *item = new node_t;
		item->id = test_in[i];
		item->next = NULL;
		item->next = head;
		head = item;
		LOG(INFO) << item->id;
	}
	
	print_list(&head);	
	reverse(&head);
	print_list(&head);	

}
