#pragma once

#include <vector>
#include "sapitemA.hpp"
// DataOutA command target
#include "../resource.h"
#include <numeric>


template <typename T>
T Sum(std::vector<T> x, T init=0)
{
	return std::accumulate(x.begin(),x.end(),init);
}

template <typename T>
T Mean(std::vector<T> x)
{
	if(x.empty())
		return 0;
	return Sum(x)/((T)(x.size()));
}

template <typename T>
T Variance(std::vector<T> x, bool bSample=true)
{
	T n=x.size();
	if(x.size()<2)
		return 0;
	T mx=Mean(x);
	mx=-mx*mx*n;
	T v=std::inner_product(x.begin(),x.end(),x.begin(),mx);
	if(bSample){
		v/=n-1;
	}
	else{
		v/=n;
	}
	return v;
}

template <typename T>
T VarianceStd(std::vector<T> x, bool bSample=true)
{
	return sqrt(Variance(x,bSample));
}

//one step filter BYTE 
#define DOA_VMS 0x01
#define DOA_S 0x02
#define DOA_A 0x04
#define DOA_L 0x08
#define DOA_MORE 0x10
#define DOA_SAMPLE 0x20
//#define DOA_RECOUNT 0x40
#define DOA_RESET_SOLUTION_AT_END 0x80

//one plot filter byte
#define PF_Q_NORMALIZED 0x04
#define PF_CONCERTRATION 0x08
#define PF_S 0x10
#define PF_A 0x20
#define PF_L 0x40
#define PF_SAMPLE 0x80

///
/// \brief The DataOutA class
///溶液状态参数
///
class DataOutA : public ObjectF
{
public:
    ///
    /// \brief Ar
    ///电量
	std::vector<double> Ar;
    ///
    /// \brief Ar0
    ///基准电量
	double Ar0;
    ///
    /// \brief Sml
    ///抑制剂的量
    ///
	double Sml;
    ///
    /// \brief Aml
    ///加速剂的量
	double Aml;
    ///
    /// \brief Lml
    ///整平剂的量
	double Lml;
    ///
    /// \brief bUnknown
    ///溶液成分是否未知
	bool bUnknown;
    ///
    /// \brief VMSVolume
    ///基液体积
	double VMSVolume;
    ///
    /// \brief additiveVolume
    ///添加剂体积
	double additiveVolume;
    ///
    /// \brief addVolume
    ///最后一次加液体积
	double addVolume;
    ///
    /// \brief stepName
    ///步骤名
	CString stepName;
    ///
    /// \brief stepFilter
    ///此步骤对加液参数的限制条件
	BYTE stepFilter;
    ///
    /// \brief UseIndex
    ///最后选用的电量值的序号
	int UseIndex;

public:
	DataOutA()
		: stepName(_T(""))
		, Ar0(0)
		, Sml(0)
		, Aml(0)
		, Lml(0)
		, VMSVolume(0)
		, additiveVolume(0)
		, addVolume(0)
		, bUnknown(false)
		, stepFilter(0)
		, UseIndex(-1)
	{
	};

	virtual ~DataOutA(){};

	void operator=(const DataOutA &src){
		stepName=src.stepName;
		Ar.assign(src.Ar.begin(),src.Ar.end());;
		Ar0=src.Ar0;
		Sml=src.Sml;
		Aml=src.Aml;
		Lml=src.Lml;
		bUnknown=src.bUnknown;
		VMSVolume=src.VMSVolume;
		additiveVolume=src.additiveVolume;
		addVolume=src.addVolume;	
		stepFilter=src.stepFilter;
		UseIndex=src.UseIndex;
	};

	DataOutA(const DataOutA &src){ operator=(src); };

