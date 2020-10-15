#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define N 1000

typedef struct{
	double real;
	double img;
}complex;

void fft();    
void ifft();
void initW(); 
void change();
void add(complex ,complex ,complex *);
void mul(complex ,complex ,complex *);
void sub(complex ,complex ,complex *); 
void divi(complex ,complex ,complex *);
void output();                           


complex x[N], *W; 
int size_x=0;     
double PI;        
/**

	Please input the size of x:
	4
	Please input the data in x[N]:
	1 0
	0 2
	3 0
	0 4
	Use FFT(0) or IFFT(1)?
	0
	The result are as follows
	4.0000+6.0000j
	-4.0000
	4.0000-6.0000j
	0.0000 
 */
int main(){
	int i,method;

	// system("clear");
	PI=atan(1)*4;
	printf("Please input the size of x:\n");
	scanf("%d",&size_x);
	printf("Please input the data in x[%d]:\n",size_x);
	for(i=0;i<size_x;i++)
		scanf("%lf%lf",&x[i].real,&x[i].img);
	printf("%s %d \n",__FILE__,__LINE__);
	initW();
	printf("%s %d \n",__FILE__,__LINE__);
#if 0
	printf("Use FFT(0) or IFFT(1)?\n");
	scanf("%d",&method);
	if(method==0)
		fft();
	else
		ifft();
#endif
	fft();
	output();
	return 0;
}


void fft(){
	int i=0,j=0,k=0,l=0;
	complex up,down,product;
	change();
	for(i=0;i< log(size_x)/log(2) ;i++){  
		l=1<<i;
		for(j=0;j<size_x;j+= 2*l ){            
			for(k=0;k<l;k++){       
				mul(x[j+k+l],W[size_x*k/2/l],&product);
				add(x[j+k],product,&up);
				sub(x[j+k],product,&down);
				x[j+k]=up;
				x[j+k+l]=down;
			}
		}
	}
}




void initW(){
	int i;
	W=(complex *)malloc(sizeof(complex) * size_x);
	for(i=0;i<size_x;i++){
		W[i].real=cos(2*PI/size_x*i);
		W[i].img=-1*sin(2*PI/size_x*i);
	}
}


void change(){
	complex temp;
	unsigned short i=0,j=0,k=0;
	double t;
	for(i=0;i<size_x;i++){
		k=i;j=0;
		t=(log(size_x)/log(2));
		while( (t--)>0 ){
			j=j<<1;
			j|=(k & 1);
			k=k>>1;
		}
		if(j>i){
			temp=x[i];
			x[i]=x[j];
			x[j]=temp;
		}
	}
}


void output(){
	int i;
	printf("The result are as follows\n");
	for(i=0;i<size_x;i++){
		printf("%.4f",x[i].real);
		if(x[i].img>=0.0001)printf("+%.4fj\n",x[i].img);
		else if(fabs(x[i].img)<0.0001)printf("\n");
		else printf("%.4fj\n",x[i].img);
	}
}
void add(complex a,complex b,complex *c){
	c->real=a.real+b.real;
	c->img=a.img+b.img;
}

void mul(complex a,complex b,complex *c){
	c->real=a.real*b.real - a.img*b.img;
	c->img=a.real*b.img + a.img*b.real;
}
void sub(complex a,complex b,complex *c){
	c->real=a.real-b.real;
	c->img=a.img-b.img;
}
void divi(complex a,complex b,complex *c){
	c->real=( a.real*b.real+a.img*b.img )/( b.real*b.real+b.img*b.img);
	c->img=( a.img*b.real-a.real*b.img)/(b.real*b.real+b.img*b.img);
} 

