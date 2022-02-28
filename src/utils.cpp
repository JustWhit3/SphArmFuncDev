//STD headers
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <array>
#include <string>
#include <functional>
#include <limits>

//Extra headers
#include <osmanip/manipulators/csmanip.hpp>

//My headers
#include "../include/utils.hpp"

namespace SphArmFuncDev
 {
  //============================================
  //     "runtime_thrower" function definition
  //============================================
  
  //Function used to return customized runtime error.
  template <typename T>
  std::runtime_error runtime_thrower( T phrase )
   { 
    return std::runtime_error( osm::feat( osm::col, "red" ) + phrase + osm::reset( "color" ) );
   }
  
  template std::runtime_error runtime_thrower <const std::string> ( const std::string phrase );
  template std::runtime_error runtime_thrower <const char*> ( const char* phrase );


  //============================================
  //     "h" function definition
  //============================================
  
  //Function used to return the derivative step-size.
  const double h( const int n, const double x_0 )
   {
    if ( n < 4 && n > 0 ) return STEP_SIZE * x_0;
    else return n * 0.09;
   }
  
  //============================================
  //     "n_derivative" function definition
  //============================================
  
  //Function used to calculate the "n"-th derivative of a function "f" in a point "x_0", which depends on an index "a". 
  //NB: Works well until n = 3/4. 
  const double n_derivative( std::function<const double( int, double )> f, const double x_0, const int a, const int n )
   {
    if( n == 0 ) return f( a, x_0 );
    else if( n < 0 ) throw runtime_thrower( "Derivative cannot be calculated for order less than 0!" );
    else 
     {
      if ( fabs( x_0 ) >= __DBL_MIN__ && std::isfinite( x_0 ) )
       {
        const double x_1 = x_0 - h( n, x_0 );
        const double x_2 = x_0 + h( n, x_0 );
        const double first_term = n_derivative( f, x_2, a, n - 1 );
        const double second_term = n_derivative( f, x_1, a, n - 1);
    
        return ( first_term - second_term ) / ( x_2 - x_1 );
       }
      else throw runtime_thrower( "Derivative cannot be calculated in this x_0 value (too much small or big)!" );
     }
   }
  
  //============================================
  //     "round" function definition
  //============================================
  
  //Function used to round the value of a double variable.
  //NB: used for testing only.
  const double round_var( const double val )
   {
    if( val < 0 ) return ceil( val - 0.5 );
    else return floor( val + 0.5 );
   }
  
  //============================================
  //     "IsInBounds" function definition
  //============================================
  
  //Function to check if a "value" is in bound [low, high].
  //NB: used for testing only.
  template <typename T>
  bool IsInBounds( const T value, const T low, const T high )
   {
    return !( value < low ) && ( value < high );
   } 
  
  template bool IsInBounds <double> ( const double value, const double low, const double high );
  
  //============================================
  //     "integral" function definition
  //============================================
  
  //Function used to integrate a function f(x,y), depending on two indexes m and l, in x = theta and y = phi.
  const double integral( std::function<const double( std::string, int, int, double, double )> f, const std::string expr, const int m, const int l )
   {  

    
    //Calculating the number of points in x and y integral:
    const double nx = ( x_fin - x_in ) / h_x + 1;
    const double ny = ( y_fin - y_in ) / h_y + 1;

    //Integral variables:
    double res{};
    std::vector<std::vector<double>> tab(std::ceil(nx), std::vector<double>(std::ceil(ny), 0.0)); //Stores the table.
    std::vector<double> ax(std::ceil(nx), 0.0);  //Stores the integral wrt y
  
    //Calculating the values of the table:
    for( int i = 0; i < nx; ++i ) 
     {
      for ( int j = 0; j < ny; ++j ) 
       {
        tab[i][j] = f( expr, m, l, x_in + i * h_x, y_in + j * h_y );
       }
     }
  
    //Calculating the integral value wrt y at each point for x:
    for( int i = 0; i < nx; ++i ) 
     {
      ax[i] = 0;
      for ( int j = 0; j < ny; ++j ) 
       {
        if ( j == 0 || j == ny - 1 ) ax[i] += tab[i][j];
        else if ( j % 2 == 0 ) ax[i] += 2 * tab[i][j];
        else ax[i] += 4 * tab[i][j];
       }
      ax[i] *= ( h_y / 3 );
     }
  
    //Calculating the final integral value using the integral obtained in the above step:
    for ( int i = 0; i < nx; ++i ) 
     {
      if ( i == 0 || i == nx - 1 ) res += ax[i];
      else if ( i % 2 == 0 ) res += 2 * ax[i];
      else res += 4 * ax[i];
     }
    res *= ( h_x / 3 );
  
    return res;
   }

  //============================================
  //     "initializer" function definition
  //============================================
  
  //Function used to initialize values of main program input.
  //NB: used in main program only.
  void initializer( std::string& equation, int& m, int& l)
   {
    std::cout << "Enter the f(th,phi) equation shape (avoid backspaces): ";
    std::cin >> equation;
    std::cout << "Enter the value of m: ";
    std::cin >> m;
    std::cout << "Enter the value of l: ";
    std::cin >> l;
    std::cout << std::endl;
   }

  //============================================
  //     "letter" function definition
  //============================================
  
  //Function to modify an input and return it.
  //NB: used in main program only.
  const char abort_this( char letter )
   {
    std::cout << "Compute another coefficient (enter \"y\" or \"n\")?: ";
    std::cin >> letter;
    std::cout << std::endl;
    return letter;
   }
 }