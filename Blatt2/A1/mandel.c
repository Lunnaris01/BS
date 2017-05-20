#define BMPHEADER_SIZE 54
#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include <pthread.h>
#include <malloc.h>

// there will be a low level I/O function from the operating system
extern long write(int, const char *, unsigned long);

float zoom      = 1.5;
float quadLimit = 3.0;
char colorLimit = 40;

typedef struct Complex_s {
    float re;
    float im;
} Complex;

typedef struct Thread_args {
    int try;
    int height;
    int width;
    int end;
    float relation;
    char *blueGreenRed;
} Thread_arg;

// bad, but fast !!!
int intFloor(double x) {
    return (int)(x+100000) - 100000;
}

// count chars until \0 or space or "to long"
int len(char * str) {
    int ddorf=0;
    while (str[ddorf] != '\0' && str[ddorf] != ' ' && ddorf != 40225) ++ddorf;
    return ddorf;
}

// read a positive number from a char array
int str2num(char * str) {
    int result = 0;
    int b = 1;
    int l = len(str);
    int i;
    for(i=1; i<l; ++i) b *= 10;
    for(i=0; i<l; ++i) {
        result += b * (int)(str[i] - '0');
        b /= 10;
    }
    return result;
}

int strIsNum(char * str){
    int l= len(str);
    int i;
    for(i=0; i<l; i++){
        if(!isdigit(str[i])) return 0;
    }
    return 1;
}



void toRGB(int id, char * blueGreenRed) {
    blueGreenRed[0] = 0;
    blueGreenRed[1] = 0;
    blueGreenRed[2] = 0;
    if ( id == colorLimit ) return;

    float hi,q,t,coeff;

    coeff = 7.0 * (id/(float)colorLimit);
    hi = intFloor(coeff);
    t = coeff - hi;
    q = 1 - t;
    if (hi == 0.0) {
        blueGreenRed[2] = 0;
        blueGreenRed[1] = t*255; //immer mehr green und blau -> dunkelblau zu cyan
        blueGreenRed[0] = t*127 + 128;
    } else if (hi == 1.0) {
        blueGreenRed[2] = t*255; //immer mehr rot -> cyan zu weiss
        blueGreenRed[1] = 255;
        blueGreenRed[0] = 255;
    } else if (hi == 2.0) {
        blueGreenRed[2] = 255;
        blueGreenRed[1] = 255;
        blueGreenRed[0] = q*255; // immer weniger blau -> weiss zu gelb
    } else if (hi == 3.0) {
        blueGreenRed[2] = 255;
        blueGreenRed[1] = q*127 + 128; // immer weniger green -> gelb zu orange
        blueGreenRed[0] = 0;
    } else if (hi == 4.0) {
        blueGreenRed[2] = q*127 + 128; // orange wird dunkler -> orange zu braun
        blueGreenRed[1] = q*63 + 64;
        blueGreenRed[0] = 0;
    } else if (hi == 5.0) {
        blueGreenRed[2] = 128;
        blueGreenRed[1] = 64;
        blueGreenRed[0] = t*128; // mehr blau -> braun zu violett
    } else if (hi == 6.0) {
        blueGreenRed[2] = q*128; // weniger rot und green -> violett wird dunkelblau
        blueGreenRed[1] = q*64;
        blueGreenRed[0] = 128;
    }
}

void  calculateSinglePoint(int x, int y, int width, int height,float imageRelation, char iterate, char * blueGreenRed,struct Complex_s c, struct Complex_s newz){
    Complex z = {0,0};
    float quad=0;

    c.re = zoom * (-1.0 + imageRelation * ( (x-1.0) / (width-1.0)) );
    c.im = zoom * ( 0.5 - (y-1.0) / (height-1.0) );

    // iterate
    for ( iterate=1; iterate < colorLimit && quad < quadLimit; ++iterate ) {
        quad = z.re * z.re + z.im * z.im;

        newz.re = (z.re * z.re) - (z.im * z.im) + c.re;
        newz.im =  z.re * z.im * 2.0            + c.im;

        z = newz;
    }

    toRGB(iterate, &blueGreenRed[(y-1)*width*3+((x-1)*3)]);
}
void *calculateLines(void* arg){
    int x,y;
    struct Thread_args *trarg=(struct Thread_args*) arg;
    for (y=(*trarg).try; y <= (*trarg).end; ++y) {
        char iterate=0;
        Complex c    = {0,0};
        Complex newz = {0,0};
        for (x=1; x <= (*trarg).width; ++x) {

            calculateSinglePoint(x,y,(*trarg).width,(*trarg).height,(*trarg).relation,iterate,(*trarg).blueGreenRed,c,newz);
        }
    }
}

