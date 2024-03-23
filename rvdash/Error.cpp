#include "Error.h"




namespace rvdash {

void failWithError(const std::string &Msg) {
  throw std::logic_error("\n\n" + Msg);
}

} // namespace rvdash


