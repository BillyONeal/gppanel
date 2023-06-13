#ifndef __BASEXY_DATA_H__
#define __BASEXY_DATA_H__

#include "gpMath.h"
#include "CMinMax.h"
#include "xyMultimapLabel.h"
#include <deque>
#include <time.h>
#include <vector>
#include <limits>

//This possible to handle a lot of data quicker
//plot just second 1000 point to display..
//#define USE_ITERATOR_INCREMENT

//#define WXSTRING(x) wxString(x.c_str(), wxConvUTF8)

/**
* Struct contains double x&y variables
* @{
*/
struct xy_s
{
public:
	double x;	//!< x value
	double y;	//!< y value

public:
	/** Default constructor
	*	@param x_	x value
	*	@param y_	y value
	*/
	xy_s(double x_, double y_);
	/** Copy constructor
	*	@param xy	class to copy
	*/
	xy_s(const xy_s& xy);

};
//! @}

/**
*  xyDeque contains X&Y values.
*  Used when not continuously x axis.
*  Example line chart where is possible
*  example: {[x,y],..} {[1,2],[3,4],[2,5],[6,3]}
*/
typedef std::deque< xy_s > xyDeque_t;

/**
* Vector for hold time_t structure
* Example when want store data with time
*/
typedef std::vector< time_t > timeList_t;


/**
*	Function that check if value is in numeric limits.
*/
template<typename T>
inline bool isanyinf(T value)
{
	return value >= std::numeric_limits<T>::min() && value <= std::numeric_limits<T>::max();
}


/**
*  baseXYData:
*  -base data class. Normal using with chart types class
*  -use xyMultimap_t for data storing
*
*  CMinMax is used for bounds checked and it storing
*  minimum and maximum values of X and Y.
*
*	gpMath possibles to calculate severals
*  mean-values from data.
*/
class baseXYData : public CMinMax, public gpMath
{
private:

	bool                    m_useTime;//!< Use time class list. Default is false
	timeList_t              m_time; //!< time container
	xyMultimap_t            m_data;	//!< graph data container
	xyMultimap_t::iterator  m_it;	//!< graph iterator for mpLayer
	xyMultimap_t::iterator  m_it_end;	//!< graph iterator for mpLayer
	xyMultimapLabel_t       m_labels;	//!< labelmap for x-y pairs

	int                     m_checkPoint;

	bool                    m_first;//!< helping when calculate min/max values

									//for mpFXY
	double                  m_currentBoundaryXmin;
	double                  m_currentBoundaryXmax;
	int                     m_iteratorIncrement;
	int                     m_showOnlyLastN;

public:
	/** Default constructor */
	baseXYData();
	/** Copy constructor
	*	@param[in]	baseXYData	copy from class
	*/
	explicit baseXYData(const baseXYData& copy);
	/**
	*  Default destructor
	*/
	virtual ~baseXYData();

	/** Get smallest X value, which Y-value  not zero*/
	double GetMinXwhichNotZero();
	double GetMaxXwitchNotZero();

	//void ShowOnlyLast(int n){ m_showOnlyLastN = n; }

	bool IsEmpty();
	int  size() const;

	bool IsTimeHolderEnabled() const;
	void UseTimeHolder(bool hold);
	const timeList_t* GetTimeHolder();
	void SetTimeHolder(const timeList_t* timeList);

	xyMultimap_t GetData() const;
	xyMultimap_t* GetDataP();
	xyMultimap_t GetLastData(int amount);

	/**
	* Set check point. Idea that can push data,
	* so that checkpoint is like offset.
	*/
	void CheckPoint();
	/**
	*   Fill zeros if missing.
	*  [0] = 2
	*  [2] = 4 -> [1]<=0
	*  [3] = 5
	*  [6] = 4 -> [4]=[5]<=0
	*/
	void FillZeros(double min, double max, double intervall = 1.0);
	void InitZeros();
	/**
	*  Get swapped X-Y to Y-X of chart data
	*
	*/
	/*xyMultimap_t GetSwappedData() const;*/
	xyMultimap_t GetDataFromCheckPoint();

	/**
	*  defines the minimum and maximum values for X and Y
	*/
	/*void SetXMinMax(double min, double max)
	{
	CMinMax::SetMinX(min);
	CMinMax::SetMaxX(max);
	}*/
	/**
	*  defines the minimum and maximum values for X and Y
	*/
	/*void SetYMinMax(double min, double max);*/
	/**
	*  Lock min max values for chart
	*/
	//void LockX(bool b=true);
	//void LockY(bool b=true);

	/**
	* base XY data functions
	*/

