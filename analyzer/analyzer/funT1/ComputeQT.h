#pragma once

#include "IntegralT.h"
#include "SeperateT.h"
#include <functional>




template <typename T>
bool RDPOnce(std::vector<T> x, std::vector<T> y, size_t &newnode, T &dist)
{

	if(	x.size()!=y.size() || x.size()<3 ){
		return false;
	}

	T xf=x.front();
	T xb=x.back();
	T yf=y.front();
	T yb=y.back();

	T D=xf*yb-xb*yf;
	T A=yf-yb;
	T B=xb-xf;

	std::vector<T> d(x.size(),0);
	for(size_t i=0;i<d.size();i++){
		d[i]=A*x[i]+B*y[i]+D;
		d[i]=abs(d[i]);
	}
	std::vector<T>::iterator it;
	it=std::max_element(d.begin(),d.end());
	newnode=it-d.begin();

	dist=d[newnode]/sqrt(A*A+B*B);

	return true;

}

template <typename T>
bool findUpLim(
	const std::vector<T> &u, 
	const std::vector<T> &i,
	size_t startidx,
	size_t endidx,
	size_t &UpLimIdx
	){
		std::vector<T> x;
		x.assign(u.begin()+startidx,u.begin()+endidx+1);
		std::vector<T> y;
		y.assign(i.begin()+startidx,i.begin()+endidx+1);
		T dist=0;
		size_t idx=0;
		bool flg=RDPOnce(x,y,idx,dist);

		if(flg){

			TRACE(L"\nu=%g,i=%g",x[idx],y[idx]);

			if(y[idx]<=0){
				UpLimIdx=idx+startidx;				
			}
			else{
				x.erase(x.begin(),x.begin()+idx);
				y.erase(y.begin(),y.begin()+idx);
				startidx+=idx;

				flg=RDPOnce(x,y,idx,dist);

				UpLimIdx=flg? idx+startidx : startidx;
			}

			TRACE(L";u=%g,i=%g",u[UpLimIdx],i[UpLimIdx]);

			return true;
		}

		return false;

}





template <typename T>
UINT ComputeQListC(
	std::vector<T> u, 
	std::vector<T> i,
	std::vector<T> &QList, 
	T scanRate,
	T umin,
	T umax
	){

		if(u.size()!=i.size()
			//||nCycle==0
			////||u.size()<nCycle*nPperCycle
			){
				return 3;//输入错误
		}

		std::vector<size_t> mini;
		std::vector<size_t> maxi;

		UINT re=Seperate(u,mini,maxi);

		if(re!=0){
			return re;//分段出错
		}

		if(mini.front()>maxi.front())
			maxi.erase(maxi.begin());

		QList.clear();

		while( !mini.empty() && !maxi.empty() ){

			if(u[mini.front()]<=umin
				&& u[maxi.front()]>=umax
				){//分段区间包含预设搜索范围[umin,umax]
					std::vector<T>::iterator it;					
					it=std::max_element(i.begin()+mini.front(),i.begin()+maxi.front()+1);
					if(it!=i.begin()+maxi.front()+1){
						size_t maxidx=it-i.begin();

						size_t uplimidx=0;

						if(findUpLim(u,i,maxidx,maxi.front(),uplimidx)){
							maxi.front()=uplimidx;
							T ilowlim=0;
							it=std::find_end(
								i.begin()+mini.front(),								
								i.begin()+maxidx+1, 									
								&ilowlim, 
								(&ilowlim)+1, 
								std::less_equal<T>());


							if(it!=i.begin()+maxidx+1){//积分下限超出分段区间
								mini.front()=it-i.begin();


								if(mini.front()<maxi.front()){//积分下限<积分上限
									std::vector<T> x(u.begin()+mini.front(),u.begin()+maxi.front()+1);
									std::vector<T> y(i.begin()+mini.front(),i.begin()+maxi.front()+1);

									T res=Integral(x,y)/scanRate;

									QList.push_back(res);
								}
							}
						}
					}
			}

			mini.erase(mini.begin());
			maxi.erase(maxi.begin());

		}

		return 0;

}





