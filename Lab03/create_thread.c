#include <pthread.h>
#include <stdio.h>

char *pHello="Hello world";

void *myThread(void* pStr) { // Đây là mã của luồng
   printf("%s\n", (char*) pStr);
   printf("Luong ket thuc\n");
}

void main() {
   pthread_t tid;
   
   // tham số: threadid, thuộc tính luồng (mặc định=NULL), con trỏ hàm xl luồng, tham số hàm
   pthread_create(&tid, NULL, myThread, (void*) pHello);   
   
   // cha chờ *myThread kết thúc
   pthread_join(tid, NULL);  // tham số: threadid luồng con, con trỏ kết quả trả lại từ luồng con
   printf("Tien trinh cha ket thuc\n");
}