	/**
	*  Set new data for graph
	*  (param itBegin  iterator where start inserting)
	*  @param newdata  xyMultimap_t data
	*/
	void DataSet(xyMultimap_t newdata);
	/**
	*  Set new data from pointer for graph
	*  (param itBegin  iterator where start inserting)
	*  @param *newdata  xyMultimap_t data
	*/
	void DataSetFromPointer(xyMultimap_t *newdata);
	/**
	*  alternative function to push xyMultimap_t
	*  @param data	Data to be push
	*/
	void DataPush(xyMultimap_t data);
	/**
	*	Data push with label
	*	@param[in] x	X value
	*	@param[in] y	y value
	*	@param[in] label	label for 2d-point
	*/
	void DataPush(double &x, double &y, wxString &label);
	/**
	*  alternative function to push only one x&y values to graph
	*  @param x X value
	*  @param y Y value
	*	@return iterator to multimap
	*/
	/*void DataPush(double &x, double &y )
	{
	(void)(xyMultimap_t::iterator)DataPush(double &x, double &y);
	}*/
	xyMultimap_t::iterator DataPush(double &x, double &y);
	xyMultimap_t::iterator DataPushUniq(double &x, double &y);
	/** Clear whole data */
	void DataClear();
	/**
	* get reference data map
	* @return xyMultimap_t	reference
	*/

	//xyMultimap_t& GetData();
	/** Get Data without zeros */
	xyMultimap_t GetDataWithoutZeros();


	/**
	*
	*	@param xmin	min value to bounds
	*	@param xmax max value to bounds
	*/
	void CurrentBounds(double& xmin, double& xmax);

	/* Virtual functions for gpMath */

	/**
	*	Virtual function (gpMath)
	*  Rewind data iterator
	*/
	virtual void   Rewind();
	virtual void Forward();

	/**
	*	Virtual function (gpMath)
	*	Get next X Y pair with label
	*/
	virtual bool GetNextXY(double & x, double & y, wxString & label);
	/**
	*	Virtual function (gpMath)
	*	Get next X Y pair
	*/
	virtual bool GetNextXY(double & x, double & y);
	/**
	* Virtual function: get Previous
	*/
	virtual bool GetPrevXY(double & x, double & y);
	/**
	*	multiply all Y values and increment offset
	*	@param mul		multiply
	*	@param offset	add value
	*/
	void MultiplyAllYvalues(double mul, double offset = 0);
	/**
	*	multiply all X values and increment offset
	*	@param mul		multiply
	*	@param offset	add value
	*/
	void MultiplyAllXvalues(double mul, double offset = 0);
	/* Below functions for:
	Get various type of xyMultimap_t (example for graph)
	-GetCustom()				for custom formula
	-GetINL()				for integral non linearity
	-GetDNL()				for differential non linearity
	-GetFft()				for Fast fourier transfer
	-GetAccumulation()		for the number of times each value occur in Y
	*/
	/**
	* Convert X axis to time (use time vectors)
	*/
	void ConvertToXtime();
	/**
	*	Get custom formula calculated data
	*	variable can be used: "X" = x value, "Y" = y value
	*	@param xformula	formula for X values
	*	@param yformula	formula for Y values
	*	@return xyMultimap_t
	*/
	xyMultimap_t GetCustom(wxString Xformula, wxString Yformula);
	/**
	*	Calculate fft from data
	*	@param len			If len == -1 fft length is as long than can be
	*	@param samplerate 	how many samples are in seconds
	*	@return xyMultimap_t
	*/
	xyMultimap_t GetFft(int len = -1, double samplerate = 1.0);
	/**
	*  Deviation of code midpoint from ideal location
	*  -if arithMean is selected function calculate arithmetic
	*	 mean first and select that to the Expected value for each Y points.
	*	 (Excpected than Y axis is Equal distribution)
	*	-If En is selected User is defined own excpected value.
	*	 (Excpected than Y axis is Equal distribution)
	*	-If Evect is selected, std::vector<double> must be same size than data in class.
	*	 Then every Y points can be select differents excpected values.
	*	 (NOT Excpected than Y axis is Equal distribution)
	*  x <= x
	*  y <= y - arithMean					//Y axis Equal distribution
	*
	*  y <= y / ( ysum / (maxX+1) ) - 1    //used when all parameters is undefined or En is selected
	*										//Expected than X axis is continously 0...n
	*										//and Y axis Equal distribution
	*
	* strange formula:
	* DNLw = (SUM(w) - a) / a  (a= y/SUM(w))
	* http://www.metrology.pg.gda.pl/full/2006/M&MS_2006_161.pdf
	*
	*	@param arithMean use arithmetic mean
	*	@param En Expected Y value
	*	@param Evect  ecpected vector pointer. If null not in use.
	*
	*/
	xyMultimap_t GetDNL(bool arithMean = false, double *En = 0, std::vector<double> *Evect = 0);
	/**
	*  Integrate nonlinearity
	*  Like DNL, but now increment y value:
	*  sum=0;
	*  {sum <= sum + y
	*   y <= sum }
	*	@param arithMean	use arithmetic mean
	*	@param En	Expected value (pointer)
	*	@param Evect	Expected values in vector (pointer)
	*	@return xyMultimap_t
	*/
	xyMultimap_t GetINL(bool arithMean = false, double *En = 0, std::vector<double> *Evect = 0);
	xyMultimap_t GetINL(xyMultimap_t in);

	/**
	*	calculates the number of times each value occur in Y
	*	@param inProcents
	*	@return xyMultimap_t
	*/
	xyMultimap_t GetAccumulation(bool inProcents = false);

	xyMultimap_t GetDataWithoutEndsZeros();

private:

	//for gpMath
	virtual bool getNextXY(double& x, double& y);
	virtual void resetI();

};

#endif