	virtual void Serialize(CArchive& ar){
		if (ar.IsStoring())
		{	// storing code
			ar	<< stepName
				<< stepFilter
				<< UseIndex
				<< Ar0
				<< Sml
				<< Aml
				<< Lml
				<< bUnknown
				<< VMSVolume
				<< additiveVolume
				<< addVolume
				<< Ar.size();

			for(size_t i=0;i<Ar.size();i++){
				ar << Ar[i];
			}
		}
		else
		{	// loading code
			size_t i;
			ar	>> stepName
				>> stepFilter
				>> UseIndex
				>> Ar0
				>> Sml
				>> Aml
				>> Lml
				>> bUnknown
				>> VMSVolume
				>> additiveVolume
				>> addVolume
				>> i;

			Ar.assign(i,0);
			for(i=0;i<Ar.size();i++){
				ar >> Ar[i];
			}
		}
	};

	/////////////////////////////////////get data///////////////////////////////////////////

    ///
    /// \brief TotalVolume
    /// \return
    ///溶液体积
	double TotalVolume(void) const{
		return VMSVolume+additiveVolume;
	};

    ///
    /// \brief AConc
    /// \return
    ///加速剂浓度
	double AConc(void) const{
		if(bUnknown)
			return -1;

		return Aml/TotalVolume();
	};
    ///
    /// \brief SConc
    /// \return
    ///抑制剂浓度
	double SConc(void) const{
		if(bUnknown)
			return -1;

		return Sml/TotalVolume();
	};
    ///
    /// \brief LConc
    /// \return
    ///整平剂浓度
	double LConc(void) const{
		if(bUnknown)
			return -1;

		return Lml/TotalVolume();
	};

    ///
    /// \brief GetStepName
    /// 步骤名
    /// \param i
    /// \return
    ///
	CString GetStepName(int i=-1) const{
		CString str,strt;

		if(stepFilter&DOA_VMS){
			str.LoadStringW(IDS_STRING_VMS);
		}
		else{
			str.LoadStringW(IDS_STRING_STEPNAME1);
			//str+=L" ";
			if(stepFilter&DOA_SAMPLE){
				strt.LoadStringW(IDS_STRING_SAMPLE);
				str+=strt;
			}
			else{				
				if(stepFilter&DOA_S){
					strt.LoadStringW(IDS_STRING_S);
					str+=strt;
				}
				if(stepFilter&DOA_A){
					strt.LoadStringW(IDS_STRING_A);
					str+=strt;
				}
				if(stepFilter&DOA_L){
					strt.LoadStringW(IDS_STRING_L);
					str+=strt;
				}
			}
					

			if(i>0){
				strt.Format(L" %d",i);
				str+=strt;
			}

		}

		return str;
	};

    ///
    /// \brief ArUse
    /// \return
    ///最后采用的电量值
	double ArUse(void) const{
		if( Ar.empty())
			return -100;

		if(UseIndex>=0 && ((size_t)UseIndex)<Ar.size())
			return Ar[UseIndex];

		return Ar.back();

	};

    ///
    /// \brief GetX
    /// 取用于画图的横坐标数据
    /// \param plotFilter
    /// 画图限制条件
    /// \param xlabel
    /// 横坐标名
    /// \return
    /// 横坐标数值
    ///
	double GetX(BYTE plotFilter, CString &xlabel) const{
		double x=additiveVolume;
		CString str;
		if( plotFilter&PF_S ){
			str.LoadStringW(IDS_STRING_SUPPRESSOR);
			str+=L" ";
			x=SConc();
		}
		else{
			if( plotFilter&PF_A ){
				str.LoadStringW(IDS_STRING_ACCELERATOR);
				str+=L" ";
				x=AConc();
			}
			else{
				if( plotFilter&PF_L ){
					str.LoadStringW(IDS_STRING_LEVELER);
					str+=L" ";
					x=LConc();
				}
				else{
					str="";
				}
			}
		}

		xlabel=str;

		if( plotFilter&PF_CONCERTRATION ){
			str.LoadStringW(IDS_STRING_CONC_);
		}
		else{
			str.LoadStringW(IDS_STRING_VOL_);
			x=additiveVolume;
		}

		xlabel+=str;

		return x;

	};

