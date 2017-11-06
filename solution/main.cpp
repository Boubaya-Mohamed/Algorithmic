#include<iostream>
#include<stdlib.h>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <exception>
#include <fstream>
#include <sstream>

using namespace std;

//. *********************************************************************************/
//. **
//. **
//. **                                                                              **
//. **.                   Copyright Mohamed Boubaya                                 **
//. **             fb  : https://www.facebook.com/0x68616DD61                       **
//. **             gmail :  mohamedboubaya1@gmail.com                               **
//. **             linkedin : http://linkedin.com/in/mohamed-boubaya-8517a5120/.    **
//. **
//. *******************************************************************************/


 
struct feature{
string name;
  int x;
  int y;
  unsigned int rowLine;
  unsigned int colLine;
};


struct Aline{
	int index;
	vector<feature*>* vect;
};
//********************************
std::ifstream infile("problem_big.txt");
//std::ifstream infile("problem_small.txt");
//********************************

std::vector<Aline> rowMat;
std::vector<Aline> colMat;
int n;
float maxmin=0;
feature target;


void afficher(feature x)
{
	cout <<x.name<<" ["<<x.x<<"]["<<x.y<<"]" << "\n";
}


int partitionRow (vector<feature*>& arr, int low, int high)
{
    feature* pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {

        if (((arr[j]->x)< pivot->x)||(((arr[j]->x)== pivot->x)&&((arr[j]->y)< pivot->y)))
        {
            i++;    // increment index of smaller element
            iter_swap(arr.begin() + i, arr.begin() + j);
        }
    }
    iter_swap(arr.begin() + i+1, arr.begin() + high);
    return (i + 1);
}

void quickSortRow(vector<feature*>& arr, int low, int high)
{
    if (low < high)
    {
        int pi = partitionRow(arr, low, high);
        quickSortRow(arr, low, pi - 1);
        quickSortRow(arr, pi + 1, high);
    }
}

int partitionCol (vector<feature*>& arr, int low, int high)
{
    feature* pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {

        if (((arr[j]->y)< pivot->y)||(((arr[j]->y)== pivot->y)&&((arr[j]->x)< pivot->x)))
        {
            i++;    // increment index of smaller element
            iter_swap(arr.begin() + i, arr.begin() + j);
        }
    }
    iter_swap(arr.begin() + i+1, arr.begin() + high);
    return (i + 1);
}

void quickSortCol(vector<feature*>& arr, int low, int high)
{
    if (low < high)
    {
        int pi = partitionCol(arr, low, high);
        quickSortCol(arr, low, pi - 1);
        quickSortCol(arr, pi + 1, high);
    }
}

//
pair<feature*,feature*> findLeftRightRow(vector<feature*>& v,int col)
	{
		if(v.size()==0)
			return make_pair<feature*,feature*>(NULL,NULL);
		if(v.size()==1)
		{
			if(v[0]->y>col)
				return make_pair<feature*,feature*>(NULL,v[0]);
			else if(v[0]->y<col)
				return make_pair<feature*,feature*>(v[0],NULL);
			return make_pair<feature*,feature*>(NULL,NULL);

		}
		if(v[0]->y>=col)
		{
			if(v[0]->y==col)
				return make_pair<feature*,feature*>(NULL,v[1]);
			return make_pair<feature*,feature*>(NULL,v[0]);
		}
		if(v[v.size()-1]->y<=col)
		{
			if(v[v.size()-1]->y==col)
				return make_pair<feature*,feature*>(v[v.size()-2],NULL);
			return make_pair<feature*,feature*>(v[v.size()-1],NULL);
		}
		int left=0;
		while(v[left+1]->y<col)
			++left;
		if(v[left+1]->y==col)
			return make_pair<feature*,feature*>(v[left],v[left+2]);
		else
			return make_pair<feature*,feature*>(v[left],v[left+1]);

	}
