#include <cmath>
#include <QTextBrowser>
#include <iostream>
#include <sstream>
#include <QTime>
#include <QTimer>
#include <QCoreApplication>

int overcount=0;//how many times the first loop exceeds e+100
int* ptr= &overcount;

long double powBino(long double b, long int exp)
{
    long double res=1;

    for(long int i=0;i<exp;i++){
        res*=pow(b,1);
        if(res<=pow(10,-100)&&*(ptr)>=1){
            res*=pow(10,100);
            --*(ptr);
        }
    }
    return res;
}

long double nCr(int n, int k){
    long double res=1;

    if(n==k or k==0) return 1;
    if(k==1 or n-k==1) return n;
    if(k>(n/2)) k=n-k;
    for(int i=1;i<=k;++i){
        res*=n;
        n--;
        if(res>=pow(10,100)) {
            res/=pow(10,100);
            ++*(ptr);
        }
    }

    for(int j=k;j!=0;--j){
        res/=j;
        if(res<=pow(10,100)&&*(ptr)>=1) {
            res*=pow(10,100);
            --*(ptr);
        }
    }

    return res;
}

double binompdf(int n, double p, int k){
    if(n<0 or p<0 or p>1 or k<0 or k>n){
        return -1;
    }
    long double res=1;
    res*=nCr(n,k);
    res*=powBino(p,k);
    res*=powBino(1-p,n-k);
    
    return res;
}

double binomcdf(int n, double p, int k1, int k2){
    long double res=0;
    if(k1>k2 or k2>n) return -1;
    for(int i=k1;i<=k2;i++){
        res+=binompdf(n,p,i);
    }
    return res;
}

double efficient_binomcdf(int n, double p, int k1, int k2){
    long double res=0;
    if(k1>k2 or k2>n) return -1;
    for(int i=k1;i<=k2;i++){
        res+=binompdf(n,p,i);
    }
    return res;
}

int fromNandP(int n, double p, double& mu, double& sigma){
    if(n*p<0 or n*p>n or sqrt(n*p*(1-p))==NAN) return -1;
    mu=n*p;
    sigma= sqrt(n*p*(1-p));
    return 0;
}

int fromMuandSigma(double mu, double sigma, int& n, double& p){
    if((1-((sigma*sigma)/mu))<0) return -1;
    p=1-((sigma*sigma)/mu);
    n=lround(mu/p);
    return 0;
}

/////   ----- Code for WF -----
typedef struct wf{ // Type for WF-Calculations
    int n=0,k=0;
    long long int overflow=0;
    int shitsPivoted=0,negative=0,achtung=0;
    long double res=1;

} wf;

//Global Structures for WF
wf* powtmp= new wf;
wf* nprtmp= new wf;
wf* ncrtmp= new wf;

void getNice(wf* obj){
    if(!obj->n or !obj->k)return;
    if(obj->overflow>pow(10,6)){
        obj->shitsPivoted++;
        return;
    }
    if(obj->res<1){
        while(obj->res<1){
            obj->res*=10;
            obj->overflow--;
        }
    }else if(obj->res>999 && obj->overflow>5){
        while(obj->res>999){
            obj->res/=10;
            obj->overflow++;
        }
    }
}
void wf_factorial(wf* obj){
    if(obj->n==0 or obj->n==1){ obj->res=1; return;}
    if(obj->n<0 or obj->n>pow(10,8)) {obj->shitsPivoted=1; return;}
    if(obj->n<15){
        for(int i=1;i<=obj->n;++i){
            obj->res*=i;
        }
        return;
    }
    for(int i=1;i<=obj->n;++i){
        obj->res*=i;
        if(obj->res>pow(10,10)){
            obj->res*=(double)pow(10,-10);
            obj->overflow++;
        }
    }
    obj->overflow*=10;

    while(obj->res>1000){
        obj->res*=pow(10,-1);
        obj->overflow++;
    }
}


void wf_pow(wf* obj){
    if(obj->n*obj->k>pow(10,9)){
        obj->shitsPivoted++;
        return;
    }
    if(obj->k>=0){
        int mul=5;
        if(obj->n>2000) mul=10;
    for(int i=obj->k;i>0;i--){
        obj->res*=obj->n;
        if(obj->res>pow(10,mul)){
            obj->overflow++;
            obj->res*=pow(10,-mul);
        }
    }
    obj->overflow*=mul;
    }else{

    if(obj->k>-5){//needs improvment
        obj->res=pow(obj->n,obj->k);
        getNice(obj);
        return;
    }
    obj->k=-obj->k;
    obj->negative=1;
    wf_pow(obj);
    obj->res=(double)1/obj->res;
    obj->overflow=-obj->overflow;
    }
    getNice(obj);
}


void wf_ncr(wf* obj){
    long double res=1;
    int n=obj->n;
    int k=obj->k;

    long long int* ptr=&(obj->overflow);
    int booly= ((double)obj->n/obj->k>1.7 && obj->n*obj->k>pow(10,9)) or obj->n*obj->k>pow(10,10);
    if(k>n or n<0 or k<0 or booly){
        obj->shitsPivoted=1; return;
    }
    if(n==k or k==0){
        obj->res=1; return;
    }
    if(k==1 or n-k==1){
        obj->res=n; return;
    }
    if(k>(n/2)) k=n-k;

    for(int i=1;i<=k;++i){
        res*=n;
        n--;
        if(res>=pow(10,5)) {
            res/=pow(10,5);
            ++*(ptr);
        }
    }

    for(int j=k;j!=0;--j){
        res/=j;
        if(res<=pow(10,5)&&*(ptr)>=1) {
            res*=pow(10,5);
            --*(ptr);
        }
    }
    obj->res=res;
    obj->overflow*=5;
    getNice(obj);
}

