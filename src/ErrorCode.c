#include "ErrorCode.h"

/**
 * @brief Checks if a given error code indicates a success or not.
 *
 * @param[in] code the error code.
 * @return whether the error code indicates a success or not.
 */
bool error_isSuccess(ErrorCode code) {
  if (code == ERROR_SUCCESS) {
    return true;
  }
  return false;
}

/**
 * @brief gets a textual error message for a given error code.
 *
 * @param[in] code the error code.
 * @return const char* the textual representation of the error code.
 */
const char *error_getErrorMessage(ErrorCode code) {
  switch (code) {
  case ERROR_ALLOCATION_FAILED:
    return "MEMORY ALLOCATION FAILED";
    break;
  case ERROR_NULL:
    return "NULL POINTER ERROR";
    break;
  case ERROR_OUT_OF_BOUNDS:
    return "INDEX OUT OF BOUNDS ERROR";
    break;
  case ERROR_SIZES_NOT_MATCH:
    return "THE SIZES OF THE MATRICES DON'T MATCH";
    break;
  default:
    return "SUCCESS";
    break;
  }
}