pair<feature*,feature*> findUpDownRow(vector<feature*>& v,int row)
	{
		if(v.size()==0)
			return make_pair<feature*,feature*>(NULL,NULL);
		if(v.size()==1)
		{
			if(v[0]->x>row)
				return make_pair<feature*,feature*>(NULL,v[0]);
			else if(v[0]->x<row)
				return make_pair<feature*,feature*>(v[0],NULL);
			return make_pair<feature*,feature*>(NULL,NULL);

		}
		if(v[0]->x>=row)
		{
			if(v[0]->x==row)
				return make_pair<feature*,feature*>(NULL,v[1]);
			return make_pair<feature*,feature*>(NULL,v[0]);
		}
		if(v[v.size()-1]->x<=row)
		{
			if(v[v.size()-1]->x==row)
				return make_pair<feature*,feature*>(v[v.size()-2],NULL);
			return make_pair<feature*,feature*>(v[v.size()-1],NULL);
		}
		int up=0;
		while(v[up+1]->x<row)
			++up;
		if(v[up+1]->x==row)
			return make_pair<feature*,feature*>(v[up],v[up+2]);
		else
			return make_pair<feature*,feature*>(v[up],v[up+1]);

	}
void addFeature(vector<feature*>& v,feature* x)
{
	if(x!=NULL)
		v.push_back(x);
}
void addPair(vector<feature*>& v,pair<feature*,feature*>p)
{
	addFeature(v,p.first);
	addFeature(v,p.second);
}

float pytha(int x,int y)
{
	return sqrt(pow(x,2)+pow(y,2));
}

float eucDistance(feature& fx, feature& fy)
{
	
	return pytha(abs((fx.x)-(fy.x)),abs((fx.y)-(fy.y)));
}
float findRayon(vector<feature*>& v, feature& p)
{
	if(v.size()==0)
	{
		
		return 0;
	}
	float min=eucDistance(p,*v[0]);
	for(unsigned int i=1; i<v.size();++i)
	{
		float x=eucDistance(p,*v[i]);
		if((x)<min)
			min=x;
	}

	return min;
}
void searchUp(feature& p,float& rayon,vector<feature*> v)
{
	while(abs(p.x-(v[0]->x))<rayon)
	{
		
		pair<feature*,feature*> uppers=findLeftRightRow(v,p.y);
		if(uppers.first!=NULL)
			if(eucDistance(p,*uppers.first)<rayon)
			{
				rayon=eucDistance(p,*uppers.first);
				
			}
		if(uppers.second!=NULL)
					if(eucDistance(p,*uppers.second)<rayon)
					{
						rayon=eucDistance(p,*uppers.second);
						
					}
		if(v[0]->rowLine>0)
			v=*(rowMat[v[0]->rowLine-1].vect);
		else
			break;
		
	}
}

void searchDown(feature& p,float& rayon,vector<feature*> v)
{
	while((v[0]->x)-p.x<rayon)
	{
		pair<feature*,feature*> uppers=findLeftRightRow(v,p.y);
		if(uppers.first!=NULL)
			if(eucDistance(p,*uppers.first)<rayon)
				rayon=eucDistance(p,*uppers.first);
		if(uppers.second!=NULL)
					if(eucDistance(*uppers.second,p)<rayon)
						rayon=eucDistance(*uppers.second,p);
		if(v[0]->rowLine<rowMat.size()-1)
			v=*(rowMat[v[0]->rowLine+1].vect);
		else
			break;
	}
}
int myMin(vector<feature*>* left,vector<feature*>* right,feature& p)
{
	if(left!=NULL)
	{
		if(right!=NULL)
			return min(abs((*right)[0]->y-p.y),abs(p.y-(*left)[0]->y));
		return abs(p.y-(*left)[0]->y);
	}
	else
	{
		if(right!=NULL)
			return abs((*right)[0]->y-p.y);
		return -1;
	}
}