int main(int argc, char ** argv, char ** envp) {

    if (argc < 3) {
        fprintf(stderr,
                "Wrong amount of Arguments:\n USAGE: ./mandel WIDTH HEIGHT AMOUNTOFTHREADS\nFor useful result pipe output like:\n ./mandel WIDTH HEIGHT AMOUNTOFTHREADS> output.bmp\nOmmitting AMOUNTOFTHREADS will start in Single-Thread Mode.\n");
        return 1;
    }
    if (!strIsNum(argv[1]) || !strIsNum(argv[2])) {
        fprintf(stderr,
                "First 3 Arguments need to be numbers:\n USAGE: ./mandel WIDTH HEIGHT\nFor useful result pipe output like:\n ./mandel 1200 800 2 > output.bmp\n");
        return 1;
    }

    int threadNum=1;

    if(argc>3){
        if(!strIsNum(argv[3])) {
            fprintf(stderr, "Thread Parameter wrong or missing?, for multithreading call the programm like this: ./mandel WIDTH HEIGHT AMOUNTOFTHREADS >output.bmp\nProceeding with single Thread!\n");
        } else {
            threadNum=str2num(argv[3]);
        }
    } else {
        fprintf(stderr, "Thread Parameter wrong or missing?, for multithreading call the programm like this: ./mandel WIDTH HEIGHT AMOUNTOFTHREADS >output.bmp\nProceeding with single Thread!\n");
    }


    int width  = str2num(argv[1]);
    int height = str2num(argv[2]);

    float imageRelation = (float)width/(float)height;
    int y,x;
    char* blueGreenRed=malloc(sizeof(char)*height*width*3);

    unsigned char info[BMPHEADER_SIZE] = {
            //size
            'B','M',  0,0,0,0, 0,0, 0,0, 54,0,0,0,
            //width  //height
            40,0,0,0, 0,0,0,0, 0,0,0,0,  1,0, 24,0,
            // datasize
            0,0,0,0,  0,0,0,0
    };

    // BMP lines must be of lengths divisible by 4
    char span[4] = "\0\0\0\0";
    int spanBytes = 4 - ((width * 3) % 4);
    if (spanBytes == 4) spanBytes = 0;
    int psize = ((width * 3) + spanBytes) * height;

    *( (int*) &info[2])  = BMPHEADER_SIZE + psize;
    *( (int*) &info[18]) = width;
    *( (int*) &info[22]) = height;
    *( (int*) &info[34]) = psize;

    write(1, (char *) info, BMPHEADER_SIZE);

    fprintf(stderr, "Starting calculations\n");
    struct timeval start,ende;
    gettimeofday(&start,NULL);
    int cnt;
    int linesPerThread=(height/threadNum +1); // +1 to make sure no line is ommited when height/threadNum is rounded
    pthread_t threads[threadNum];
	struct Thread_args single[threadNum];
    for(cnt=1;cnt<=threadNum;++cnt) {
	//Check if endline>height.

        	single[cnt-1].try = 1 + linesPerThread * (cnt - 1);
		single[cnt-1].height=height;
		single[cnt-1].width=width;
		 if(1+linesPerThread*cnt<height)
		single[cnt-1].end=(1 + linesPerThread * cnt);
		else
				single[cnt-1].end=height;
		single[cnt-1].relation=imageRelation;
		single[cnt-1].blueGreenRed=blueGreenRed;
            if(pthread_create(&threads[cnt-1],NULL,calculateLines, &single[cnt-1])){
                fprintf(stderr, "Error creating thread\n");
                    free(blueGreenRed);
                return 1;
            }else{
                //fprintf(stderr,"Calculating Line %d to %d\n",(1+linesPerThread*(cnt-1)),single[cnt-1].end);
            }

    }
    for(cnt=1;cnt<=threadNum;++cnt){
        if(pthread_join(threads[cnt-1],NULL)){
            fprintf(stderr,"Error joining threads\n");
                free(blueGreenRed);
            return 2;
        } else {
            //fprintf(stderr,"Getting Thread %d \n",cnt);
        }
    }

    for(y=1;y<=height;++y){
        write(1, &blueGreenRed[(y-1)*width*3], width*3);
        // BMP lines must be of lengths divisible by 4
        write(1, span, spanBytes);
    }
    gettimeofday(&ende,NULL);
    fprintf(stderr,"Mandelbrot-Set generated in %ld microseconds\n",((ende.tv_sec*1000000+ende.tv_usec) - (start.tv_sec*1000000+start.tv_usec)));
    free(blueGreenRed);
    return 0;
}