template <typename T>
UINT ComputeQListB(
	std::vector<T> u, 
	std::vector<T> i,
	std::vector<T> &QList, 
	T lowLimit,
	T scanRate,
	T umin,
	T umax
	){

		if(u.size()!=i.size()
			//||nCycle==0
			////||u.size()<nCycle*nPperCycle
			){
				return 3;//输入错误
		}

		std::vector<size_t> mini;
		std::vector<size_t> maxi;

		UINT re=Seperate(u,mini,maxi);

		if(re!=0){
			return re;//分段出错
		}

		if(mini.front()>maxi.front())
			maxi.erase(maxi.begin());

		QList.clear();

		while( !mini.empty() && !maxi.empty() ){

			if(u[mini.front()]<=umin
				&& u[maxi.front()]>=umax
				){//分段区间包含预设搜索范围[umin,umax]

					if(u[mini.front()]<=lowLimit){
						std::vector<T>::iterator it;
						it=std::find_first_of(
							u.begin()+mini.front(), 
							u.begin()+maxi.front()+1, 
							&lowLimit, 
							(&lowLimit)+1, 
							std::greater_equal<T>());

						if(it!=u.begin()+maxi.front()+1){//积分下限未超出分段区间
							mini.front()=it-u.begin();
							it=std::max_element(i.begin()+mini.front(),i.begin()+maxi.front()+1);

							if(it!=i.begin()+maxi.front()+1){
								size_t maxidx=it-i.begin();

								size_t uplimidx=0;

								if(findUpLim(u,i,maxidx,maxi.front(),uplimidx)){
									maxi.front()=uplimidx;

									if(mini.front()<maxi.front()){//积分下限<积分上限
										std::vector<T> x(u.begin()+mini.front(),u.begin()+maxi.front()+1);
										std::vector<T> y(i.begin()+mini.front(),i.begin()+maxi.front()+1);

										if(mini.front()+1<u.size() && u[mini.front()]<lowLimit){
											T lowY=((i[mini.front()+1]-i[mini.front()])*lowLimit+u[mini.front()+1]*i[mini.front()]-u[mini.front()]*i[mini.front()+1])/(u[mini.front()+1]-u[mini.front()]);
											x.front()=(lowLimit);
											y.front()=(lowY);
										}


										T res=Integral(x,y)/scanRate;

										QList.push_back(res);
									}
								}
							}
						}
					}
			}

			mini.erase(mini.begin());
			maxi.erase(maxi.begin());

		}

		return 0;

}



template <typename T>
UINT ComputeQList(
	std::vector<T> u, 
	std::vector<T> i,
	std::vector<T> &QList, 
	T upLimit,
	T scanRate,
	T umin,
	T umax
	){

		if(u.size()!=i.size()
			//||nCycle==0
			////||u.size()<nCycle*nPperCycle
			){
				return 3;//输入错误
		}

		std::vector<size_t> mini;
		std::vector<size_t> maxi;

		UINT re=Seperate(u,mini,maxi);

		if(re!=0){
			return re;//分段出错
		}

		if(mini.front()>maxi.front())
			maxi.erase(maxi.begin());

		QList.clear();

		while( !mini.empty() && !maxi.empty() ){

			if(u[mini.front()]<=umin
				&& u[maxi.front()]>=umax
				){//分段区间包含预设搜索范围[umin,umax]

					if(u[maxi.front()]>=upLimit){
						std::vector<T>::iterator it;
						it=std::find_end(
							u.begin()+mini.front(), 
							u.begin()+maxi.front()+1, 
							&upLimit, 
							(&upLimit)+1, 
							std::less_equal<T>());

						if(it!=u.begin()+maxi.front()+1){//积分上限未超出分段区间
							maxi.front()=it-u.begin();
							it=std::max_element(i.begin()+mini.front(),i.begin()+maxi.front()+1);
							if(it!=i.begin()+maxi.front()+1){
								size_t maxidx=it-i.begin();


								T ilowlim=0;
								it=std::find_end(
									i.begin()+mini.front(),
									i.begin()+maxidx+1, 
									&ilowlim, 
									(&ilowlim)+1, 
									std::less_equal<T>());

								if(it!=i.begin()+maxidx+1){//积分下限超出分段区间
									mini.front()=it-i.begin();
									if(mini.front()<maxi.front()){//积分下限<积分上限
										std::vector<T> x(u.begin()+mini.front(),u.begin()+maxi.front()+1);
										std::vector<T> y(i.begin()+mini.front(),i.begin()+maxi.front()+1);


										if(maxi.front()+1<u.size() && u[maxi.front()]<upLimit){
											T upY=((i[maxi.front()+1]-i[maxi.front()])*upLimit+u[maxi.front()+1]*i[maxi.front()]-u[maxi.front()]*i[maxi.front()+1])/(u[maxi.front()+1]-u[maxi.front()]);
											x.push_back(upLimit);
											y.push_back(upY);
										}


										T res=Integral(x,y)/scanRate;

										QList.push_back(res);
									}
								}
							}
						}
					}
			}

			mini.erase(mini.begin());
			maxi.erase(maxi.begin());

		}

		return 0;

}




