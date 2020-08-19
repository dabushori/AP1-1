#include "ErrorCode.h"

bool error_isSuccess(const ErrorCode code) {
  if (code == ERROR_SUCCESS) {
    return true;
  }
  return false;
}

const char *error_getErrorMessage(const ErrorCode code) {
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