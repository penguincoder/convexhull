#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <getopt.h>

/*
	finds the area in a given set of points. all the arrays will be named points
	and i really don't feel like having this thing in several places in my code
	
	it's huge
*/
#define AREA(a,b,c)	( points[a].x * points[b].y + points[c].x * points[a].y + points[b].x * points[c].y - points[c].x * points[b].y - points[b].x * points[a].y - points[a].x * points[c].y )

/* maximum size of coordinate plane */
#define COORDMAX 500
/* maximum number of points to compute */
#define POINTMAX 5000

using namespace std;

ofstream out;

/* the class that I will use to wrap a coordinate using oo design */
class Point	{
public:
	short x, y;
	Point()	{
		Reset();
	}

	Point ( short one, short two )	{
		x = one; y = two;
	}

	void Reset()	{
		x = -1;
		y = -1;
	}
	
	double Slope ( const Point &end )	{
		double result = 0.0;
		result = ( this->y - end.y ) / ( this->x - end.x );
		return result;
	}
	
	/* overloaded C++ operators for various syntax simplifications */
	friend ostream& operator<< ( ostream &out, const Point &p )	{
		out << p.x << " " << p.y;
		return out;
	}
	
	friend ostream& operator<< ( ostream &out, const Point *p )	{
		out << p->x << " " << p->y;
		return out;
	}
	
	Point& operator= ( const Point &p )	{
		this->x = p.x;
		this->y = p.y;
		return *this;
	}
	
	Point& operator= ( const Point *p )	{
		this->x = p->x;
		this->y = p->y;
		return *this;
	}

	bool operator== ( const Point &p )	{
		return ( this->x == p.x && this->y == p.y );
	}
	
	bool operator!= ( const Point &p )	{
		return ( this->x != p.x && this->y != p.y );
	}
};

/*
	this function reads in all of the points in a file, up to POINTMAX in size
	since we don't include a size field in the input file
*/
Point *CreateByFile ( const char *name, long *size )	{
	
	ifstream infile ( name );
	Point *points;
	
	*size = 0;
	points = new Point[POINTMAX];
	if ( points == NULL )	{return NULL;}
	
	while ( !infile.eof() )	{
		short x = 0, y = 0;
		infile >> x;
		infile >> y;
		if ( x != 0 && y != 0 )	{
			points[*size].x = x;
			points[*size].y = y;
			(*size)++;
		}
		if ( *size >= POINTMAX )	{break;}
	}
	infile.close();
	return points;
}

/* this function creates an randomly sized array of Points randomly selected between 1 and COORDMAX */
Point *CreateByRandom ( long *size, bool flag )	{
	Point *hull;

	srand ( time ( NULL ) );
	if ( !flag )
		*size = rand() % POINTMAX + 1;
	hull = new Point[*size];
	if ( hull == NULL )
		return hull;
	for ( long count = 0; count < *size; count++ )	{
		hull[count].x = rand() % COORDMAX + 1;
		hull[count].y = rand() % COORDMAX + 1;
	}

	return hull;
}

/* this function checks two coordinates against a set of Points for an edge */
bool ConvexHullContains ( Point *points, long size, long first, long second )	{
	long count = 0;
	double d = 0.0;
	bool above = false, below = false;
	
	/* check all the points. points may or may not be sorted */
	for ( count = 0; count < size; count++ )	{
		/* don't check the checking points */
		if ( count == first || count == second )	{continue;}
		
		/* area from the book algorithm */
		d = AREA(first, second, count);
		/*
			one side or the other. it doesn't matter.
			by setting one flag and not the other, you can check to see which
			side of the line the points are on; only on *one* side of the line.
		*/
		if ( d > 0.0 )	{
			if ( below )	{return false;}
			above = true;
		}
		else if ( d < 0.0 )	{
			if ( above )	{return false;}
			below = true;
		}
		/*
			not really sure about this one. doesn't work otherwise.
			i don't know why the area would be zero, but it gives me strange
			hulls if i don't do this
		*/
		else
			return false;
	}
	
	/* finished the 'gauntlet' of checks, we have a winner */
	return true;
}

/*
	Brute Force convex hull smashing. Figures out which points are on the edge
	by generating all possible permutations of the point set
*/
void SmashHull ( Point *points, long size, bool *mark )	{
	long hullpoint = 0, count = 0, firsty = -1;
	/* blindly adds any points that are on the edge to the mark array */
	for ( hullpoint = 0; hullpoint < size; hullpoint++ )	{
		for ( count = 0; count < size; count++ )	{
			if ( hullpoint == count )	{continue;}
			if ( ConvexHullContains ( points, size, hullpoint, count ) )	{mark[hullpoint] = true;}
		}
	}

	/* this doesn't really output the points in order, but it does a generally decent job */
	long b = 0;
	for ( count = 0; count < size; count++ )	{b += points[count].y;}
	double t = b / size;
	for ( count = 0; count < size; count++ )	{
		if ( mark[count] && points[count].y >= t )
			out << points[count] << endl;
	}
	for ( count = size - 1; count >= 0 ; count-- )	{
		if ( mark[count] && points[count].y < t )
			out << points[count] << endl;
	}
}