template <typename T>
UINT ComputeQListA(
	std::vector<T> u, 
	std::vector<T> i,
	std::vector<T> &QList, 
	T upLimit,
	T lowLimit,
	T scanRate
	){

		if(u.size()!=i.size()
			//||nCycle==0
			////||u.size()<nCycle*nPperCycle
			){
				return 3;//输入错误
		}

		std::vector<size_t> mini;
		std::vector<size_t> maxi;

		UINT re=Seperate(u,mini,maxi);

		if(re!=0){
			return re;//分段出错
		}

		if(mini.front()>maxi.front())
			maxi.erase(maxi.begin());

		QList.clear();

		while( !mini.empty() && !maxi.empty() ){

			if(u[maxi.front()]>=upLimit){
				std::vector<T>::iterator it;
				it=std::find_end(
					u.begin()+mini.front(), 
					u.begin()+maxi.front()+1,
					&upLimit, 
					(&upLimit)+1,
					std::less_equal<T>());
				if(it!=u.begin()+maxi.front()+1){//积分上限未超出分段区间
					maxi.front()=it-u.begin();

					if(u[mini.front()]<=lowLimit){
						it=std::find_end(
							u.begin()+mini.front(),
							u.begin()+maxi.front()+1, 
							&lowLimit, 
							(&lowLimit)+1, 
							std::less_equal<T>());
						if(it!=u.begin()+maxi.front()+1){//积分下限未超出分段区间
							mini.front()=it-u.begin();

							if(mini.front()<maxi.front()){//积分下限<积分上限
								std::vector<T> x(u.begin()+mini.front(),u.begin()+maxi.front()+1);
								std::vector<T> y(i.begin()+mini.front(),i.begin()+maxi.front()+1);

								if(maxi.front()+1<u.size() && u[maxi.front()]<upLimit){
									T upY=((i[maxi.front()+1]-i[maxi.front()])*upLimit+u[maxi.front()+1]*i[maxi.front()]-u[maxi.front()]*i[maxi.front()+1])/(u[maxi.front()+1]-u[maxi.front()]);
									x.push_back(upLimit);
									y.push_back(upY);
								}

								if(mini.front()+1<u.size() && u[mini.front()]<lowLimit){
									T lowY=((i[mini.front()+1]-i[mini.front()])*lowLimit+u[mini.front()+1]*i[mini.front()]-u[mini.front()]*i[mini.front()+1])/(u[mini.front()+1]-u[mini.front()]);
									x.front()=(lowLimit);
									y.front()=(lowY);
								}

								T res=Integral(x,y)/scanRate;
								QList.push_back(res);
							}
						}
					}
				}
			}
			mini.erase(mini.begin());
			maxi.erase(maxi.begin());

		}



		return 0;
}