    ///
    /// \brief GetY
    /// 取用于画图的纵坐标数据
    /// \param plotFilter
    /// 画图限制条件
    /// \param ylabel
    /// 纵坐标名
    /// \return
    /// 纵坐标数值
    ///
	double GetY(BYTE plotFilter, CString &ylabel) const{
		if( plotFilter&PF_Q_NORMALIZED ){
			ylabel.LoadStringW(IDS_STRING_NORMALIZED_Q);
			return ArUse()/Ar0;
		}
		//else{
		ylabel.LoadStringW(IDS_STRING_CHARGE_Q);
		return ArUse();
		//}
	};

    ///
    /// \brief EndFlag
    /// 判断循环次数是否足够
    /// 是否要停止循环
    /// \param nCycle
    /// 设定的目标循环次数
    /// \param tolarance
    /// 设定的容限
    /// \return
    ///
    ///
	bool EndFlag(size_t nCycle, double tolarance) const{
		//if( Ar.size()>=nCycle || (Ar.size()>1 && VarianceStd(Ar)<tolarance) ){
		//	
		//	TRACE("\nv=%g\n",VarianceStd(Ar));
		//	return true;//step should end
		//}
		//return false;//step should continue

		///////////////////////////////////////////////////////////

		

		if( (nCycle>0 && Ar.size()>=nCycle) 
			|| (Ar.size()>1 && abs(Ar[Ar.size()-2]/Ar.back()-1)<tolarance)
			){
				return true;
		}

		return false;

	};

	///////////////////////////////////set data /////////////////////////////////////////

    ///
    /// \brief ResetCompound
    ///重设溶液成分
    void ResetCompound(void){
		bUnknown=false;
		Aml=0;
		Lml=0;
		Sml=0;
	};

    ///
    /// \brief VMSOnce
    /// 加电镀基液
    /// \param sapi
    /// 加液参数
    ///
	void VMSOnce(sapitemA sapi){
		Ar0=0;

		addVolume=sapi.volconc;
		VMSVolume=addVolume;
		additiveVolume=0;

		ResetCompound();

	};
    ///
    /// \brief VolOnce
    /// 按体积加一次
    /// \param sapi
    /// 加液参数
    /// \return
    ///
	bool VolOnce(sapitemA sapi){
		addVolume=sapi.volconc;
		additiveVolume+=addVolume;

		bUnknown|=sapi.isUnknownComposition();

		if(!bUnknown){
			Aml+=sapi.Aconc*addVolume;
			Lml+=sapi.Lconc*addVolume;
			Sml+=sapi.Sconc*addVolume;
		}
		return true;
	};

    ///
    /// \brief ConcOnce
    /// 按浓度加一次
    /// \param ml
    /// 某成分的量
    /// 可以是抑制剂加速剂整平剂三者取其一
    /// \param endConc
    /// 某成分的目标浓度
    /// \param addConc
    /// 添加的溶液中某成分的浓度
    /// \return
    ///
	bool ConcOnce(double &ml, double endConc, double addConc){
		double tmp1=endConc-ml/TotalVolume();
		double tmp2=addConc-endConc;
		if(tmp1*tmp2>0){
			addVolume=TotalVolume()*tmp1/tmp2;
			ml+=addConc*addVolume;

			additiveVolume+=addVolume;

			//CString str;
			//str.LoadStringW(IDS_STRING_STEPNAME1);
			//doa.stepName.Format(L"%s %d",str,stepCount);
			//bUpdateAr0=false;

			return true;

		}
		return false;

	};
    ///
    /// \brief ConcIntv
    /// 按浓度增量加一次
    /// \param ml
    /// 某成分的量
    /// 可以是抑制剂加速剂整平剂三者取其一
    /// \param intvConc
    /// 某成分的目标浓度增量
    /// \param addConc
    /// 添加的溶液中某成分的浓度
    /// \return
    ///
	bool ConcIntv(double &ml, double intvConc, double addConc){
		double tmp1=ml/TotalVolume()-addConc;//tmp1>0, dilute, tmp1<0, concertrate
		tmp1=abs(tmp1)/intvConc-1;
		if(tmp1>0){
			addVolume=TotalVolume()/tmp1;
			ml+=addConc*addVolume;

			additiveVolume+=addVolume;

			//CString str;
			//str.LoadStringW(IDS_STRING_STEPNAME1);
			//stepName.Format(L"%s %d",str,stepCount);
			//bUpdateAr0=false;

			return true;
		}
		return false;
	};