/*
	a recursive function that determines the convex hull of a point set using
	a method similar to quicksort
*/
void QuickHull ( Point *points, long left, long right, bool *mark, bool flag )	{
	long count = 0, maxindex = 0;
	double area = 0.0, maxarea = 0.0;
	
	for ( count = left; count < right; count++ )	{
		area = AREA( left, right, count );
		if ( (area > maxarea && flag) || (area < maxarea && !flag) )	{maxarea = area;maxindex = count;}
	}

	if ( maxindex > 0 )	{
		mark[maxindex] = true;
		if ( flag )	{
			QuickHull ( points, left, maxindex, mark, flag );
			out << points[maxindex] << endl;
			QuickHull ( points, maxindex, right, mark, flag );
		}
		else	{
			QuickHull ( points, maxindex, right, mark, flag );
			out << points[maxindex] << endl;
			QuickHull ( points, left, maxindex, mark, flag );
		}
	}
}

/*
	this is a C qsort compatible comparison function
	returns -1 when a < b, 1 when a > b, and 0 when a == b
*/
int ComparePoints ( const void *a, const void *b )	{
	Point *one = (Point *)a;
	Point *two = (Point *)b;

	if ( one->x < two->x )	{return -1;}
	else if ( one->x > two->x )	{return 1;}
	else	{
		if ( one->y < two->y )	{return -1;}
		else if ( one->y > two->y )	{return 1;}
		else	{return 0;}
	}
}

int main ( int argc, char **argv )	{
	Point *x = NULL;
	short firsty = -1;
	bool algorithm = false, comparative = false, outputflag = true;
	int arg = 0, argindex = 0;
	long size = 0, count = 0, t = 0;
	char *outputname = NULL, *inputname = NULL;
	
	/* hooray for getopt */
	struct option longopts[] =	{
		{"p", 1, NULL, 'p'},
		{"b", 0, NULL, 'b'},
		{"i", 1, NULL, 'i'},
		{"o", 1, NULL, 'o'},
		{"h", 0, NULL, 'h'},
		{"help", 0, NULL, 'h'},
		{"?", 0, NULL, 'h'}
	};
	
	while ( 1 )	{
		arg = getopt_long ( argc, argv, "p:bi:o:h", longopts, &argindex );
		if ( arg == -1 )	{break;}
		
		switch ( arg )	{
			case 'p':
				comparative = true;
				size = atol ( optarg );
				break;
			case 'b':
				algorithm = true;
				break;
			case 'i':
				inputname = optarg;
				break;
			case 'o':
				outputname = optarg;
				outputflag = false;
				break;
			case 'h':
				cout << "Usage: " << argv[0] << " [options] --i <infile> --o <outfile>" << endl;
				cout << "\tOptions" << endl;
				cout << "\t--b\tChanges algorithm to brute force (default is quickhull)" << endl;
				cout << "\t--p\tSpecifies problem size of random generation" << endl;
				exit ( 0 );
				break;
			default:
				break;
		}
	}
	/* end getopt */
		
	if ( outputflag )	{
		outputname = new char[ strlen( argv[0] ) + 5 ];
		strcpy ( outputname, argv[0] );
		strcat ( outputname, ".out" );
	}
	
	out.open ( outputname );
	
	if ( inputname == NULL )	{x = CreateByRandom ( &size, comparative );}
	else	{x = CreateByFile ( inputname, &size );}
	
	/* sort and output the point set */
	qsort ( x, size, sizeof( Point ), &ComparePoints );
	for ( count = 0; count < size; count++ )	{out << x[count] << endl;}
	out << endl;
	
	/* calculate convex hull and output results as they are found */
	bool result[size];
	for ( count = 0; count < size; count++ )	{result[count] = false;}
	result[0] = true;result[size - 1] = true;
	if ( algorithm )	{
		SmashHull ( x, size, result );
	}
	else	{
		out << x[0] << endl;
		QuickHull ( x, 0, size - 1, result, true );
		out << x[size - 1] << endl;
		QuickHull ( x, 0, size - 1, result, false );
	}
	
	out.close();
	delete [] x;
	if ( outputflag )	{delete [] outputname;}
	
	return 0;
}