//template <typename T>
//UINT ComputeQList(
//	const std::vector<T> &u, 
//	const std::vector<T> &i,
//	T* QList, 
//	size_t nCycle,
//	T upLimit,
//	T scanRate,
//	T umin,
//	T umax,
//	size_t nPperCycle=4)
//{
//
//	size_t ci=0;
//
//	if(u.size()!=i.size()
//		||nCycle==0
//		||u.size()<nCycle*nPperCycle){
//			return ci;//输入错误
//	}
//
//	std::vector<size_t> mini;
//	std::vector<size_t> maxi;
//
//	int re=Seperate(u,mini,maxi);
//
//	if(re!=0){
//		return ci;//分段出错
//	}
//
//	std::vector<T> intgi(i.size(),0);
//
//	if(mini.front()>maxi.front())
//		maxi.erase(maxi.begin());
//
//
//
//	while(ci<nCycle){
//
//		if(mini.empty() || maxi.empty())
//			return ci;//数据不完整
//
//		if(u[mini.front()]>=upLimit
//			|| u[mini.front()]>umin
//			|| u[maxi.front()]<umax
//			|| u[maxi.front()]<=upLimit){
//				mini.erase(mini.begin());
//				maxi.erase(maxi.begin());
//				continue;//分段区间过短，不能包含[umin,umax]，或积分上限超出分段区间
//		}
//
//		//////////////////////////////////找积分下限/////////////////////////////////
//		auto result = std::minmax_element(i.begin()+mini.front(),i.begin()+maxi.front()+1);
//		size_t maxidx=result.second-i.begin();
//
//		if(*result.second<=0)
//			continue;//区间值域为负
//
//		size_t minidx=maxidx;
//		for(;minidx>mini.front();minidx--){
//			if(i[minidx]>0 && i[minidx-1]<=0)
//				break;//找积分下限
//		}
//
//		if(u[minidx]>=upLimit)
//			continue;//积分下限大于积分上限
//		///////////////////////////////////////////////
//		QList[ci]=0;
//
//		for(size_t j=minidx;j<maxi.front();j++){
//			if(u[j]<upLimit){
//				QList[ci]+=(i[j]+i[j+1])*(u[j+1]-u[j]);
//			}
//			else{
//				break;
//			}
//		}
//
//		//bool bStart=false;
//		//for(size_t j=maxi.front();j>mini.front();j--){			
//		//	if(bStart){
//		//		QList[ci]+=(i[j]+i[j+1])*(u[j+1]-u[j]);
//		//		//TRACE(L"\n%g,%g",u[j],i[j]);
//		//		if(i[j-1]<0)
//		//			break;
//		//	}
//		//	else{
//		//		bStart|=(u[j]<=upLimit);
//		//	}
//		//}
//
//		QList[ci]/=scanRate*2;
//
//		ci++;
//		mini.erase(mini.begin());
//		maxi.erase(maxi.begin());
//
//	}
//
//	return ci;
//}
//
//
//
//template <typename T>
//UINT ComputeQListA(
//	const std::vector<T> &u, 
//	const std::vector<T> &i,
//	T* QList, 
//	size_t nCycle,
//	T upLimit,
//	T lowLimit,
//	T scanRate,
//	//T umin,
//	//T umax,
//	size_t nPperCycle=4)
//{
//
//	size_t ci=0;
//
//	if(u.size()!=i.size()
//		||nCycle==0
//		||u.size()<nCycle*nPperCycle){
//			return ci;//输入错误
//	}
//
//	std::vector<size_t> mini;
//	std::vector<size_t> maxi;
//
//	int re=Seperate(u,mini,maxi);
//
//	if(re!=0){
//		return ci;//分段出错
//	}
//
//	std::vector<T> intgi(i.size(),0);
//
//	if(mini.front()>maxi.front())
//		maxi.erase(maxi.begin());
//
//
//
//	while(ci<nCycle){
//
//		if(mini.empty() || maxi.empty())
//			return ci;//数据不完整
//
//		if(u[mini.front()]>=lowLimit
//			//|| u[mini.front()]>umin
//			//|| u[maxi.front()]<umax
//			|| u[maxi.front()]<=upLimit){
//				mini.erase(mini.begin());
//				maxi.erase(maxi.begin());
//				continue;//分段区间过短，不能包含[umin,umax]，或积分上下限超出分段区间
//		}
//
//		QList[ci]=0;
//
//		for(size_t j=mini.front();j<maxi.front();j++){
//			if(u[j]<upLimit && u[j]>lowLimit){
//				QList[ci]+=(i[j]+i[j+1])*(u[j+1]-u[j]);
//			}
//			//else{
//			//break;
//			//}
//		}
//
//		QList[ci]/=scanRate*2;
//
//		ci++;
//		mini.erase(mini.begin());
//		maxi.erase(maxi.begin());
//
//	}
//
//	return ci;
//}