    ///
    /// \brief Update
    /// 检查加液参数是否合法并加液一次
    /// \param sapi
    /// 加液参数
    /// \param bFlag
    /// 对加液参数的限制条件
    /// \return
    ///加液成功为真
    /// 否则为假
    ///
	bool Update(sapitemA sapi, BYTE bFlag=DOA_VMS){
		if(sapi.volconc<=0)
			return false;

		if(bFlag&DOA_VMS){
			if(sapi.addType!=4)
				return false;
		}
		else{

			if(sapi.isUnknownComposition()){
				if(!(bFlag&DOA_SAMPLE))
					return false;
			}
			else{

				if( (sapi.Sconc!=0) && !(bFlag&DOA_S) )
					return false;

				if( (sapi.Aconc!=0) && !(bFlag&DOA_A) )
					return false;

				if( (sapi.Lconc!=0) && !(bFlag&DOA_L) )
					return false;
			}
		}



		switch(sapi.addType){
		case 0:
			{
				if(VolOnce(sapi)){
					stepFilter=bFlag;
					stepName=GetStepName();					
					return true;
				}
				return false;

			}

		case 1:
			{
				if( (ArUse()/Ar0>sapi.endRatio)^(bFlag&DOA_MORE) ){
					if(VolOnce(sapi)){
						stepFilter=bFlag;
						stepName=GetStepName();					
						return true;
					}

				}

				return false;

			}

		case 2:
			{
				if( bUnknown || sapi.isMixedComposition() || sapi.isUnknownComposition() )
					return false;

				if( (sapi.Sconc>0 && ConcOnce(Sml,sapi.volconc,sapi.Sconc))
					||(sapi.Aconc>0 && ConcOnce(Aml,sapi.volconc,sapi.Aconc))
					||(sapi.Lconc>0 && ConcOnce(Lml,sapi.volconc,sapi.Lconc)) )
				{
					stepFilter=bFlag;
					stepName=GetStepName();					
					return true;
				}

				return false;
			}

		case 3:

			if( (ArUse()/Ar0>sapi.endRatio)^(bFlag&DOA_MORE) ){

				if( bUnknown || sapi.isMixedComposition() || sapi.isUnknownComposition() )
					return false;

				if( (sapi.Lconc>0 && ConcIntv(Lml,sapi.volconc,sapi.Lconc))
					||(sapi.Aconc>0 && ConcIntv(Aml,sapi.volconc,sapi.Aconc))
					||(sapi.Sconc>0 && ConcIntv(Sml,sapi.volconc,sapi.Sconc)) )
				{
					stepFilter=bFlag;
					stepName=GetStepName();					
					return true;
				}

			}
			return false;
		case 4:
			{
				stepFilter=bFlag;
				stepName=GetStepName();	

				VMSOnce(sapi);

				//Ar0=0;

				//addVolume=sapi.volconc;
				//VMSVolume=addVolume;
				//additiveVolume=0;

				//bUnknown=false;

				//Aml=0;
				//Lml=0;
				//Sml=0;
			}
			return true;

		default:
			return false;
		}
		return false;
	};

	//bool Update(sapitemA sapi, BYTE bFlag, const std::vector<double> &ArList);


};


