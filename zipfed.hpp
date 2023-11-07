/** Structures used for management of zip code records
 *
 * @author Blake Nelson
 * @editor Krishna Garg
 */

#ifndef ZIPFED_HPP
#define ZIPFED_HPP

#include <strings.h>
#include <iostream>
/** @brief Zipcode can be either STANDARD zip codes or PO BOX zip code
 */
typedef enum {
  INVALID,               /**< Invalid entry for field */
  STANDARD,              /**< A zipcode for city / town / region */
  PO_BOX,                /**< A zipcode for a PO Box */
  UNIQUE,                /**< A zipcode fo unique ID */
  MILITARY               /**< Military e.g. APO */
} ZIPCODE_TYPE;

/** @brief Zip code location types can be ACCEPTABLE or NOT ACCEPTABLE
 *
 * The loctaion is flagged as acceptable or not acceptable. Our application
 * doesn't care about this setting. 
 */
typedef enum {
  NOT_ACCEPTABLE,        /**< The specified location type not acceptable */
  ACCEPTABLE             /**< The specified location type is acceptable */
} LOCATION_TYPE;

/* @brief Struct for handling full zipcode record from Federal Gov
 *
 * This structure is used for reading the input format as  US Fed Gov
 * provides the zip code data in CSV file available for free download.
 * This struct is used to read in the source. 
 */
class Zipfed {
private:
  std::string zipcode;              /**< five digits zip code stored as string */
  ZIPCODE_TYPE zctype;              /**< zip code for PO Box or standard region */
  std::string city;                 /**< Name of city for zip code */
  std::string state;                /**< State of the zip code */
  float lat;                        /**< Latitude of zip code */
  float lon;                        /**< Longitude of zip code */
public:
  Zipfed();                         /**< default constructor for Zipfed */
  int parse_zip_federal(char *csv); /**< parse and inialize from line of input */
  int parse_zip_cs2303 (char *csv);
  void print(void);
  void print(FILE * file);
  /**getter method for the city field
  *@return the string of the city of a Zipfed object
  **/
  std::string get_city(){return city;}
  /**getter method for the zip code of a Zipfed object
  *@return the string of the zip of a Zipfed object
  **/
  std::string get_zip(){return zipcode;}
};
#endif // ZIPSTRUCTS
