#include "ErrorCode.h"

bool error_isSuccess(const ErrorCode code) { return code == ERROR_SUCCESS; }

const char *error_getErrorMessage(const ErrorCode code) {
  if (code == ERROR_ALLOCATION_FAILED) {
    return "memory allocation failed";
  } else if (code == ERROR_NULL) {
    return "one of the given pointers is NULL";
  } else if (code == ERROR_OUT_OF_BOUNDS) {
    return "the given index is out of the matrix bounds";
  } else if (code == ERROR_SIZES_NOT_MATCH) {
    return "the sizes of the matrixes doesn't match";
  } else if (code == ERROR_SUCCESS) {
    return "success";
  }
  return "unkown error accured";
}