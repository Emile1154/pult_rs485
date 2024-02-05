#include <model/queue.h>

uint8_t items[64];
uint8_t size = 0;

uint8_t isfull() {
  if (size == 63)
    return 1;
  else
    return 0;
}

uint8_t isempty() {
  if (size == 0)
    return 1;
  else
    return 0;
}

void push(uint8_t newitem) {
  if (isfull()) {

  } else {
    items[size] = newitem;
    size++;
  }
}

uint8_t pop() {
    if(isempty() ) {
        return 255;
    } 
    if(size == 1){
        size--;
        return items[0];
    }
    uint8_t tmp = items[0];
    for(uint8_t i = 1 ; i < size; i++){
        items[i-1] = items[i];
    }
    size--;
    return tmp;
}
