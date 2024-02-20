///////////////////////// TO-DO (1) //////////////////////////////
  /// Include necessary header files
  /// Hint:  Include what you use, use what you include
  ///
  /// Do not put anything else in this section, i.e. comments, classes, functions, etc.  Only #include directives

#include <cstddef>    // For size_t
#include <filesystem> // For filesystem operations
#include <fstream>    // For file operations
#include <string>     // For string operations
#include <utility>    // For utility functions like move
#include "GroceryItem.hpp"
#include "GroceryItemDatabase.hpp"
/////////////////////// END-TO-DO (1) ////////////////////////////



// Return a reference to the one and only instance of the database
GroceryItemDatabase & GroceryItemDatabase::instance()
{
  // Want to probe for persistent database file only the first time called.  By making a (Lambda) function that returns the results
  // of the probe and then calling that when fist construction the instance ensure all this probing stuff happens only the first
  // time instance() is called.
  auto getFileName = []()
  {
    std::string filename;

    // Look for a prioritized list of database files in the current working directory to use
    // Don't forget to #include <filesystem> to get visibility to the exists() function
    if     ( filename = "Grocery_UPC_Database-Full.dat"  ;   std::filesystem::exists( filename ) ) /* intentionally empty*/ ;
    else if( filename = "Grocery_UPC_Database-Large.dat" ;   std::filesystem::exists( filename ) ) /* intentionally empty*/ ;
    else if( filename = "Grocery_UPC_Database-Medium.dat";   std::filesystem::exists( filename ) ) /* intentionally empty*/ ;
    else if( filename = "Grocery_UPC_Database-Small.dat" ;   std::filesystem::exists( filename ) ) /* intentionally empty*/ ;
    else if( filename = "Sample_GroceryItem_Database.dat";   std::filesystem::exists( filename ) ) /* intentionally empty*/ ;
    else     filename.clear();

    return filename;
  };

  static GroceryItemDatabase theInstance( getFileName() );
  return theInstance;
}




// Construction
GroceryItemDatabase::GroceryItemDatabase( const std::string & filename )
{
  std::ifstream fin( filename, std::ios::binary );
  if( !fin.is_open() ) std::cerr << "Warning:  Could not open persistent grocery item database file \"" << filename << "\".  Proceeding with empty database\n\n";

  // The file contains grocery items separated by whitespace.  A grocery item has 4 pieces of data delimited with a comma.  (This
  // exactly matches the previous assignment as to how GroceryItems are read)
  //
  //       Field            Type            Notes
  //       --------------------------------------------------------------------------------------------------
  //  1.   UPC Code         String          Unique identifier (primary key), always enclosed in double quotes
  //  2.   Brand Name       String          May contain spaces, always enclosed in double quotes
  //  3.   Product Name     String          May contain spaces, always enclosed in double quotes
  //  4.   Price            Floating Point  In dollars
  //
  //  Example:
  //    "00024600017008",   "Morton",         "Morton Kosher Salt Coarse",                                    15.17
  //    "00033674100066",   "Nature's Way",   "Nature's Way Forskohlii - 60 Ct",                              6.11
  //    "00041520893307",   "Smart Living",   "Smart Living 10.5\" X 8\" 3 Subject Notebook College Ruled",   18.98
  //
  //  Note: double quotes within the string are escaped with the backslash character
  //

  ///////////////////////// TO-DO (2) //////////////////////////////
    /// Hint:  Use your GroceryItem's extraction operator to read GroceryItems, don't reinvent that here.
    ///        Read grocery items until end of file pushing each grocery item into the data store as they're read.

    // Read GroceryItems from the file
  GroceryItem item;

  while (fin >> item) {
    _data[item.upcCode()] = std::move(item);
  }

// Note: The file is intentionally not explicitly closed. 
// It will be closed when 'fin' goes out of scope due to RAII (Resource Acquisition Is Initialization).
  
  /////////////////////// END-TO-DO (2) ////////////////////////////

  // Note:  The file is intentionally not explicitly closed.  The file is closed when fin goes out of scope - for whatever
  //        reason.  More precisely, the object named "fin" is destroyed when it goes out of scope and the file is closed in the
  //        destructor. See RAII
}









///////////////////////// TO-DO (3) //////////////////////////////
  /// Implement the rest of the interface, including functions find and size
  ///
  /// In the last assignment you implemented GroceryItemDatabase::find() as a recursive linear search (an O(n) operation).  In this
  /// assignment, implement GroceryItemDatabase::find() as a binary search (an O(log n) operation) by delegating to the std::map's binary
  /// search function find().

  // Find function using binary search
GroceryItem * GroceryItemDatabase::find( const std::string & upc )
{
  // map's find function uses binary search tree
  auto it = _data.find( upc );

  // if upc is found in _data map, return address of item
  if( it != _data.end() )
  {
    return &(it->second);
  }
  // if upc cannot be found, return nullptr
  else
  {
  return nullptr;
  }
}


// Size function to get the number of items in the database
std::size_t GroceryItemDatabase::size() const {
  return _data.size();
}
/////////////////////// END-TO-DO (3) ////////////////////////////