void wrld()
{
	vector<Aline>& v=rowMat;
	target=*(*v[0].vect)[0];


	for(unsigned int i=0;i<v.size();++i)
	{
		Aline& myLine=v[i];
		vector<feature*>& myRow=(*myLine.vect);
	

		for(unsigned int j=0;j<myRow.size();++j)
		{
			feature& point=*myRow[j];
			vector<feature*> bound;
			bound.reserve(12);
			pair<feature*,feature*> RL=findLeftRightRow(*(rowMat[point.rowLine].vect),point.y);
			addPair(bound,RL);
			pair<feature*,feature*> UD=findUpDownRow(*(colMat[point.colLine].vect),point.x);
			addPair(bound,UD);
			vector<feature*>* upRow=NULL;
			vector<feature*>* downRow=NULL;
			vector<feature*>* rightCol=NULL;
			vector<feature*>* leftCol=NULL;
			if(point.rowLine>0)
			{
				upRow=rowMat[point.rowLine-1].vect;
				addPair(bound,findLeftRightRow(*upRow,point.y));

			}
			if((point.rowLine)<(rowMat.size()-1))
			{
				downRow=rowMat[point.rowLine+1].vect;
				addPair(bound,findLeftRightRow(*downRow,point.y));
			}
			if(point.colLine>0)
			{
				leftCol=colMat[point.colLine-1].vect;
				addPair(bound,findUpDownRow(*leftCol,point.x));
			}
			if((point.colLine)<(colMat.size()-1))
			{
				rightCol=colMat[point.colLine+1].vect;
				addPair(bound,findUpDownRow(*rightCol,point.x));
			}
			float rayon=findRayon(bound,point);
			if(rayon>maxmin)
			{

				int larg=myMin(rightCol,leftCol,point);
				if(upRow!=NULL)
				{
					if(larg>0)
					{
						if(pytha(larg,(*upRow)[0]->x-point.x)<rayon)
							searchUp(point,rayon,*upRow);
					}
				}

				if(downRow!=NULL)
				{
					if(larg>0)
					{
						if(pytha(larg,abs((*downRow)[0]->x-point.x))<rayon)
							searchDown(point,rayon,*downRow);
					}
				}

				if(rayon>maxmin)
				{
					maxmin=rayon;
					
					target=point;
				}
			}
		}
		
	}

}

int main(int argc, char const *argv[]) {
  //infile>>n;

  feature* pFeature;
  std::vector<feature*> rowSorted;
  std::vector<feature*> colSorted;
  rowSorted.reserve(100000);
  colSorted.reserve(100000);
  rowMat.reserve(100);
  colMat.reserve(100);
  std::string line;
  while (std::getline(infile, line))
  {
	    pFeature=new feature;
	    std::istringstream iss(line);
	    if (!(iss>> pFeature->name>>pFeature->x>>pFeature->y)) { break; }
	  
	    rowSorted.push_back(pFeature);
	    colSorted.push_back(pFeature);// error

     
  }
  n=rowSorted.size();
 


  quickSortRow(rowSorted,0,rowSorted.size()-1);
  quickSortCol(colSorted,0,colSorted.size()-1);
 



    int row;
  for(int i=0;i<n;++i)
  {
	  row=rowSorted[i]->x;
	  Aline l;
	  l.index=row;
	  l.vect=new vector<feature*>;
	  l.vect->reserve(10);
	  l.vect->push_back(rowSorted[i]);
	  rowSorted[i]->rowLine=rowMat.size();
	  if(i+1!=n)
	  {
		  while((rowSorted[i+1]->x)==(rowSorted[i]->x))
		  {
			  ++i;
			  l.vect->push_back(rowSorted[i]);
			  rowSorted[i]->rowLine=rowMat.size();
			  if(i+1==n)
				  break;
		  }
	  }
	  rowMat.push_back(l);

  }
  int col;
  for(int i=0;i<n;++i)
  {
	  col=colSorted[i]->y;
	  Aline l;
	  l.index=col;
	  l.vect=new vector<feature*>;
	  l.vect->reserve(10);
	  l.vect->push_back(colSorted[i]);
	  colSorted[i]->colLine=colMat.size();
	  if(i+1!=n)
	  {
		  while((colSorted[i+1]->y)==(colSorted[i]->y))
		  {
			  ++i;
			  l.vect->push_back(colSorted[i]);
			  colSorted[i]->colLine=colMat.size();
			  if(i+1==n)
				  break;
		  }
	  }
	  colMat.push_back(l);

  } 
  wrld();
  afficher(target);

  return 0;
}