void wf_npr(wf* obj){
    wf_ncr(obj);
    if(obj->shitsPivoted)return;
    int tmpover=0;
    for(int i=1;i<=obj->k;i++){
        obj->res*=i;
        if(obj->res>=pow(10,5)) {
            obj->res/=pow(10,5);
                ++tmpover;
            }
    }
    obj->overflow+=(tmpover*5);
    getNice(obj);
}
///----- end of block --------
int perfectspacing(int n){
    int i=n;
    int res[200];
    int arrcount=0;
    int result=0;
    while(i){
        if(((double)n/(double)i)==(n/i)){
            res[arrcount]=i;
            arrcount++;
        }
        i--;
    }

    for(int j=arrcount-1;j>=0;j--){
        if(res[j]<21)result = res[j];
    }
    return result>3?result:5;
}

/// ---- <<< Code for PRK >>> ----
class PRK {
public:
    int n=0,k=0,k1=0,k2=0;
    double p=0,pRes=0;
    int cmpstatus=0; //0 <==> ≥ ; 1 <==> ≤
    int warnung=0;
    void resetprk(){
            n=p=k=k1=k2=pRes=0;
    }
    double getOtherLimit(int cumulative, char missing, double firstLimit){
        if(!cmpstatus&&firstLimit!=-1){
            switch(missing){
            case 'n':
            {
                if(!cumulative){
                    for(int i=firstLimit;i<=10000;i++){
                        if(binompdf(i,p,k)<=pRes){
                            return i-1;
                        }
                    }
                }else{
                    for(int i=firstLimit;i<=5000;i++){
                        if(binomcdf(i,p,k1,k2)<=pRes){
                            return i-1;
                        }
                    }
                }
                break;
            }
            case 'p':
            {
                if(!cumulative){
                    for(int i=firstLimit*10000;i<=10000;i++){
                        if(binompdf(n,i*0.0001,k)<=pRes){
                            return (i-1)*0.0001;
                        }
                    }
                }else{
                    for(int i=firstLimit*10000;i<=10000;i++){
                        if(binomcdf(n,i*0.0001,k1,k2)<=pRes){
                            return (i-1)*0.0001;
                        }
                    }
                }
                break;
            }
            }
        }
        return -1;
    }
    int nMissing(int cumulative){
        if(n==-1 or p==-1 or k==-1 or k1==-1 or k2==-1 or pRes==-1) return -1;
            if(!cumulative){
                for(int i=0;i<=10000;i++){
                    if(pRes>binompdf(i,p,i*p)) return -1;
                    if(binompdf(i,p,k)>=pRes){
                        return n=cmpstatus?i-1:i;
                    }
                }
            }else{
                if(k2==1010101010&&pRes>0){//for input "n"
                    for(int i=k1;i<=5000;i++){
                        if(binomcdf(i,p,k1,i)>=pRes){
                            return n=cmpstatus?(i==k2?i:i-1):i;
                        }
                    }
                }
                for(int i=k1;i<=5000;i++){
                    if(binomcdf(i,p,k1,k2)>=pRes){
                        return n=cmpstatus?(i==k2?i:i-1):i;
                    }
                }
            }
            return -1;
    }
    double pMissing(int cumulative){
            if(n==-1 or p==-1 or k==-1 or k1==-1 or k2==-1 or pRes<=0) return -1;
            if(!cumulative){
                if(k){
                for(int i=0;i<=10000;i++){
                    if(binompdf(n,i*0.0001,k)>=pRes){
                            return p=(cmpstatus?i-1:i)*0.0001;//>=
                    }
                }
                }else{
                for(int i=10000;i;i--){
                    if(binompdf(n,i*0.0001,k)>=pRes){
                        return p=(cmpstatus?i+1:i)*0.0001;
                    }
                }
                }
            }else{
                if(k1){
                for(int i=0;i<=10000;i++){
                    if(binomcdf(n,i*0.0001,k1,k2)>=pRes){
                        return p=(cmpstatus?i-1:i)*0.0001;
                    }
                }
                }else{
                for(int i=10000;i;i--){
                    if(binomcdf(n,i*0.0001,k1,k2)>=pRes){
                        return p=(cmpstatus?i+1:i)*0.0001;
                    }
                }
                }
            }
            return -1;
    }
    int kMissing(int cumulative){
            if(n==-1 or p==-1 or k==-1 or k1==-1 or k2==-1 or pRes==-1) return -1;
            if(!cumulative){
                for(int i=0;i<=n;i++){
                if(binompdf(n,p,i)>=pRes){
                    return k=cmpstatus?i-1:i;
                }
                }
            }
            return -1;
    }
    int k1Missing(int cumulative){
            if(n==-1 or p==-1 or k==-1 or k1==-1 or k2==-1 or pRes==-1) return -1;
            if(cumulative){
                for(int i=k2;i;i--){   
                if(binomcdf(n,p,i,k2)>=pRes){
                    return k1=cmpstatus?(i==n?i:i+1):i;
                }
                }
            }
            return -1;
    }
    int k2Missing(int cumulative){
            if(n==-1 or p==-1 or k==-1 or k1==-1 or k2==-1 or pRes==-1) return -1;
            if(cumulative){
                for(int i=k1;i<=n;i++){
                if(binomcdf(n,p,k1,i)>=pRes){
                    return k2=cmpstatus?(i==n?i:i-1):i;
                }
                }
            }
            return -1;
    }
};

PRK* prkObj = new PRK;

/*
void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(3);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
*/
inline void delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}
/*
void welcomeDisplay(QTextBrowser& x){
    int i=25;
    while(i){

        x.setHtml(welcome1);
        delay(3000);
        x.setHtml(welcome2);
        delay(3000);
        i--;
    }

}
*/
