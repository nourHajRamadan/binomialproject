//#include <iostream>
#include <cmath>

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

int fromNandP(int n, double p, double& mu, double& sigma){
    if(n*p<0 or sqrt(n*p*(1-p))==NAN) return -1;
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
/*
main(){
std::cout<<"binomcdf:"<<binomcdf(9376,0.982,9100,9100)<<std::endl;
std::cout<<"binompdf:"<<binompdf(9376,0.982,9100)<<std::endl;
std::cout<<"overcount:"<<overcount<<std::endl;
}
*/
