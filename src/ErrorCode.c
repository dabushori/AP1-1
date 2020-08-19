#include "ErrorCode.h"

bool error_isSuccess(const ErrorCode code) { return code == ERROR_SUCCESS; }

const char *error_getErrorMessage(const ErrorCode code) {
  if (code == ERROR_ALLOCATION_FAILED) {
    return "MEMORY ALLOCATION FAILED";
  } else if (code == ERROR_NULL) {
    return "NULL POINTER ERROR";
  } else if (code == ERROR_OUT_OF_BOUNDS) {
    return "INDEX OUT OF BOUNDS ERROR";
  } else if (code == ERROR_SIZES_NOT_MATCH) {
    return "THE SIZES OF THE MATRICES DON'T MATCH";
  } else if (code == ERROR_SUCCESS) {
    return "SUCCESS";
  }
  return "UNKOWN ERROR";
}