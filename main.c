int OneArgOneReturn(int a){
    int b=a+200;
    return b;
}

int FiveArgOneReturn(int a,int b,int c,int d,int e){
    int f=(a+b)*c+d+e;
    return f;
}

void MutiReturn(int *a0,int *a1,int *a2,int *a3,int *a4){
        a0=a0+100;
        a1=a1+100;
        a2=a2+100;
        a3=a3+100;
        a4=a4+100;
}

void reset_handler(void){
    int a0=0;
    int a1=1;
    int a2=2;
    int a3=3;
    int a4=4;

    int x= OneArgOneReturn(1);
    int y=FiveArgOneReturn(1,2,3,4,5);
    MutiReturn(&a0,&a1,&a2,&a3,&a4);
	while (1)
		;
}
