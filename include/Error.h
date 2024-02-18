#ifndef ERROR_H
#define ERROR_H




namespace rvdash {

void failWithError(const std::string &Msg) {
  throw std::logic_error(Msg);
}

} // namespace rvdash

#endif // ERROR_H

